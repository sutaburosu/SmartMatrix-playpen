#include "../config.h"

uint32_t XY(uint16_t x, uint16_t y) {
  if (x >= kMatrixWidth) return 0;
  if (y >= kMatrixHeight) return 0;
  return y * kMatrixWidth + x;
}

// make sure these function signatures match your own XY()
using XY_Func = uint32_t (*)(uint16_t, uint16_t);
XY_Func YX(uint16_t y, uint16_t x) {
  return (XY_Func)XY(x, y);
}

// x and y are 24.8 fixed point
// Not Ray Wu. ;)  The idea came from Xiaolin Wu.
void wu_pixel(uint32_t x, uint32_t y, CRGB* col) {
  // extract the fractional parts and derive their inverses
  uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
// calculate the intensities for each affected pixel
#define WU_WEIGHT(a, b) ((uint8_t)(((a) * (b) + (a) + (b)) >> 8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
#undef WU_WEIGHT
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    uint16_t local_x = (x >> 8) + (i & 1);
    uint16_t local_y = (y >> 8) + ((i >> 1) & 1);
    if (local_x >= kMatrixWidth || local_y >= kMatrixHeight)
      continue;
    uint16_t xy      = XY(local_x, local_y);
    crgbleds[xy].r   = qadd8(crgbleds[xy].r, col->r * wu[i] >> 8);
    crgbleds[xy].g   = qadd8(crgbleds[xy].g, col->g * wu[i] >> 8);
    crgbleds[xy].b   = qadd8(crgbleds[xy].b, col->b * wu[i] >> 8);
  }
}

// note that this will write to NUM_LEDS + 1
void wu_pixel1d(uint32_t y, CRGB* col) {
  // extract the fractional parts and derive their inverses
  uint8_t yy = y & 0xff, iy = 255 - yy;
  y             = y >> 8;
  uint8_t wu[2] = {iy, yy};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 2; i++) {
    crgbleds[y].r = qadd8(crgbleds[y].r, col->r * wu[i] >> 8);
    crgbleds[y].g = qadd8(crgbleds[y].g, col->g * wu[i] >> 8);
    crgbleds[y].b = qadd8(crgbleds[y].b, col->b * wu[i] >> 8);
    y++;
  }
}

// blend between two colours by amount/255ths
// updates the first colour in-place
void rgbcrossfade(CRGB* a, const CRGB* b, uint8_t amount) {
  uint8_t rev = 255 - amount;
  a->red      = (a->red * amount + b->red * rev) >> 8;
  a->green    = (a->green * amount + b->green * rev) >> 8;
  a->blue     = (a->blue * amount + b->blue * rev) >> 8;
}

void rgbcrossfade8(byte* a, uint8_t amount) {
  uint8_t rev = 255 - amount;
  *a          = (*a * amount + 255 * rev) >> 8;
}

void wuLineAA8(uint8_t* fb, saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2) {
  saccum78 grad, xd;
  saccum78 xend, yend, yf;
  int8_t ix1, ix2;
  fract8 xgap, coverage;

  // reject trivially off-screen lines
  if (x1 < -255 && x2 < -255) return;
  if (y1 < -255 && y2 < -255) return;
  if ((x1 >> 8) >= kMatrixWidth && (x2 >> 8) >= kMatrixWidth) return;
  if ((y1 >> 8) >= kMatrixHeight && (y2 >> 8) >= kMatrixHeight) return;

  XY_Func xyfunc = (XY_Func)XY; // use regular XY() mapper when X is the major axis
  if (abs(x2 - x1) < abs(y2 - y1)) {
    // Y is major axis: swap X and Y and switch to the YX() mapper
    xyfunc = (XY_Func)YX;
    saccum78 tmp;
    tmp = x1;
    x1  = y1;
    y1  = tmp;
    tmp = x2;
    x2  = y2;
    y2  = tmp;
  }

  if (x2 < x1) {
    // line is backwards: reverse it
    saccum78 tmp;
    tmp = x1;
    x1  = x2;
    x2  = tmp;
    tmp = y1;
    y1  = y2;
    y2  = tmp;
  }

  xd = x2 - x1;
  // Treat very short lines as unit length
  if (xd < 25) {
    x2 = x1 + 128;
    x1 -= 128;
    grad = 0;
    // Serial.println("Vshort");
  } else {
    grad = ((saccum1516)(y2 - y1) << 8) / xd;

    // if line length is less than 1, extend it to 1
    if (xd < 256) {
      // Serial.println("Short");
      // find mid point of line
      saccum78 xm = (x1 + x2) / 2;
      saccum78 ym = (y1 + y2) / 2;

      // recalculate end points so that xd=1
      x1 = xm - 128;
      x2 = xm + 128;
      y1 = ym - (grad / 2);
      y2 = ym + (grad / 2);

      xd   = 256;
      grad = 0;
    }
  }

  // project to find coordinates of endpoint 1
  xend = (x1 + 128) & 0xff00;
  yend = y1 + ((grad * (xend - x1)) >> 8);

  // distance from beginning of line to next pixel boundary
  xgap = 255 - ((x1 /*+ 128*/) & 0xff);

  // calc pixel intensities
  coverage = ((yend & 0xff) * xgap) >> 8;
  ix1      = xend >> 8;
  rgbcrossfade8(&fb[xyfunc(ix1, (yend >> 8))], coverage);
  rgbcrossfade8(&fb[xyfunc(ix1, (yend >> 8) + 1)], 255 - coverage);

  ix1++;
  yf = yend + grad;

  // project to find coordinates of endpoint 2
  xend = (x2 + 128) & 0xff00;
  yend = y2 + ((grad * (xend - x2)) >> 8);

  // distance from end of line to previous pixel boundary
  xgap = (x2 /*- 128*/) & 0xff;

  // calc pixel intensities
  coverage = ((yend & 0xff) * xgap) >> 8;

  ix2 = xend >> 8;
  // *col = 0x0000ff;
  rgbcrossfade8(&fb[xyfunc(ix2, (yend >> 8))], coverage);
  // *col = 0xff00ff;
  rgbcrossfade8(&fb[xyfunc(ix2, (yend >> 8) + 1)], 255 - coverage);
  // *col = 0xffffff;

  while (ix1 < ix2) {
    coverage = yf & 0xff;
    rgbcrossfade8(&fb[xyfunc(ix1, yf >> 8)], coverage);
    rgbcrossfade8(&fb[xyfunc(ix1, (yf >> 8) + 1)], 255 - coverage);
    yf += grad;
    ix1++;
  }
}

void wuLineAA(saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2, CRGB* col) {
  saccum78 grad, xd;
  saccum78 xend, yend, yf;
  int8_t ix1, ix2;
  fract8 xgap, coverage;

  // reject trivially off-screen lines
  if (x1 < -255 && x2 < -255) return;
  if (y1 < -255 && y2 < -255) return;
  if ((x1 >> 8) >= kMatrixWidth && (x2 >> 8) >= kMatrixWidth) return;
  if ((y1 >> 8) >= kMatrixHeight && (y2 >> 8) >= kMatrixHeight) return;

  XY_Func xyfunc = (XY_Func)XY; // use regular XY() mapper when X is the major axis
  if (abs(x2 - x1) < abs(y2 - y1)) {
    // Y is major axis: swap X and Y and switch to the YX() mapper
    xyfunc = (XY_Func)YX;
    saccum78 tmp;
    tmp = x1;
    x1  = y1;
    y1  = tmp;
    tmp = x2;
    x2  = y2;
    y2  = tmp;
  }

  if (x2 < x1) {
    // line is backwards: reverse it
    saccum78 tmp;
    tmp = x1;
    x1  = x2;
    x2  = tmp;
    tmp = y1;
    y1  = y2;
    y2  = tmp;
  }

  xd = x2 - x1;
  // Treat very short lines as unit length
  if (xd < 25) {
    x2 = x1 + 128;
    x1 -= 128;
    grad = 0;
    // Serial.println("Vshort");
  } else {
    grad = ((saccum1516)(y2 - y1) << 8) / xd;

    // if line length is less than 1, extend it to 1
    if (xd < 256) {
      // Serial.println("Short");
      // find mid point of line
      saccum78 xm = (x1 + x2) / 2;
      saccum78 ym = (y1 + y2) / 2;

      // recalculate end points so that xd=1
      x1 = xm - 128;
      x2 = xm + 128;
      y1 = ym - (grad / 2);
      y2 = ym + (grad / 2);

      xd   = 256;
      grad = 0;
    }
  }

  // project to find coordinates of endpoint 1
  xend = (x1 + 128) & 0xff00;
  yend = y1 + ((grad * (xend - x1)) >> 8);

  // distance from beginning of line to next pixel boundary
  xgap = 255 - ((x1 /*+ 128*/) & 0xff);

  // calc pixel intensities
  coverage = ((yend & 0xff) * xgap) >> 8;
  ix1      = xend >> 8;
  // *col = 0xff0000;
  rgbcrossfade(&crgbleds[xyfunc(ix1, (yend >> 8))], col, coverage);
  // *col = 0x00ff00;
  rgbcrossfade(&crgbleds[xyfunc(ix1, (yend >> 8) + 1)], col, 255 - coverage);

  ix1++;
  yf = yend + grad;

  // project to find coordinates of endpoint 2
  xend = (x2 + 128) & 0xff00;
  yend = y2 + ((grad * (xend - x2)) >> 8);

  // distance from end of line to previous pixel boundary
  xgap = (x2 /*- 128*/) & 0xff;

  // calc pixel intensities
  coverage = ((yend & 0xff) * xgap) >> 8;

  ix2 = xend >> 8;
  // *col = 0x0000ff;
  rgbcrossfade(&crgbleds[xyfunc(ix2, (yend >> 8))], col, coverage);
  // *col = 0xff00ff;
  rgbcrossfade(&crgbleds[xyfunc(ix2, (yend >> 8) + 1)], col, 255 - coverage);
  // *col = 0xffffff;

  while (ix1 < ix2) {
    coverage = yf & 0xff;
    rgbcrossfade(&crgbleds[xyfunc(ix1, yf >> 8)], col, coverage);
    rgbcrossfade(&crgbleds[xyfunc(ix1, (yf >> 8) + 1)], col, 255 - coverage);
    yf += grad;
    ix1++;
  }
}

void wuVectorAA(const uint16_t x, const uint16_t y, const uint16_t length, const uint16_t theta, CRGB* col) {
  int16_t dx, dy;
  dx = ((int32_t)cos16(theta) * length) / 32768;
  dy = ((int32_t)sin16(theta) * length) / 32768;
  wuLineAA(x, y, x + dx, y + dy, col);
}

void wuVectorAA8(uint8_t* fb, const uint16_t x, const uint16_t y, const uint16_t length, const uint16_t theta) {
  int16_t dx, dy;
  dx = ((int32_t)cos16(theta) * length) / 32768;
  dy = ((int32_t)sin16(theta) * length) / 32768;
  wuLineAA8(fb, x, y, x + dx, y + dy);
}

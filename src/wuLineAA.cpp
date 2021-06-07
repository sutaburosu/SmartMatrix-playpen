/* someone - circa 1995-2000

  I found this code on an old Acorn Archimedes hard drive.
  I think I may have done a floating- to fixed-point conversion
  of someone else's code.

  wuLine() still has bugs at the endpoints of the lines
*/

#include "config.h"

int32_t XY(int16_t x, int16_t y) {
  if (x < 0 || x >= kMatrixWidth) return -1;
  if (y < 0 || y >= kMatrixHeight) return -1;
  return y * kMatrixWidth + x;
}

// make sure these function signatures match your own XY()
using XY_Func = int32_t (*)(int16_t, int16_t);
XY_Func YX(int16_t y, int16_t x) {
  return (XY_Func)XY(x, y);
}

// x and y are 24.8 fixed point
// Not Ray Wu. ;)  The idea came from Xiaolin Wu.
void wu_pixel(int32_t x, int32_t y, CRGB* col) {
  // extract the fractional parts and derive their inverses
  uint8_t xx = x & 0xff, yy = y & 0xff, ix = 255 - xx, iy = 255 - yy;
// calculate the intensities for each affected pixel
#define WU_WEIGHT(a, b) ((uint8_t)(((a) * (b) + (a) + (b)) >> 8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
#undef WU_WEIGHT
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    int16_t local_x = (x >> 8) + (i & 1);
    int16_t local_y = (y >> 8) + ((i >> 1) & 1);
    int32_t xy      = XY(local_x, local_y);
    if (xy < 0) continue;
    crgbleds[xy].r = qadd8(crgbleds[xy].r, col->r * wu[i] >> 8);
    crgbleds[xy].g = qadd8(crgbleds[xy].g, col->g * wu[i] >> 8);
    crgbleds[xy].b = qadd8(crgbleds[xy].b, col->b * wu[i] >> 8);
  }
}

// note that this will write to NUM_LEDS + 1
void wu_pixel1d(int16_t y, CRGB* col) {
  // extract the fractional parts and derive their inverses
  uint8_t yy    = y & 0xff,
          iy    = 255 - yy;
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

void wuLineAA8(uint8_t* fb, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
  int16_t grad, xd;
  int16_t xend, yend, yf;
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
    int16_t tmp;
    tmp = x1;
    x1  = y1;
    y1  = tmp;
    tmp = x2;
    x2  = y2;
    y2  = tmp;
  }

  if (x2 < x1) {
    // line is backwards: reverse it
    int16_t tmp;
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
      int16_t xm = (x1 + x2) / 2;
      int16_t ym = (y1 + y2) / 2;

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
  coverage   = ((yend & 0xff) * xgap) >> 8;
  ix1        = xend >> 8;
  int32_t xy = xyfunc(ix1, (yend >> 8));
  if (xy >= 0)
    rgbcrossfade8(&fb[xy], coverage);
  xy = xyfunc(ix1, (yend >> 8) + 1);
  if (xy >= 0)
    rgbcrossfade8(&fb[xy], 255 - coverage);

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
  xy  = xyfunc(ix2, (yend >> 8));
  if (xy >= 0)
    rgbcrossfade8(&fb[xy], coverage);
  xy = xyfunc(ix2, (yend >> 8) + 1);
  if (xy >= 0)
    rgbcrossfade8(&fb[xy], 255 - coverage);

  while (ix1 < ix2) {
    coverage = yf & 0xff;
    xy       = xyfunc(ix1, yf >> 8);
    if (xy >= 0)
      rgbcrossfade8(&fb[xy], coverage);
    xy = xyfunc(ix1, (yf >> 8) + 1);
    if (xy >= 0)
      rgbcrossfade8(&fb[xy], 255 - coverage);
    yf += grad;
    ix1++;
  }
}

void wuLineAA(int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB* col) {
  int16_t grad, xd;
  int16_t xend, yend, yf;
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
    int16_t tmp;
    tmp = x1;
    x1  = y1;
    y1  = tmp;
    tmp = x2;
    x2  = y2;
    y2  = tmp;
  }

  if (x2 < x1) {
    // line is backwards: reverse it
    int16_t tmp;
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
  } else {
    grad = ((int16_t)(y2 - y1) << 8) / xd;

    // if line length is less than 1, extend it to 1
    if (xd < 256) {
      // Serial.println("Short");
      // find mid point of line
      int16_t xm = (x1 + x2) / 2;
      int16_t ym = (y1 + y2) / 2;

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
  coverage   = ((yend & 0xff) * xgap) >> 8;
  ix1        = xend >> 8;
  int32_t xy = xyfunc(ix1, (yend >> 8));
  if (xy >= 0)
    rgbcrossfade(&crgbleds[xy], col, coverage);
  xy = xyfunc(ix1, (yend >> 8) + 1);
  if (xy >= 0)
    rgbcrossfade(&crgbleds[xy], col, 255 - coverage);

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
  xy  = xyfunc(ix2, (yend >> 8));
  if (xy >= 0)
    rgbcrossfade(&crgbleds[xy], col, coverage);
  xy = xyfunc(ix2, (yend >> 8) + 1);
  if (xy >= 0)
    rgbcrossfade(&crgbleds[xy], col, 255 - coverage);

  while (ix1 < ix2) {
    coverage = yf & 0xff;
    xy       = xyfunc(ix1, yf >> 8);
    if (xy >= 0)
      rgbcrossfade(&crgbleds[xy], col, coverage);
    xy = xyfunc(ix1, (yf >> 8) + 1);
    if (xy >= 0)
      rgbcrossfade(&crgbleds[xy], col, 255 - coverage);
    yf += grad;
    ix1++;
  }
}

void wuVectorAA(const int16_t x, const int16_t y, const uint16_t length, const uint16_t theta, CRGB* col) {
  int16_t dx, dy;
  dx = ((int32_t)cos16(theta) * length) / 32768;
  dy = ((int32_t)sin16(theta) * length) / 32768;
  wuLineAA(x, y, x + dx, y + dy, col);
}

void wuVectorAA8(uint8_t* fb, const int16_t x, const int16_t y, const uint16_t length, const uint16_t theta) {
  int16_t dx, dy;
  dx = ((int32_t)cos16(theta) * length) / 32768;
  dy = ((int32_t)sin16(theta) * length) / 32768;
  wuLineAA8(fb, x, y, x + dx, y + dy);
}

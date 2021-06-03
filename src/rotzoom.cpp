#include "config.h"

#define SPR_SIZE 18
#define TILE_SIZE 32

const uint32_t fish [SPR_SIZE * SPR_SIZE]  PROGMEM = { //prepare image with LCD Image Converter
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x509ee6, 0x8abcff, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x569ce1, 0x84c1f7, 0x5d9bd8, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x55a4e7, 0x7fbffd, 0x5a9fda, 0x80befb, 0x57a4e8, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x579ce1, 0x84c1f8, 0x569ce1, 0x84bff9, 0x579ce1, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x579de3, 0x84c0f6, 0x549de2, 0x84c1f8, 0x579be4, 0x84bff7, 0x000000, 0x000000, 0x000000, 0x5b9fe6, 0x569ce1, 0x579ce1,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x5069ea, 0x4f6cee, 0x516ced, 0x569be0, 0x87bffa, 0x559cde, 0x85c2fb, 0x000000, 0x000000, 0x559ee3, 0x85c0f8, 0x85bef5, 0x000000,
  0x000000, 0x000000, 0x000000, 0x576be8, 0x516cef, 0x4f6cee, 0x546de1, 0x506def, 0x536ceb, 0x536bf1, 0x536bf1, 0x000000, 0x000000, 0x589ce3, 0x569bde, 0x579be2, 0x579be2, 0x000000,
  0x000000, 0x7bc6fd, 0x80c0fd, 0x8bbcff, 0x516cef, 0x506def, 0x536ced, 0x506def, 0x4f6cee, 0x516ced, 0x506def, 0x556cee, 0x546bef, 0x87bff8, 0x87c0f5, 0x87c0f5, 0x000000, 0x000000,
  0x89c0f6, 0x89bef2, 0x32352e, 0x8bbcfe, 0x8bbdfc, 0x516cef, 0x506def, 0x5773f1, 0x506bee, 0x526df0, 0x4f6cee, 0x536bef, 0x506bee, 0x516eee, 0x569bde, 0x579ade, 0x000000, 0x000000,
  0x88c0f3, 0x86c1fb, 0x2b323a, 0x80c2ff, 0x83c0f6, 0x516ced, 0x4e6be3, 0x516cef, 0x4f6def, 0x516aeb, 0x526df2, 0x506def, 0x516eee, 0x516de8, 0x569ddf, 0x549edf, 0x000000, 0x000000,
  0x88c1f6, 0x84bff9, 0x85c1f7, 0x86befb, 0x84c0fc, 0x5370f0, 0x516cef, 0x4f6cee, 0x516cef, 0x516bf3, 0x4f6cee, 0x526af0, 0x516ef0, 0x84bff7, 0x88bff8, 0x85c0f8, 0x000000, 0x000000,
  0x000000, 0x84c0fc, 0x87bef7, 0x87c0f5, 0x586beb, 0x506bee, 0x506cf1, 0x516ced, 0x536bef, 0x516cef, 0x516feb, 0x579ce1, 0x000000, 0x569ae1, 0x579ce1, 0x539edf, 0x579cdf, 0x000000,
  0x000000, 0x000000, 0x000000, 0x556be5, 0x516cef, 0x506def, 0x516cef, 0x536bef, 0x000000, 0x000000, 0x579ce1, 0x83c0f9, 0x000000, 0x000000, 0x569ce4, 0x84c1fa, 0x84c1f8, 0x85c0f8,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x549de1, 0x85c0f8, 0x569ce2, 0x000000, 0x000000, 0x579cdd, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x569ae1, 0x85c0fa, 0x569ce4, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x92cbff, 0x73b0ef, 0x68ace9, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x539ee2, 0x85c2f8, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x579be2, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000
};


const uint8_t wu_weight(uint8_t a, uint8_t b) {
  return ((uint16_t) a * b + a + b) >> 8;
}


void rotzoom() {
  uint32_t ms = millis();

  // https://wikipedia.org/wiki/Rotation_matrix
  int16_t xdx = cos16(ms * 3);
  int16_t ydx = sin16(ms * 3);
  int16_t xdy = -sin16(ms * 3);
  int16_t ydy = cos16(ms * 3);
  xdx = ( cos(ms / 3000.f) * (sin(ms / 1900.f)/3.f + .66666f)) * 32767.f;
  xdy = ( sin(ms / 3000.f) * (sin(ms / 1900.f)/3.f + .66666f)) * 32767.f;
  ydx = (-sin(ms / 3000.f) * (sin(ms / 1900.f)/3.f + .66666f)) * 32767.f;
  ydy = ( cos(ms / 3000.f) * (sin(ms / 1900.f)/3.f + .66666f)) * 32767.f;

  // move the centre of rotation
  uint32_t xstart = -xdx * (kMatrixWidth + 1) / 2;
  uint32_t ystart = -ydx * (kMatrixHeight + 1) / 2;
  xstart -= xdy * (kMatrixWidth + 1) / 2;
  ystart -= ydy * (kMatrixHeight + 1) / 2;
  

  for (byte row = 0; row < kMatrixHeight; row++) {
    uint32_t x = xstart;
    uint32_t y = ystart;
    for (byte column = 0; column < kMatrixWidth; column++) {
      CRGB output = 0;
      if ((millis() / 2048) & 0) {
        // non super-sampled
        uint8_t xoffset = ((x >> 15) + (ms >> 5));
        uint8_t yoffset = y >> 15;

        // tile the SPR_SIZExSPR_SIZE sprite on a 32x32 grid
        xoffset %= TILE_SIZE;
        yoffset %= TILE_SIZE;
        // sample 1 pixel from the texture
        if (xoffset < SPR_SIZE && yoffset < SPR_SIZE)
          output =  pgm_read_dword (fish + xoffset + yoffset * SPR_SIZE);

      } else {
        // super-sampled (the values are 256x bigger, so the bottom 8-bits are the fractional part)
        uint16_t xoffset = (x >> 7) + (ms << 3);
        uint16_t yoffset = y >> 7;

        // extract the fractional parts and derive their inverses
        uint8_t xx = xoffset & 0xff, yy = yoffset & 0xff, ix = 255 - xx, iy = 255 - yy;

        // co-ordinates of the 4 texture samples
        uint8_t x0 = (xoffset >> 8) % TILE_SIZE;
        uint8_t y0 = (yoffset >> 8) % TILE_SIZE;
        uint8_t x1 = (x0 + 1) % TILE_SIZE;
        uint8_t y1 = (y0 + 1) % TILE_SIZE;

        // sample 4 texture pixels, scaling them by the Wu weight of the fractional offset
        // keep tiling the 18x18 texture lookups on a 32x32 grid
        CRGB sample;
        if (x0 < SPR_SIZE) {
          if (y0 < SPR_SIZE) {
            sample = pgm_read_dword_near(fish + x0 + y0 * SPR_SIZE);
            output.r = (sample.r * wu_weight(ix, iy)) >> 8;
            output.g = (sample.g * wu_weight(ix, iy)) >> 8;
            output.b = (sample.b * wu_weight(ix, iy)) >> 8;
          }
          if (y1 < SPR_SIZE) {
            sample = pgm_read_dword_near(fish + x0 + y1 * SPR_SIZE);
            output.r += (sample.r * wu_weight(ix, yy)) >> 8;
            output.g += (sample.g * wu_weight(ix, yy)) >> 8;
            output.b += (sample.b * wu_weight(ix, yy)) >> 8;
          }
        }
        if (x1 < SPR_SIZE) {
          if (y0 < SPR_SIZE) {
            sample = pgm_read_dword_near(fish + x1 + y0 * SPR_SIZE);
            output.r += (sample.r * wu_weight(xx, iy)) >> 8;
            output.g += (sample.g * wu_weight(xx, iy)) >> 8;
            output.b += (sample.b * wu_weight(xx, iy)) >> 8;
          }
          if (y1 < SPR_SIZE) {
            sample = pgm_read_dword_near(fish + x1 + y1 * SPR_SIZE);
            output.r += (sample.r * wu_weight(xx, yy)) >> 8;
            output.g += (sample.g * wu_weight(xx, yy)) >> 8;
            output.b += (sample.b * wu_weight(xx, yy)) >> 8;
          }
        }
      }
      rgb24 out = rgb24(output.r, output.g, output.b);
      backgroundLayer.drawPixel(column, row, out);
      x += xdx; y+= ydx;
    }
    xstart += xdy; ystart += ydy;
  }
}

/* sutaburosu - 2021-03-19
*/

#pragma once

#include "../config.h"
#include "images.h"

#define TILE_SIZE 32

    class RotZoomer : public Effect {
  public:
  RotZoomer(void) {
    name = (char*)"RotZoomer";
  }
  virtual ~RotZoomer(void) {};

  private:
  const uint8_t wu_weight(uint8_t a, uint8_t b) {
    return ((uint16_t)a * b + a + b) >> 8;
  };

  public:
  void drawFrame(void) {
    uint32_t ms = millis();

    // https://wikipedia.org/wiki/Rotation_matrix
    int32_t xdx = cos16(ms * 3);
    int32_t ydx = sin16(ms * 3);
    int32_t xdy = -sin16(ms * 3);
    int32_t ydy = cos16(ms * 3);
    xdx         = (cos(ms / 3000.f) * (sin(ms / 1900.f) / 2.5f + .7f)) * 65535.f;
    xdy         = (sin(ms / 3000.f) * (sin(ms / 1900.f) / 2.5f + .7f)) * 65535.f;
    ydx         = (-sin(ms / 3000.f) * (sin(ms / 1900.f) / 2.5f + .7f)) * 65535.f;
    ydy         = (cos(ms / 3000.f) * (sin(ms / 1900.f) / 2.5f + .7f)) * 65535.f;

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
          if (xoffset < FISH_SPR_SIZE && yoffset < FISH_SPR_SIZE)
            output = pgm_read_dword(fish + xoffset + yoffset * FISH_SPR_SIZE);

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
          if (x0 < FISH_SPR_SIZE) {
            if (y0 < FISH_SPR_SIZE) {
              sample   = pgm_read_dword_near(fish + x0 + y0 * FISH_SPR_SIZE);
              output.r = (sample.r * wu_weight(ix, iy)) >> 8;
              output.g = (sample.g * wu_weight(ix, iy)) >> 8;
              output.b = (sample.b * wu_weight(ix, iy)) >> 8;
            }
            if (y1 < FISH_SPR_SIZE) {
              sample = pgm_read_dword_near(fish + x0 + y1 * FISH_SPR_SIZE);
              output.r += (sample.r * wu_weight(ix, yy)) >> 8;
              output.g += (sample.g * wu_weight(ix, yy)) >> 8;
              output.b += (sample.b * wu_weight(ix, yy)) >> 8;
            }
          }
          if (x1 < FISH_SPR_SIZE) {
            if (y0 < FISH_SPR_SIZE) {
              sample = pgm_read_dword_near(fish + x1 + y0 * FISH_SPR_SIZE);
              output.r += (sample.r * wu_weight(xx, iy)) >> 8;
              output.g += (sample.g * wu_weight(xx, iy)) >> 8;
              output.b += (sample.b * wu_weight(xx, iy)) >> 8;
            }
            if (y1 < FISH_SPR_SIZE) {
              sample = pgm_read_dword_near(fish + x1 + y1 * FISH_SPR_SIZE);
              output.r += (sample.r * wu_weight(xx, yy)) >> 8;
              output.g += (sample.g * wu_weight(xx, yy)) >> 8;
              output.b += (sample.b * wu_weight(xx, yy)) >> 8;
            }
          }
        }
        rgb24 out = rgb24(output.r, output.g, output.b);
        backgroundLayer->drawPixel(column, row, out);
        x += xdx;
        y += ydx;
      }
      xstart += xdy;
      ystart += ydy;
    }
  };
};

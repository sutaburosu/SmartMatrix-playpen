/* sutaburosu - 2021

 original by ldirko - 2020
  https://www.reddit.com/r/FastLED/comments/hv77xm/my_metaballs_implementation_i_have_not_tried_it/
*/

#pragma once
#include "../config.h"

class MetaBalls : public Effect {
private:
  uint8_t* buffer;
  uint8_t* divide;
  CRGBPalette16* palette;

public:
  MetaBalls() {
    name    = (char*)F("MetaBalls");
    buffer  = (uint8_t*)malloc(kMatrixWidth * kMatrixHeight);
    divide  = (uint8_t*)malloc(256);
    palette = (CRGBPalette16*)malloc(3 * 16);
    if (!(buffer && divide && palette)) return;
    *palette = lava;
    for (uint16_t i = 0; i < 256; i++)
      divide[i] = 350 / (i + 1);
  }

  virtual ~MetaBalls() {
    if (buffer) free(buffer);
    if (divide) free(divide);
    if (palette) free(palette);
  }

  virtual void drawFrame() {
    if (!(buffer && divide && palette)) return;
    uint8_t bx1 = beatsin8(15, 0, kMatrixWidth - 1, 0, 0);
    uint8_t by1 = beatsin8(18, 0, kMatrixHeight - 1, 0, 0);
    uint8_t bx2 = beatsin8(28, 0, kMatrixWidth - 1, 0, 32);
    uint8_t by2 = beatsin8(23, 0, kMatrixHeight - 1, 0, 32);
    uint8_t bx3 = beatsin8(30, 0, kMatrixWidth - 1, 0, 64);
    uint8_t by3 = beatsin8(24, 0, kMatrixHeight - 1, 0, 64);
    uint8_t bx4 = beatsin8(17, 0, kMatrixWidth - 1, 0, 128);
    uint8_t by4 = beatsin8(25, 0, kMatrixHeight - 1, 0, 128);
    uint8_t bx5 = beatsin8(19, 0, kMatrixWidth - 1, 0, 170);
    uint8_t by5 = beatsin8(21, 0, kMatrixHeight - 1, 0, 170);

    if (0) {
      for (uint8_t i = 0; i < kMatrixWidth; i++) {
        for (uint8_t j = 0; j < kMatrixHeight; j++) {

          int16_t sum = dist2(i, j, bx1, by1);
          sum += dist2(i, j, bx2, by2);
          sum += dist2(i, j, bx3, by3);
          sum += dist2(i, j, bx4, by4);
          sum += dist2(i, j, bx5, by5);

          byte col                       = constrain(sum, 0, 240);
          crgbleds[i + j * kMatrixWidth] = ColorFromPalette(*palette, col, 255);
        }
      }
    } else {
      radial_fill(buffer, bx1 - kMatrixWidth / 1, by1 - kMatrixHeight / 1);
      radial_fill(buffer, bx2 - kMatrixWidth / 1, by2 - kMatrixHeight / 1);
      radial_fill(buffer, bx3 - kMatrixWidth / 1, by3 - kMatrixHeight / 1);
      radial_fill(buffer, bx4 - kMatrixWidth / 1, by4 - kMatrixHeight / 1);
      radial_fill(buffer, bx5 - kMatrixWidth / 1, by5 - kMatrixHeight / 1);
      uint8_t* src = buffer;
      CRGB* led    = crgbleds;
      for (uint8_t y = 0; y < kMatrixHeight; y++) {
        for (uint8_t x = 0; x < kMatrixWidth; x++) {
          *led++ = ColorFromPalette(*palette, *src, 255);
          *src++ = 0;
        }
      }
    }
  }

  byte dist4(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    byte dx = abs8(y2 - y1);
    byte dy = abs8(x2 - x1);
    if (dx < dy)
      return 200 / dy;
    else
      return 200 / dx;
  }

  byte dist3(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    byte a = y2 - y1;
    byte b = x2 - x1;
    a *= a;
    b *= b;
    byte dist = 200 / sqrt16(a + b);
    return dist;
  }

  byte dist2(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    byte a = y2 - y1;
    byte b = x2 - x1;
    a *= a;
    b *= b;
    return 200.0 / sqrt16(a + b + 1);
  }

  // visits each pixel in raster order whilst maintaining xroot = sqrt(x^2 + y^2)
  // using only a single sqrt() per frame rather than one per pixel.
  void radial_fill(uint8_t* dst, int8_t x_offset, int8_t y_offset) {
    uint8_t screenx, screeny, xroot, yroot;
    uint16_t xsumsquares, ysumsquares, xnextsquare, ynextsquare;
    int8_t x, y;

    // offset the origin in screen space
    x           = x_offset;
    y           = y_offset;
    ysumsquares = x_offset * x_offset + y * y;
    yroot       = sqrt16(ysumsquares);
    ynextsquare = yroot * yroot;

    // Quadrant II (top-left)
    screeny = kMatrixHeight;
    while (y < 0 && screeny) {
      x           = x_offset;
      screenx     = kMatrixWidth;
      xsumsquares = ysumsquares;
      xroot       = yroot;
      if (x < 0) {
        xnextsquare = xroot * xroot;
        while (x < 0 && screenx) {
          screenx--;
          *dst = qadd8(*dst, divide[xroot]);
          dst++;
          xsumsquares += 2 * x++ + 1;
          if (xsumsquares < xnextsquare)
            xnextsquare -= 2 * xroot-- - 1;
        }
      }
      // Quadrant I (top right)
      if (screenx) {
        xnextsquare = (xroot + 1) * (xroot + 1);
        while (screenx) {
          screenx--;
          *dst = qadd8(*dst, divide[xroot]);
          dst++;
          xsumsquares += 2 * x++ + 1;
          if (xsumsquares >= xnextsquare)
            xnextsquare += 2 * ++xroot + 1;
        }
      }
      ysumsquares += 2 * y++ + 1;
      if (ysumsquares < ynextsquare)
        ynextsquare -= 2 * yroot-- - 1;
      screeny--;
    }
    // Quadrant III (bottom left)
    ynextsquare = (yroot + 1) * (yroot + 1);
    while (screeny) {
      x           = x_offset;
      screenx     = kMatrixWidth;
      xsumsquares = ysumsquares;
      xroot       = yroot;
      if (x < 0) {
        xnextsquare = xroot * xroot;
        while (x < 0 && screenx) {
          screenx--;
          *dst = qadd8(*dst, divide[xroot]);
          dst++;
          xsumsquares += 2 * x++ + 1;
          if (xsumsquares < xnextsquare)
            xnextsquare -= 2 * xroot-- - 1;
        }
      }
      // Quadrant IV (bottom right)
      if (screenx) {
        xnextsquare = (xroot + 1) * (xroot + 1);
        while (screenx--) {
          *dst = qadd8(*dst, divide[xroot]);
          dst++;
          xsumsquares += 2 * x++ + 1;
          if (xsumsquares >= xnextsquare)
            xnextsquare += 2 * ++xroot + 1;
        }
      }
      ysumsquares += 2 * y++ + 1;
      if (ysumsquares >= ynextsquare)
        ynextsquare += 2 * ++yroot + 1;
      screeny--;
    }
  }
};
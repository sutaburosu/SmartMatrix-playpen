/* sutaburosu 2021-06-06 
  inspired by this animated GIF
  https://youtu.be/WrSl1rnEOdA?t=2186
*/

#pragma once
#include "../config.h"
#include "wuLineAA.h"

class WavyDots : public Effect {
  public:
  WavyDots() {
    name = (char*)"WavyDots";
  };
  virtual ~WavyDots(){};

  virtual void drawFrame() {
    backgroundLayer->fillScreen({0, 0, 0});
    float ms = millis() / 100.f;
    const uint16_t kHalfWidth = kMatrixWidth / 2;
    const uint16_t kHalfHeight = kMatrixHeight / 2;

    for (uint16_t y = 0; y < kHalfHeight; y += 1)
      for (uint16_t x = 0; x < kHalfWidth; x += 1) {
        float hyp = hypotf(x, y) * 3.f;
        float fx  = sin(ms + hyp / 8.f) * 192.f;
        float fy  = cos(ms*.8f + hyp / 8.f) * 192.f;
        CRGB col  = CHSV(hyp + ms, 255, 255);
        wu_pixel(256 * (x + kHalfWidth) + fx, 256 * (y + kHalfHeight) + fy, &col);
        if (x) wu_pixel(256 * (-x + kHalfWidth) + fx, 256 * (y + kHalfHeight) + fy, &col);
        if (y) wu_pixel(256 * (x + kHalfWidth) + fx, 256 * (-y + kHalfHeight) + fy, &col);
        if (x && y) wu_pixel(256 * (-x + kHalfWidth) + fx, 256 * (-y + kHalfHeight) + fy, &col);
      }
  }
};
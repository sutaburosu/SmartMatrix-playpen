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
    for (uint16_t y = 0; y < kMatrixHeight / 2; y += 1)
      for (uint16_t x = 0; x < kMatrixWidth / 2; x += 1) {
        float hyp = hypotf(x, y) * 3.f;
        float fx  = sin(ms + hyp / 8.f) * 192.f;
        float fy  = cos(ms*.8f + hyp / 8.f) * 192.f;
        CRGB col  = CHSV(hyp + ms, 255, 255);
        wu_pixel(256 * (x + kMatrixWidth / 2) + fx, 256 * (y + kMatrixHeight / 2) + fy, &col);
        if (x) wu_pixel(256 * (-x + kMatrixWidth / 2) + fx, 256 * (y + kMatrixHeight / 2) + fy, &col);
        if (y) wu_pixel(256 * (x + kMatrixWidth / 2) + fx, 256 * (-y + kMatrixHeight / 2) + fy, &col);
        if (x && y) wu_pixel(256 * (-x + kMatrixWidth / 2) + fx, 256 * (-y + kMatrixHeight / 2) + fy, &col);
      }
  }
};
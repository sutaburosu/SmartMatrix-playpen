#pragma once
#include "../config.h"
#include "wuLineAA.h"
#include <TimeLib.h>

class AnalogueClock : public Effect {
  private:
  byte* heat;

  public:
  AnalogueClock() {
    name = (char*)"AnalogueClock";
    heat = (byte*)malloc((kMatrixHeight + 1) * kMatrixWidth);
  };
  virtual ~AnalogueClock() {
    free(heat);
  };
#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  virtual void drawFrame() {
    analogueClock();
  }

  private:
  void analogueClock() {
    ledclock(hour(), minute(), second());
  }

  void Fire2012(uint8_t activity) {
    for (uint8_t x = 0; x < kMatrixWidth; x++) {
      // Cool down every cell a little
      for (uint8_t y = 0; y <= kMatrixHeight; y++)
        heat[y * kMatrixWidth + x] = qsub8(heat[y * kMatrixWidth + x], random8(33));

      // Heat from each cell drifts 'up' and diffuses a little
      uint8_t xleft  = x > 0 ? (x - 1) : 0;
      uint8_t xright = (x < kMatrixWidth - 1) ? (x + 1) : x;
      for (uint8_t y = 0; y < kMatrixHeight; y++)
        heat[y * kMatrixWidth + x] = (heat[y * kMatrixWidth + x] +
                                      heat[(y + 1) * kMatrixWidth + xleft] +
                                      heat[(y + 1) * kMatrixWidth + x] +
                                      heat[(y + 1) * kMatrixWidth + xright]) /
                                     4;

      // add some random noise at the bottom
      if (random8() < activity)
        heat[kMatrixHeight * kMatrixWidth + x] = qadd8(heat[kMatrixHeight * kMatrixWidth + x], random8(activity));
    }
  }

  void ledclock(uint8_t hour, uint8_t minute, uint8_t second) {
    uint16_t centrex    = kMatrixWidth * 128 - 128;
    uint16_t centrey    = kMatrixHeight * 128 - 128;
    uint16_t length     = kMatrixHeight * 128;
    uint16_t base_theta = 65536 * 3 / 4;

    // draw the clock hands with fire

    // second hand with sweep action
    uint16_t theta              = second * 65536 / 60;
    static uint16_t sweep_theta = theta;
    int32_t diff                = theta - sweep_theta;
    if (diff < 0)
      diff += 65536;
    sweep_theta += (diff + 175) / 350;
    wuVectorAA8(heat, centrex, centrey, length, base_theta + sweep_theta);

    // minute hand
    theta              = (theta + minute * 65536) / 60;
    uint16_t min_theta = theta;
    wuVectorAA8(heat, centrex, centrey, length * 13 / 16, base_theta + theta);

    // hour hand
    theta = (theta + (hour % 12) * 65536) / 12;
    wuVectorAA8(heat, centrex, centrey, length * 5 / 8, base_theta + theta);

    // advance the fire animation
    Fire2012(random(128));

    // and map the fire onto the LEDs
    for (int16_t i = kMatrixWidth * kMatrixHeight; i--;)
      crgbleds[i] = HeatColor(heat[i]);

    // then draw the hands in black
    CRGB col = 0x0;
    wuVectorAA(centrex, centrey, length, base_theta + sweep_theta, &col);
    wuVectorAA(centrex, centrey, length * 13 / 16, base_theta + min_theta, &col);
    wuVectorAA(centrex, centrey, length * 5 / 8, base_theta + theta, &col);
  }
#pragma GCC pop_options
};

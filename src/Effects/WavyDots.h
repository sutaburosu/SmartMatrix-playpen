/* sutaburosu 2021-06-06 
  inspired by this animated GIF
  https://youtu.be/WrSl1rnEOdA?t=2186
*/

#pragma once
#include "../config.h"
#include "../wuLineAA.h"

class WavyDots : public Effect {
public:
  WavyDots() {
    name = (char*)F("WavyDots");
  };
  virtual ~WavyDots(){};

#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  virtual void drawFrame() {
    backgroundLayer->fillScreen({0, 0, 0});
    float ms                = millis() / 300.f;
    const float kHalfWidth  = kMatrixWidth / 2;
    const float kHalfHeight = kMatrixHeight / 2;

    const float density = 1.0875f + 0.0675f * sinf(ms * 0.29f);
    for (float y = 0; y <= kHalfHeight + 2; y += density)
      for (float x = 0; x <= kHalfWidth + 2; x += density) {
        float hyp = hypotf(x, y) * 3.f;
        float fx  = sin(ms + hyp / 8.f) * 192.f;
        float fy  = cos(ms * .8f + hyp / 8.f) * 192.f;
        // CRGB col  = CHSV(hyp + ms, 255, 255);
        CRGB col = ColorFromPaletteExtended(RainbowHalfStripeColors_p, (ms * 6. - hyp) * 256.f, 255, LINEARBLEND);
        wu_pixel(256 * (x + kHalfWidth) + fx, 256 * (y + kHalfHeight) + fy, &col);
        if (x) wu_pixel(256 * (-x + kHalfWidth) + fx, 256 * (y + kHalfHeight) + fy, &col);
        if (y) wu_pixel(256 * (x + kHalfWidth) + fx, 256 * (-y + kHalfHeight) + fy, &col);
        if (x && y) wu_pixel(256 * (-x + kHalfWidth) + fx, 256 * (-y + kHalfHeight) + fy, &col);
      }
  }
#pragma GCC pop_options
};
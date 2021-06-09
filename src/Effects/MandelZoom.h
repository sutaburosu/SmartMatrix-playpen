/* sutaburosu - 2021-06-09
  derived from code by atuline a.k.a. Johnny5Canuck 
  which was derived from someone else's code */

#pragma once
#include "../config.h"

typedef struct {
  float real;
  float imag;
  float zoom;
  uint16_t maxiters;
} MbrotView;

MbrotView MbrotViews[] = {
    {-0.813118939, 0.203263184, 850, 750},
    {-0.374054956, 0.659940459, 150, 256},
    {-0.94299, 0.3162, 120, 256},
    {-1.342282444, 0.0, 120, 200},
    {-1.999985881, 0.0, 130, 200},
    {-1.768778832, -0.001738995, 200, 400},
    {-0.071875677, -0.649981301, 100, 400},
    {-0.743517833, -0.127094578, 100, 400},
    {-0.374004139, 0.659792175, 75, 160},
    {-0.776592852, -0.13664085, 150, 400},
    {-0.226266647, 1.11617444, 200, 300},
    {-1.940157342, -0.0000008, 200, 300},
};

class MandelZoom : public Effect {
public:
  MandelZoom() {
    name           = (char*)F("MandelZoom");
    currentPalette = RainbowStripeColors_p;
    for (uint8_t i = 0; i < 16; i++) {
      targetPalette[i] = currentPalette[i];
    }
  };
  virtual ~MandelZoom(){};

private:
  CRGBPalette16 currentPalette;
  CRGBPalette16 targetPalette;
  uint16_t view_now       = 0;
  uint16_t view_next      = 1;
  const uint16_t view_max = sizeof(MbrotViews) / sizeof(MbrotViews[0]);
  uint16_t status         = 0;

  float maxzoom       = 1000.;
  int iters_high_cap  = 768;
  int iters_low_cap   = 192;
  float maxIterations = 0;

  // Calculated start coordinates and per-pixel deltas
  float real, imag, xmin, ymin;
  float xdx, xdy, ydx, ydy;

public:
  virtual void drawFrame() {

    resize(); // Define the window to display.
    mandel(); // Calculate and display that window.

    EVERY_N_SECONDS(60) {
      view_now  = view_next;
      view_next = (view_next + 1) % view_max;
    }
    maxzoom -= (maxzoom - MbrotViews[view_now].zoom) / 100.f;
    real -= (real - MbrotViews[view_now].real) / 100.f;
    imag -= (imag - MbrotViews[view_now].imag) / 100.f;
    maxIterations -= (maxIterations - MbrotViews[view_now].maxiters) / 100.f;

    // Let's get some real nice palettes and palette transitioning happening.
    EVERY_N_MILLISECONDS(50) {
      uint8_t maxChanges = 4;
      nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges); // AWESOME palette blending capability.
    }

    EVERY_N_SECONDS(5) { // Change the target palette to a random one every 5 seconds.
      uint8_t i = 2 * (random8() % 8);
      //   // for (uint8_t i = 0; i < 16; i++) {
      targetPalette[i] = CHSV(random8(), random8(128, 255), random8(192, 255));
      //   // targetPalette[i] = CRGB(random8(), random8(), random8());
      //   // }
    }
  }

private:
  void resize() {
    float zoom = maxzoom * cosf(millis() / 3000.f) + maxzoom + 0.2;

    // wobble slowly around the point of interest
    // real += sin(millis() / 1500.f) / zoom;
    // imag += sin(millis() / 1400.f) / zoom;

    // scale the aspect ratio of the default set by the zoom
    float dx = 3.5 / zoom;
    float dy = 2.0 / zoom;

    // rotate slowly
    xdx = dx * cos(millis() / 3000.f);
    ydx = dx * sin(millis() / 3000.f);
    xdy = dy * -sin(millis() / 3000.f);
    ydy = dy * cos(millis() / 3000.f);

    // rotate around the centre of the matrix
    xmin = real - xdx / 2.f - xdy / 2.f;
    ymin = imag - ydx / 2.f - ydy / 2.f;

    // scale the deltas to be per-pixel
    xdx /= kMatrixWidth;
    ydx /= kMatrixWidth;
    xdy /= kMatrixHeight;
    ydy /= kMatrixHeight;

    // increase iterations at higher zoom levels
    maxIterations = max(min(zoom, iters_high_cap), iters_low_cap);
    // Serial.print(maxIterations);
    // Serial.print(" ");
  }

#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  void mandel() { // Calculate and display the Mandelbrot set for the current window.
    uint16_t hue_rot = millis() * 3;
    CRGB* leds = crgbleds;
    // Start y
    float ystart = ymin;
    float xstart = xmin;
    for (int j = 0; j < kMatrixHeight; j++) {
      // Start x
      float x0 = xstart;
      float y0 = ystart;
      for (int i = 0; i < kMatrixWidth; i++) {
        int iter = 0;

        // quickly rule out many of the
        // inside period-2 bulb?
        float r = (x0 + 1) * (x0 + 1) + y0 * y0;
        if (r <= 0.0625f)
          iter = maxIterations; // +r * 255. / 0.0625f;
        else {
          // inside main cardioid?
          float q = (x0 - 0.25f) * (x0 - 0.25f) + y0 * y0;
          if (q * (q + (x0 - 0.25f)) <= 0.25f * y0 * y0)
            iter = maxIterations; //  + q * 64. / 0.25f;
        }

        float x2   = 0;
        float y2   = 0;
        float w    = 0;
        float xold = 0;
        float yold = 0;

        // Now we test, as we iterate z = z^2 + c does z tend towards infinity?
        while (x2 + y2 < 4 && iter < maxIterations) {
          float x = x2 - y2 + x0;
          float y = w - x2 - y2 + y0;
          x2      = x * x;
          y2      = y * y;
          w       = (x + y) * (x + y);
          iter++;

          // detect periodicity (generally slower on Teensy4)
          if (0) {
            if (fabsf(x2 - xold) < 0.001f && fabsf(y2 - yold) < 0.001f) {
              iter = maxIterations; // + iter;
              break;
            }
            if ((iter & 0x1f) == 0) {
              xold = x2;
              yold = y2;
            }
          }
        }

        // We color each pixel based on how long it takes to get to infinity, or black if it never gets there.
        if (iter >= maxIterations) {
          // if (iter > maxIterations) {
          //   uint16_t overflow = 255 - min(iter - maxIterations, 255);
          //   *leds++           = CRGB(overflow, overflow, overflow);
          // } else
          *leds++ = CRGB::Black; // within the set.
        } else {
          *leds++ = ColorFromPaletteExtended(currentPalette, iter * 256 + hue_rot, 255, LINEARBLEND);
        }
        x0 += xdx;
        y0 += ydx;
      }
      xstart += xdy;
      ystart += ydy;
    }
    // blur2d(crgbleds, kMatrixWidth, kMatrixHeight, 16); // Optionally smooth out the rough edges a bit.

  } // mandel()
#pragma GCC pop_options
};

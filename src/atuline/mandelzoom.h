#pragma once
#include "../config.h"

class MandelZoom : public Effect {
  public:
  MandelZoom() {
    name = (char*)"MandelZoom";
    currentPalette = RainbowColors_p;
  };
  virtual ~MandelZoom(){};

  private:
  CRGBPalette16 currentPalette;
  CRGBPalette16 targetPalette;

  // These initial values show the whole thing on the matrix.
  float reAl = -.5; // Relatively close to https://mandel.gart.nz/#/
  float imAg = 0.2;
  float zoOm = 452.;

  // Calculated start/stop coordinates.
  float xmin, ymin, xmax, ymax;
  float dx; // Delta x is mapped to the matrix size.
  float dy; // Delta y is mapped to the matrix size.

  int maxIterations = 64;   // How many iterations per pixel before we give up.
  float maxCalc     = 16.0; // How big is each calculation allowed to be before we give up.

#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  public:
  virtual void drawFrame() {

    resize(); // Define the window to display.
    mandel(); // Calculate and display that window.

    // Let's get some real nice palettes and palette transitioning happening.
    EVERY_N_MILLISECONDS(20) {
      uint8_t maxChanges = 16;
      nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges); // AWESOME palette blending capability.
    }

    EVERY_N_SECONDS(1) { // Change the target palette to a random one every 5 seconds.
      // uint8_t i = random8() % 16;
      for (uint8_t i = 0; i < 16; i++) {
        // targetPalette[i] = CHSV(random8(), 255, random8(224, 255));
        targetPalette[i] = CRGB(random8(), random8(), random8());
      }
    }
  }

  private:
  void resize() { // Resize the minimum and maximum values on the fly to display.

    // Let's animate our Mandelbrot set.
    zoOm = 9000. * fabsf(sin(millis() / 2000.f)) + 200.f; // Change zoom to 200 up to 5200
    reAl = -0.94299;                                      // Use the location as shown at https://www.youtube.com/watch?v=V2dlac4WSic
    imAg = 0.3162;                                        // His code is https://github.com/zranger1/PixelblazePatterns/blob/master/mandelbrot2D.js
                                                          // And I'm not sure how he's getting the animation he does though.

    reAl = reAl + sin(millis() / 500.f) * 500.f / zoOm;
    imAg = imAg + sin(millis() / 400.f) * 300.f / zoOm;

    // These adjustments make the pan and zoom somewhat compatible with https://mandel.gart.nz/#/
    const float zoomAdj = .0016; // Adjust zoom factor.
    const float xadj    = 0.;    // Same for x and y values.
    const float yadj    = .001;

    xmin = reAl - 1. / zoOm / zoomAdj + xadj; // Scale the windows in accordance with the zoom factor.
    xmax = reAl + 1. / zoOm / zoomAdj + xadj;
    ymin = imAg - 1. / zoOm / zoomAdj + yadj;
    ymax = imAg + 1. / zoOm / zoomAdj + yadj;

    dx = (xmax - xmin) / (kMatrixWidth);  // Scale the delta x values to our matrix.
    dy = (ymax - ymin) / (kMatrixHeight); // Scale the delta y values to our matrix.

  } // resize()

  void mandel() { // Calculate and display the Mandelbrot set for the current window.
    static uint16_t hue_rot = 0;
    hue_rot += 128;
    CRGB* leds = (CRGB*)rgb24leds;
    // Start y
    float y = ymin;
    for (int j = 0; j < kMatrixHeight; j++) {

      // Start x
      float x = xmin;
      for (int i = 0; i < kMatrixWidth; i++) {

        // Now we test, as we iterate z = z^2 + c does z tend towards infinity?
        float a  = x;
        float b  = y;
        int iter = 0;

        while (iter < maxIterations) {
          float aa  = a * a;
          float bb  = b * b;
          float len = aa + bb;
          if (len > maxCalc) { // |z| = sqrt(a^2+b^2) OR z^2 = a^2+b^2
            break;             // Bail
          }

          // This operation corresponds to z -> z^2+c where z=a+ib c=(x,y). Remember to use 'foil'.
          b = 2 * a * b + y;
          a = aa - bb + x;
          iter++;
        } // while

        // We color each pixel based on how long it takes to get to infinity, or black if it never gets there.
        if (iter == maxIterations) {
          *leds++ = CRGB::Black; // Calculation kept on going, so it was within the set.
        } else {
          *leds++ = ColorFromPaletteExtended(currentPalette, iter * 65535.0 / maxIterations + hue_rot, 255 - iter / 16, LINEARBLEND);
        }
        x += dx;
      }
      y += dy;
    }

    //   blur2d( leds, kMatrixWidth, kMatrixHeight, 64);           // Optionally smooth out the rough edges a bit.

  } // mandel()

#pragma GCC pop_options
};

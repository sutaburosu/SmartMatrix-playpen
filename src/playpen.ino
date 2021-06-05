#include "config.h"
#include "atuline/mandelzoom.h"
#include "exoticorn/exoticorn_tunnel.h"
#include "FMS_Cat/quadtree.h"
#include "Incubus/stereo_tartan.h"
#include "jimLee/life.h"
#include "sutaburosu/analogueClock.h"
#include "sutaburosu/rotzoom.h"


SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, 0);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(realBackgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, 0);
SMLayerBackground<RGB_TYPE(COLOR_DEPTH), 0> backgroundLayer = realBackgroundLayer;

CRGB *crgbleds;
rgb24 *rgb24leds;


void setup() {
  Serial.begin(115200);
  matrix.addLayer(&backgroundLayer);
  matrix.begin();
  // matrix.setRefreshRate(300);
  backgroundLayer.setBrightness(128);
}


void loop() {
  static uint32_t effect = 0;

  static uint32_t last_change_ms;
  // if ((last_change_ms++ % 1024) == 0) last_effect = -effect;
  if (millis() - last_change_ms > 10000) {
    effect = (effect + 1) % 9;
    last_change_ms = millis();
  }

  const uint32_t us_points = 3;
  static uint64_t us_timing[us_points];
  static uint64_t us_total[us_points];
  static uint32_t us_frame = 0;
  static uint64_t fpslast_us = us_timing[0];
  static uint32_t last_effect = effect;

  us_timing[0] = micros();
  backgroundLayer.swapBuffers(true);
  rgb24leds = backgroundLayer.backBuffer();
  crgbleds = (CRGB *) rgb24leds;

  us_timing[1] = micros();
  switch (effect) {
    case 0: life(); break;
    case 1: rotzoom(); break;
    case 2: exoticorn_tunnel1(); break;
    case 3: exoticorn_tunnel2(); break;
    case 4: exoticorn_tunnel3(); break;
    case 5: FMS_Cat_quadtree(); break;
    case 6: stereo_tartan(); break;
    case 7: analogueClock(); break;
    case 8: mandelzoom(); break;
    default: break;
  }
  us_timing[2] = micros();

  {
    // check for unexpected delays. On TD1.53 this sketch triggers at 31.5 minutes,
    // and every 71.6 minutes after that. On TD1.54b9 it has never triggered.
    // https://forum.pjrc.com/threads/66525-Teensy-4-1-Freeze-(-1786ms) 
    static uint32_t last_ms = 0;
    static uint32_t delays = 0;
    static uint64_t total_frames = 0;
    uint32_t this_ms = millis();
    if ((this_ms - last_ms > 100) && (total_frames > 0)) {
      delays++;
      Serial.println();
      Serial.print("!!!! DELAY !!!!  @");
      Serial.print(this_ms);
      Serial.print(" ms   Delay: ");
      Serial.print(this_ms - last_ms);
      Serial.print(" ms  count: ");
      Serial.println(delays);
    }
    last_ms = this_ms;
    total_frames++;
  }

  // return;
  // print frames per second
  ++us_frame;
  for (uint8_t us_loop = 1; us_loop < us_points; us_loop++) {
    us_total[us_loop] += us_timing[us_loop] - us_timing[us_loop - 1];
  }
  if (effect != last_effect) {
    Serial.printf("effect:%2d %5d ", last_effect, us_frame);
    for (uint8_t us_loop = 1; us_loop < us_points; us_loop++) {
      Serial.printf("%6.2fms  ", us_total[us_loop] / 1000.0 / us_frame);
      us_total[us_loop] = 0;
    }
    Serial.printf("%6.2f FPS @%dHz\r\n", us_frame * 1000000.0 / (micros() - fpslast_us), matrix.getRefreshRate());
    fpslast_us = micros();
    us_frame = 0;
    last_effect = effect;
  }
}

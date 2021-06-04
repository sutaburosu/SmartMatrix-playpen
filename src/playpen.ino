#include "config.h"
#include "jimLee/life.h"
#include "exoticorn/exoticorn_tunnel.h"
#include "FMS_Cat/quadtree.h"
#include "Incubus/stereo_tartan.h"
#include "sutaburosu/analogueClock.h"
#include "sutaburosu/rotzoom.h"


SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, 0);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(realBackgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, 0);
SMLayerBackground<RGB_TYPE(COLOR_DEPTH), 0> backgroundLayer = realBackgroundLayer;

CRGB *crgbleds;
rgb24 *rgb24leds;


void setup() {
  Serial.begin(115200);
  // while (!Serial) {};
  matrix.addLayer(&backgroundLayer);
  matrix.begin();
  // matrix.setRefreshRate(300);
  backgroundLayer.setBrightness(128);
  randomFill(); // start Life 
}


void loop() {
  backgroundLayer.swapBuffers(true);
  rgb24leds = backgroundLayer.backBuffer();
  crgbleds = (CRGB *) rgb24leds;
  static uint32_t effect = 5;
  static uint32_t last_change_ms;

  // if (((millis() / 64) % 64) == 0) effect = random(8);
  if (millis() - last_change_ms > 100000) {
    effect = (effect + 1) % 8;
    last_change_ms = millis();
  }

  uint64_t t1 = micros();
  switch (effect) {
    case 0: life(); break;
    case 1: rotzoom(); break;
    case 2: exoticorn_tunnel1(); break;
    case 3: exoticorn_tunnel2(); break;
    case 4: exoticorn_tunnel3(); break;
    case 5: FMS_Cat_quadtree(); break;
    case 6: stereo_tartan(); break;
    case 7: analogueClock(); break;
    default: break;
  }
  uint64_t t2 = micros();

  // check for unexpected delays
  static uint32_t last_ms = 0;
  static uint32_t delays = 0;
  static uint32_t frames = 0;
  uint32_t this_ms = millis();
  if ((this_ms - last_ms > 100) && (frames > 0)) {
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
  frames++;

  // return;
  // print frames per second
  uint16_t sample_frames = 128;
  static uint16_t fpsframe;
  static uint64_t fpslast_ms;
  if (++fpsframe == sample_frames) {
    fpsframe = 0;
    Serial.print((t2 - t1) / 1000.0);
    Serial.print(" ");
    Serial.print(sample_frames * 1000000.0 / (micros() - fpslast_ms));
    Serial.print(" FPS  refresh: ");
    Serial.println(matrix.getRefreshRate());
    fpslast_ms = micros();
  }
}

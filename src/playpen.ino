#include "config.h"
#include "life.h"
#include "exoticorn_tunnel.h"
#include "FMS_Cat/quadtree.h"
#include "stereo_tartan.h"
#include "sutaburosu/analogueClock.h"
#include "sutaburosu/rotzoom.h"


SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, 0);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(realBackgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, 0);
SMLayerBackground<RGB_TYPE(COLOR_DEPTH), 0> backgroundLayer = realBackgroundLayer;

// https://github.com/nesbox/TIC-80/wiki/palette
rgb24 sweetie16[] = {
  rgb24(0x1a, 0x1c, 0x2c), rgb24(0x5d, 0x27, 0x5d), rgb24(0xb1, 0x3e, 0x53), rgb24(0xef, 0x7d, 0x57),
  rgb24(0xff, 0xcd, 0x75), rgb24(0xa7, 0xf0, 0x70), rgb24(0x38, 0xb7, 0x64), rgb24(0x25, 0x71, 0x79),
  rgb24(0x29, 0x36, 0x6f), rgb24(0x3b, 0x5d, 0xc9), rgb24(0x41, 0xa6, 0xf6), rgb24(0x73, 0xef, 0xf7),
  rgb24(0xf4, 0xf4, 0xf4), rgb24(0x94, 0xb0, 0xc2), rgb24(0x56, 0x6c, 0x86), rgb24(0x33, 0x3c, 0x57)
};

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
  rgb24 *led = backgroundLayer.backBuffer();
  rgb24leds = led;
  crgbleds = (CRGB *) led;
  static uint32_t effect = 7;
  static uint32_t last_change_ms;

  // if (((millis() / 64) % 64) == 0) effect = random(8);
  if (millis() - last_change_ms > 10000) {
    effect = (effect + 1) % 8;
    last_change_ms = millis();
  }

  uint64_t t1 = micros();
  switch (effect) {
    case 0: life(); break;
    case 1: rotzoom(); break;
    case 2: exoticorn_tunnel1(led); break;
    case 3: exoticorn_tunnel2(led); break;
    case 4: exoticorn_tunnel3(led); break;
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

#include "config.h"
#include "life.h"
#include "rotzoom.h"


SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, 0);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(realBackgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, 0);
SMLayerBackground<RGB_TYPE(COLOR_DEPTH), 0> backgroundLayer = realBackgroundLayer;


uint16_t XY (uint8_t x, uint8_t y) {
  return (y * kMatrixWidth + x);
}

void setup() {
  Serial.begin(115200);
  matrix.addLayer(&backgroundLayer);
  matrix.begin();
  // matrix.setRefreshRate(300);
  backgroundLayer.setBrightness(128);
  randomFill(); // start Life 
}



void loop() {
  backgroundLayer.swapBuffers(true);
  static uint32_t effect = 0;

  uint64_t t1 = micros();
  switch (effect) {
    case 0: life(); break;
    case 1: rotzoom(); break;
    default: break;
  }
  uint64_t t2 = micros();

  // check for unexpected delays
  static uint32_t last_ms = 0;
  static uint32_t delays = 0;
  static uint32_t frames = 0;
  uint32_t this_ms = millis();
  if ((this_ms - last_ms > 20) && (frames > 0)) {
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

  return;
  // print frames per second
  uint16_t sample_frames = 256;
  static uint16_t fpsframe;
  static uint64_t fpslast_ms;
  if (++fpsframe == sample_frames) {
    fpsframe = 0;
    Serial.print((t2 - t1) / 1000.0);
    Serial.print(" ");
    Serial.print(sample_frames * 1000000.0 / (micros() - fpslast_ms));
    Serial.println(" FPS");
    fpslast_ms = micros();
  }
}

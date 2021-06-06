#include "config.h"
#include "Effects/AllEffects.h"

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, 0);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(realBackgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, 0);
SMLayerBackground<RGB_TYPE(COLOR_DEPTH), 0>* backgroundLayer = &realBackgroundLayer;

CRGB* crgbleds;
rgb24* rgb24leds;

extern unsigned long _heap_start;
extern unsigned long _heap_end;
extern char* __brkval;
uint32_t freeram_lowest = -1;
uint32_t freeram() {
  uint32_t freenow = (char*)&_heap_end - __brkval;
  if (freenow < freeram_lowest) {
    freeram_lowest = freenow;
  }
  return freenow;
}

// Teensy3 and 4 clock
time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void clockSetup() {
  static bool setup = true;
  if (setup) {
    setSyncProvider(getTeensy3Time);
    setup = false;
  }
}

void setup() {
  Serial.begin(115200);
  clockSetup();
  matrix.addLayer(backgroundLayer);
  // matrix.setRefreshRate(300);
  backgroundLayer->setBrightness(128);
  matrix.begin();
  // delay(900);
  // Serial.println("boot");
}

static Effect* activeEffect;
void playEffect(uint16_t new_effect_nr) {
  static uint16_t effect_nr = -1;
  if (new_effect_nr != effect_nr) {
    delete (activeEffect);
    effect_nr    = new_effect_nr;
    activeEffect = newEffect(static_cast<EffectType>(new_effect_nr));
  }
  if (activeEffect)
    activeEffect->drawFrame();
}

void loop() {
  static EffectType effect = wavyDots;
  const uint8_t us_points  = 3;
  static uint64_t us_timing[us_points];
  static uint64_t us_total[us_points];
  static uint32_t us_frames      = 0;
  us_timing[0] = micros();
  static uint64_t us_last_report = us_timing[0];

  backgroundLayer->swapBuffers(true);
  rgb24leds = backgroundLayer->backBuffer();
  crgbleds  = (CRGB*)rgb24leds;

  us_timing[1] = micros();
  playEffect(effect);
  us_timing[2] = micros();
  freeram();

  // sum the execution times for each frame
  ++us_frames;
  for (uint8_t us_loop = 1; us_loop < us_points; us_loop++) {
    us_total[us_loop] += us_timing[us_loop] - us_timing[us_loop - 1];
  }

  EVERY_N_SECONDS(30) {
    // print timing report
    Serial.printf("%2u:%-20s ", effect, activeEffect->name);
    for (uint8_t us_loop = 1; us_loop < us_points; us_loop++) {
      Serial.printf("%6.2fms  ", us_total[us_loop] / 1000.0 / us_frames);
      us_total[us_loop] = 0;
    }
    Serial.printf("%6.2f FPS @%dHz freeram:%d (min %d)\r\n",
                  us_frames * 1000000.0 / (micros() - us_last_report),
                  matrix.getRefreshRate(), freeram(), freeram_lowest);
    us_last_report = micros();
    us_frames      = 0;

    // move to next Effect, and loop at the length of the list
    uint16_t inc = (effect + 1) % static_cast<EffectType>(_random_effect);
    effect       = static_cast<EffectType>(inc);
  }
}

#include "config.h"
#include "atuline/mandelzoom.h"
#include "exoticorn/exoticorn_tunnel.h"
#include "FMS_Cat/quadtree.h"
#include "Incubus/stereo_tartan.h"
#include "jimLee/life.h"
#include "sutaburosu/analogueClock.h"
#include "sutaburosu/EffectWavyDots.h"
#include "sutaburosu/EffectMetaBalls.h"
#include "sutaburosu/rotzoom.h"

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
    Serial.printf("free:%d\r\n", freenow);
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
  matrix.begin();
  // matrix.setRefreshRate(300);
  backgroundLayer->setBrightness(128);
  delay(900);
  Serial.println("boot");
}

#define MAX_EFFECTS 10
void newEffect(uint16_t n, Effect** e) {
  switch (n) {
    default:
    case 0: *e = new (Life); break;
    case 1: *e = new (RotZoomer); break;
    case 2: *e = new (ExoticornTunnel1); break;
    case 3: *e = new (ExoticornTunnel2); break;
    case 4: *e = new (ExoticornTunnel3); break;
    case 5: *e = new (FMS_Cat_quadtree); break;
    case 6: *e = new (StereoTartan); break;
    case 7: *e = new (AnalogueClock); break;
    case 8: *e = new (MandelZoom); break;
    case 9: *e = new (WavyDots); break;
    case 10: *e = new (MetaBalls); break;
  }
  Effect* c = *e;
  Serial.printf("%-20s", c->name);
}

void playEffect(uint16_t new_effect_nr) {
  static Effect* active_effect;
  static uint16_t effect_nr = -1;
  if (new_effect_nr != effect_nr) {
    if (active_effect) {
      delete (active_effect);
      active_effect = 0;
    }
    effect_nr = new_effect_nr;
    newEffect(effect_nr, &active_effect);
  }
  if (active_effect)
    active_effect->drawFrame();
}

void check_delays() {
  // check for unexpected delays. On TD1.53 this sketch triggers at 31.5 minutes,
  // and every 71.6 minutes after that. On TD1.54b9 it has never triggered.
  // https://forum.pjrc.com/threads/66525-Teensy-4-1-Freeze-(-1786ms)
  static uint32_t delay_last_ms = 0;
  static uint32_t delay_count   = 0;
  static uint64_t delay_frames  = 0;
  uint32_t delay_this_ms        = millis();
  if ((delay_this_ms - delay_last_ms > 100) && (delay_frames > 0)) {
    delay_count++;
    Serial.println();
    Serial.print("!!!! DELAY !!!!  @");
    Serial.print(delay_this_ms);
    Serial.print(" ms   Delay: ");
    Serial.print(delay_this_ms - delay_last_ms);
    Serial.print(" ms  count: ");
    Serial.println(delay_count);
  }
  delay_last_ms = delay_this_ms;
  delay_frames++;
}

void loop() {
  check_delays();
  static uint16_t effect = 10;
  static uint32_t last_change_ms;
  if (millis() - last_change_ms > 30000) {
    effect         = (effect + 1) % MAX_EFFECTS;
    last_change_ms = millis();
  }

  const uint8_t us_points = 3;
  static uint64_t us_timing[us_points];
  us_timing[0] = micros();
  backgroundLayer->swapBuffers(true);
  rgb24leds    = backgroundLayer->backBuffer();
  crgbleds     = (CRGB*)rgb24leds;
  us_timing[1] = micros();

  playEffect(effect);
  freeram();
  us_timing[2] = micros();

  // return;
  // print frames per second
  static uint64_t us_total[us_points];
  static uint32_t us_frame    = 0;
  static uint64_t fpslast_us  = us_timing[0];
  static uint32_t last_effect = effect;
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
    Serial.printf("%6.2f FPS @%dHz freeram:%d (min %d)\r\n",
                  us_frame * 1000000.0 / (micros() - fpslast_us),
                  matrix.getRefreshRate(), freeram(), freeram_lowest);
    fpslast_us  = micros();
    us_frame    = 0;
    last_effect = effect;
  }
}

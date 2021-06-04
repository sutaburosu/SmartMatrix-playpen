#pragma once

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#define SM_INTERNAL
//#define SMARTMATRIX_USE_PSRAM
#include "MatrixHardware_Teensy4_ShieldV5_RBG.h"
#include <SmartMatrix.h>
#include "sutaburosu/images.h"
// #include "gems.h"

#define X_RES 128
#define Y_RES 64

#define COLOR_DEPTH 24                    // Choose the color depth used for storing pixels in the layers: 24 or 48 (24 is good for most sketches - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24)
const uint16_t kMatrixWidth = X_RES;      // Set to the width of your display, must be a multiple of 8
const uint16_t kMatrixHeight = Y_RES;     // Set to the height of your display
const uint8_t kRefreshDepth = 36;         // Tradeoff of color quality vs refresh rate, max brightness, and RAM usage.  36 is typically good, drop down to 24 if you need to.  On Teensy, multiples of 3, up to 48: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48.  On ESP32: 24, 36, 48
const uint8_t kDmaBufferRows = 4;         // known working: 2-4, use 2 to save RAM, more to keep from dropping frames and automatically lowering refresh rate.  (This isn't used on ESP32, leave as default)
const uint8_t kPanelType = SM_PANELTYPE_HUB75_64ROW_MOD32SCAN;  // Choose the configuration that matches your panels.  See more details in MatrixCommonHub75.h and the docs: https://github.com/pixelmatix/SmartMatrix/wiki

extern SMLayerBackground<rgb24, 0> backgroundLayer;
extern CRGB *crgbleds;
extern rgb24 *rgb24leds;
extern rgb24 sweetie16[16];
#pragma once

// silence the version numbers
#define FASTLED_INTERNAL
#define SM_INTERNAL
//#define SMARTMATRIX_USE_PSRAM

#include <Arduino.h>
#include <FastLED.h>
#include "MatrixHardware_Teensy4_ShieldV5_RBG.h"
#include <SmartMatrix.h>
#include "Effect.h"
#include "FastLED_additions.h"
#include "palettes.h"

#define COLOR_DEPTH 24                                             // Choose the color depth used for storing pixels in the layers: 24 or 48 (24 is good for most sketches - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24)
const uint16_t kMatrixWidth  = 128;                                // Set to the width of your display, must be a multiple of 8
const uint16_t kMatrixHeight = 64;                                 // Set to the height of your display
const uint8_t kRefreshDepth  = 36;                                 // Tradeoff of color quality vs refresh rate, max brightness, and RAM usage.  36 is typically good, drop down to 24 if you need to.  On Teensy, multiples of 3, up to 48: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48.  On ESP32: 24, 36, 48
const uint8_t kDmaBufferRows = 4;                                  // known working: 2-4, use 2 to save RAM, more to keep from dropping frames and automatically lowering refresh rate.  (This isn't used on ESP32, leave as default)
const uint8_t kPanelType     = SM_PANELTYPE_HUB75_64ROW_MOD32SCAN; // Choose the configuration that matches your panels.  See more details in MatrixCommonHub75.h and the docs: https://github.com/pixelmatix/SmartMatrix/wiki

extern SMLayerBackground<rgb24, 0>* backgroundLayer;
extern CRGB* crgbleds;

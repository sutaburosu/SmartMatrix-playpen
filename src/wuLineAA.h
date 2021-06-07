#pragma once
#include <FastLED.h>

extern void crossfade(CRGB* a, const CRGB* b, uint8_t amount);
extern void crossfade8(byte* a, uint8_t amount);
extern void wuLineAA8(uint8_t* fb, saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2);
extern void wuLineAA(saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2, CRGB* col);
extern void wuVectorAA(const int16_t x, const int16_t y, const uint16_t length, const uint16_t theta, CRGB* col);
extern void wuVectorAA8(uint8_t* fb, const int16_t x, const int16_t y, const uint16_t length, const uint16_t theta);
extern void wu_pixel(int32_t x, int32_t y, CRGB* col);
extern void wu_pixel1d(int32_t y, CRGB* col);
extern int32_t XY(int16_t x, int16_t y);
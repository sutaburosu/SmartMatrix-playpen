#pragma once
#include <FastLED.h>

extern void crossfade(CRGB *a, const CRGB *b, uint8_t amount);
extern void crossfade8(byte *a, uint8_t amount);
extern void wuLineAA8(uint8_t* fb, saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2);
extern void wuLineAA(saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2, CRGB * col);
extern void wuVectorAA(const uint16_t x, const uint16_t y, const uint16_t length, const uint16_t theta, CRGB *col);
extern void wuVectorAA8(uint8_t *fb, const uint16_t x, const uint16_t y, const uint16_t length, const uint16_t theta);
extern void wu_pixel(uint32_t x, uint32_t y, CRGB* col);
extern void wu_pixel1d(uint32_t y, CRGB* col);
extern uint32_t XY(uint16_t x, uint16_t y);
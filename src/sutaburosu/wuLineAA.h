#pragma once

void crossfade(CRGB *a, const CRGB *b, uint8_t amount);
void crossfade8(byte *a, uint8_t amount);
void wuLineAA8(saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2);
void wuLineAA(saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2, CRGB * col);
void wuVectorAA(const uint16_t x, const uint16_t y, const uint16_t length, const uint16_t theta, CRGB *col);
void wuVectorAA8(const uint16_t x, const uint16_t y, const uint16_t length, const uint16_t theta);

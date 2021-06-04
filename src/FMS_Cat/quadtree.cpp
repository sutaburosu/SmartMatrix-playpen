/* https://tic80.com/play?cart=1868 */

#include "../config.h"

uint32_t quadtree_rand = 0;

uint32_t xorshift32(uint32_t state) {
  state ^= state << 13;
  state ^= state >> 17;
  state ^= state << 5;
  return state;
}


uint32_t quadrand(uint32_t max) {
  quadtree_rand = xorshift32(quadtree_rand);
  return quadtree_rand % max;
}

uint32_t quadrand(uint32_t min, uint32_t max) {
  quadtree_rand = xorshift32(quadtree_rand);
  uint32_t diff = max - min;
  return min + (quadtree_rand % diff);
}

void quadtree(uint32_t x, uint32_t y, uint32_t s, uint32_t t) {
  for (uint32_t i = 0; i < 4; i++) {
    uint32_t u = x + (i%2) * s;
    uint32_t v = y + (i/2) * s;
    if ((uint32_t) quadrand(2, 16) < s) {
      quadtree(u, v, s/2, t);
    } else {
      uint32_t rectx = ((u + t*(((v/16)%3)-3)) % 512) - 99;
      uint32_t recty = ((v+t) % 512) - 99;
      uint32_t rectw = s - 1 + rectx;
      uint32_t recth = s - 1 + recty;
      uint32_t rectc = ((((v-t)/8)%9) + quadrand(2) - 5);
      if (quadrand(2) == 1) {
        backgroundLayer.fillRectangle(rectx, recty, rectw, recth, rgb24(0, 0, 0), sweetie16[rectc]);
      } else {
        backgroundLayer.fillRectangle(rectx, recty, rectw, recth, sweetie16[rectc], rgb24(0, 0, 0));
      }
    }
  }
}

void FMS_Cat_quadtree() {
  static uint32_t t = 0;
  quadtree_rand = 31415926;
  quadtree(0, 0, kMatrixWidth * 2, t);
  t++;
}
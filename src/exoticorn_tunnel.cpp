#include "config.h"

const uint16_t kHalfWidth = kMatrixWidth / 2;
const uint16_t kHalfHeight = kMatrixHeight / 2;

#pragma GCC push_options
#pragma GCC optimize "-Ofast"
void exoticorn_tunnel1(rgb24 *led) {
  byte c;
  float t, z, q, w;
  uint8_t path_width = 139;
  float tunnel_bore = 40.0f;
  float floor_offset = 25.0f;
  float hill_steepness = 50.f;
  float hill_height = 99.f;
    
  t = (millis() % (9 * 2 * 512)) / 512.f;
  for (int16_t y = -kHalfHeight-kHalfHeight/9; y < kHalfHeight-kHalfHeight/9; y++) {
    for (int16_t x = -kHalfWidth; x <= 0; x++) {
      z = tunnel_bore / sqrt(x*x + y*y) + t;
      q = (fmod(z, 9) < 6) ? z - fmod(z, 9) + 6 : z;
      w = floor_offset / y + t;
      if (y > 0 && w < q)
        // floor
        c = (powf(x*(w-t), 2) < path_width ? 14 : 6) + fmod(w, 2);
      else
        if (-y*(q-t) < hill_height / (powf(x*(q-t) / hill_steepness, 2)+1))
          // face of hill, and interior tunnel walls
          c = (q==z) ? fmod(z, 2) : 3;
        else
          // sky
          c = 9 - y/12 + ((0b111101001000 >> (-y%12)) & 1);
      *led++ = sweetie16[c];
    }
    rgb24 *src = led-1;
    for (uint16_t x = 1; x < kHalfWidth; x++)
      *led++ = *--src;
  }
}
#pragma GCC pop_options

/* exoticorn - 2021-05-15

  The 3 original Lua/TIC-80 effects were live-coded in
  25 minutes each by exoticorn during a 256-byte battle
  at outlinedemoparty.nl

   https://tic80.com/play?cart=1871 
   https://tic80.com/play?cart=1872
   https://tic80.com/play?cart=1873
*/

#pragma once
#include "../config.h"

const uint16_t kHalfWidth  = kMatrixWidth / 2;
const uint16_t kHalfHeight = kMatrixHeight / 2;

class ExoticornTunnel1 : public Effect {
  public:
  ExoticornTunnel1(){
    name = (char*)"exoticorn_tunnel1";
  };
  virtual ~ExoticornTunnel1(){};
#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  virtual void drawFrame() {
    rgb24* led = rgb24leds;
    float o, q, w, r, z, s, t = millis() / 2000.f;
    o = sinf(t) * .8f;
    for (int16_t y = -kHalfHeight; y < kHalfHeight; y++) {
      for (int16_t x = -kHalfWidth; x < kHalfWidth; x++) {
        q      = x - 0.1f;
        w      = y - kMatrixHeight / 8;
        r      = sqrtf(q * q + w * w);
        z      = q / r;
        s      = z * o + sqrtf(z * z * o * o + 1 - o * o);
        q      = z * s - o + t / 9.f;
        w      = w / r * s + t / 9.f;
        s      = s * 20.f / r;
        z      = (int(q / .1f) ^ (int(w / .1f) & int((s + t) / .05f))) & 5;
        *led++ = sweetie16[int(z * max(0, 2.f - s))];
      }
    }
  }
#pragma GCC pop_options
};

class ExoticornTunnel2 : public Effect {
  public:
  ExoticornTunnel2(){
    name = (char*)"exoticorn_tunnel2";
  };
  virtual ~ExoticornTunnel2(){};

  private:
  const float n(float x) {
    return (sin(x) + sin(x * .7f)) / 2.f;
  }

  public:
#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  virtual void drawFrame() {
    backgroundLayer->fillScreen({0, 0, 0});
    float t = millis() / 10.f;
    for (float i = 500; i >= 10; i--) {
      float p = i + floor(t);
      float z = i - t + int(t);
      float f = 4000.f / z;
      float r = 1.f + (int(p) % 4) / 4.f;
      float x = n(p / 50.f) - n(t / 50.f) + sin(p + 0.1f) * r;
      float y = n(p / 50.f + 9.f) - n(t / 50.f + 9.1f) + sin(p + 11.f) * r;
      byte c  = max(0, 5 - z / 100.f);
      backgroundLayer->fillCircle(kHalfWidth + x * f, kHalfHeight + y * f,
                                  f / 15.f * powf(r, 4.f),
                                  sweetie16[(c ^ ((int(p) % 4) * 8)) & 0xf]);
    }
  }
#pragma GCC pop_options
};

class ExoticornTunnel3 : public Effect {
  public:
  ExoticornTunnel3(){
    name = (char*)"exoticorn_tunnel3";
  };
  virtual ~ExoticornTunnel3(){};
#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  virtual void drawFrame() {
    rgb24* led = rgb24leds;
    byte c;
    float t, z, q, w;
    uint8_t path_width   = 139;
    float tunnel_bore    = 40.0f;
    float floor_offset   = 25.0f;
    float hill_steepness = 50.f;
    float hill_height    = 99.f;

    t = (millis() % (9 * 2 * 512)) / 512.f;
    for (int16_t y = -kHalfHeight - kHalfHeight / 9; y < kHalfHeight - kHalfHeight / 9; y++) {
      for (int16_t x = -kHalfWidth; x <= 0; x++) {
        z = tunnel_bore / sqrtf(x * x + y * y) + t;
        q = (fmodf(z, 9) < 6) ? z - fmodf(z, 9) + 6 : z;
        w = floor_offset / y + t;
        if (y > 0 && w < q)
          // floor
          c = (powf(x * (w - t), 2) < path_width ? 14 : 6) + fmodf(w, 2);
        else if (-y * (q - t) < hill_height / (powf(x * (q - t) / hill_steepness, 2) + 1))
          // face of hill, and interior tunnel walls
          c = (q == z) ? fmodf(z, 2) : 3;
        else
          // sky
          c = 9 - y / 12 + ((0b111101001000 >> (-y % 12)) & 1);
        *led++ = sweetie16[c];
      }
      rgb24* src = led - 1;
      for (uint16_t x = 1; x < kHalfWidth; x++)
        *led++ = *--src;
    }
  }
#pragma GCC pop_options
};
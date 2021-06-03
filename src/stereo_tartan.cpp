#include "config.h"

/*
-- title:  Stereo Tartan
-- author: Incubus / Incognita
-- desc:   Stereographic projection of a rotating sphere, in 255 characters of lua code. Released at Shadow Party 2021.
-- script: lua
f=0
m=math
p=m.pi/48
h=m.floor
l=m.atan
n=68
o=120
function TIC()f=f+.1
d=m.sin(f)e=m.cos(f)for t=-n,n do
for s=-o,o do
k=(s*s+t*t)/48+1
x=s/k
y=t/k
z=64/k-1
q=d*x+e*y
u=e*x-d*y
v=d*q-e*z
pix(o+s,n+t,h(l(e*q+d*z,m.sqrt(u*u+v*v))/p)~h(l(u,v)/p))end
end
end
*/

#pragma GCC push_options
#pragma GCC optimize "-Ofast"
void stereo_tartan() {
  float scale = 16.f;
  float p = 3.14159265358f / scale;
  uint16_t o = 64;
  uint16_t n = 32;
  float f = millis() / 1000.f;
  float d = sin(f);
  float e = cos(f);
  for (int16_t t = -n; t < n; t++) {
    for (int16_t s = -o; s < o; s++) {
      float k = (s*s + t*t) / scale + .3f;
      float x = s / k;
      float y = t / k;
      float z = (2.f * scale) / k - 1;
      float q = d * x + e * y;
      float u = e * x - d * y;
      float v = d * q - e * z;
      backgroundLayer.drawPixel(o+s, n+t, sweetie16[
        (int(floor(atan2f(e*q + d*z, sqrtf(u*u + v*v))/p)) ^ int(floor(atan2f(u, v)/p))) & 0xf
      ]);
    }
  }
}
#pragma GCC pop_options

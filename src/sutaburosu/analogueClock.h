#pragma once
#include "../config.h"

extern byte *heat1d;
void analogueClock();

class AnalogueClock : public Effect {
  public:
  AnalogueClock() {
    name = (char*)"AnalogueClock";
  };
  virtual ~AnalogueClock(){};
#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  virtual void drawFrame() {
    analogueClock();
  }
#pragma GCC pop_options
};

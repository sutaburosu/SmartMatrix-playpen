#pragma once

extern void mandelzoom();

class MandelZoom : public Effect {
  public:
     MandelZoom() {
       name = (char*)"MandelZoom";
     };
     virtual ~MandelZoom(){};
#pragma GCC push_options
#pragma GCC optimize "-Ofast"
  virtual void drawFrame() {
    mandelzoom();
  }
#pragma GCC pop_options
};

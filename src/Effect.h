#pragma once

class Effect {
  public:
  char* name;
  Effect(void){};
  virtual ~Effect(void){};
  virtual void drawFrame() {
  };
};

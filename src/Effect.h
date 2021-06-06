#pragma once

class Effect {
  public:
  char* name;
  Effect(void){};
  virtual ~Effect(void){};
  virtual void drawFrame() {
    // backgroundLayer->fillScreen({0, 0, 0});
  };
};

// class RotZoomer : public Effect {
//   private:
//   byte* fb;

//   public:
//   RotZoomer() {
//     name = (char*)"RotZoomer";
//     fb   = (byte*)malloc(kMatrixWidth * kMatrixHeight);
//     // Serial.print("Crz");
//   }

//   virtual ~RotZoomer() {
//     free(fb);
//     // Serial.print("Drz");
//   }

//   virtual void drawFrame() {
//     backgroundLayer->fillCircle(64, 32, 30, {0, 128, 0});
//   }
// };
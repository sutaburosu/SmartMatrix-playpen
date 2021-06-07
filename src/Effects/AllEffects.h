#pragma once

#include "../Effect.h"
#include "AnalogueClock.h"
#include "ExoticornTunnels.h"
#include "Life.h"
#include "MandelZoom.h"
#include "MetaBalls.h"
#include "QuadTree.h"
#include "RotZoomer.h"
#include "StereoTartan.h"
#include "WavyDots.h"

enum EffectType : uint16_t {
  life,
  rotZoomer,
  exoticornTunnel1,
  exoticornTunnel2,
  exoticornTunnel3,
  quadTree,
  stereoTartan,
  analogueClock,
  mandelZoom,
  wavyDots,
  metaBalls,
  _random_effect // keep this last
};

Effect* newEffect(EffectType type) {
  Effect* c;
  switch (type) {
    default:
    case _random_effect:
      type = static_cast<EffectType>(random(static_cast<EffectType>(_random_effect)));
      return newEffect(type);
      break;
    case life: c = new (Life); break;
    case rotZoomer: c = new (RotZoomer); break;
    case exoticornTunnel1: c = new (ExoticornTunnel1); break;
    case exoticornTunnel2: c = new (ExoticornTunnel2); break;
    case exoticornTunnel3: c = new (ExoticornTunnel3); break;
    case quadTree: c = new (QuadTree); break;
    case stereoTartan: c = new (StereoTartan); break;
    case analogueClock: c = new (AnalogueClock); break;
    case mandelZoom: c = new (MandelZoom); break;
    case wavyDots: c = new (WavyDots); break;
    case metaBalls: c = new (MetaBalls); break;
  }
  return c;
}

// compile time construction only
template <typename T>
Effect* newEffect() {
  Effect* c = new T;
  // Serial.printf("%-20s", c->name);
  return c;
}

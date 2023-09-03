/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef FX_STRIP_DEF
#define FX_STRIP_DEF
#include <Arduino.h>
#include "Config.h"
#include "SideFX.h"
#include "FxParticle.h"

enum FxPaletteType
{
  Smoothed       = 0,
  Literal        = 1,
  Literal2       = 2,
  Literal3       = 3,
  Literal4       = 4,
};

enum FxTransitionType
{
  Transition_Instant         = 0,
  Transition_TimedFade       = 1,
  Transition_TimedWipePos    = 2,
  Transition_TimedWipeNeg    = 3,
  Transition_TimedWipeOutIn  = 4,
  Transition_TimedWipeInOut  = 5,
  Transition_TimedWipeRandom = 6,
  Transition_TimedFadeSin    = 7,
  Transition_TimedFadeCos    = 8,
};

class FxStrip
{
public:  
  int numleds = 0;
  unsigned char brightness = BRIGHTNESS;
  FxTransitionType transitionType = Transition_TimedFade;
  FxPaletteType paletteType = FxPaletteType::Smoothed;
  uint32_t *palette;
  uint32_t *nextPalette;
  uint32_t *initialPalette;
  int paletteId = 0;
  int *sequence;
  int paletteSpeed = 0;
  int paletteDirection = 1;
  int paletteIndex = 0;

  FxParticle particles[NUM_PARTICLES];
  FxChannelSystem fxSystem;
public:
  FxStrip(int nl);
  void SetTransitionType(FxTransitionType t);
  void SetPaletteType(FxPaletteType pt);
  void SetParticlesLoc(int loc);
  void SetParticlesRunning(bool state);
  void SetParticlesLength(int len);
  void SetParticlesColor(uint32_t rgb);
  void SetParticlesDirection(int dir);//-1 or 1
  void SetParticleMode(FxParticleMode mode);
  void RandomizeParticles();
  void PrintTransitionName();
};

#endif

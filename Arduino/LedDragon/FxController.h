/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef FX_CONTROLLER_DEF
#define FX_CONTROLLER_DEF
#include "Config.h"
#include "FxEvent.h"
#include "FxStrip.h"

enum FxState
{
  FxState_Default          = 0,
  FxState_TestPattern      = 1,
  FxState_MultiTestPattern = 2,
  FxState_SideFX           = 3
};

enum FxStripMask
{
  LEDS_0 = 1,   RIGHT_CHEST_A = LEDS_0,
  LEDS_1 = 2,   LEFT_CHEST_A = LEDS_1,
  LEDS_2 = 4,   RIGHT_CHEST_B = LEDS_2,
  LEDS_3 = 8,   LEFT_CHEST_B = LEDS_3,
  LEDS_4 = 16,  RIGHT_ARM = LEDS_4,
  LEDS_5 = 32,  LEFT_ARM = LEDS_5,
  LEDS_6 = 64,  RIGHT_LEG = LEDS_6,
  LEDS_7 = 128, LEFT_LEG = LEDS_7
};

struct FxController
{
public:  
  unsigned char brightness = BRIGHTNESS;
  float transitionMux = 0;
  FxState fxState = FxState_Default;
  unsigned long lastTimeLedUpdate = 0;  
  unsigned char stripMask = 255;
  FxStrip *strip[NUM_STRIPS];
  int select = 0;
  float vol = 0.5f;
  public:
    FxController();
    void Toggle(int channel);
    void ToggleOn(int channel);
    void ToggleOff(int channel);
    void Excite(int channel);
    void Reset(int channel);
    void KillFX();
    void SetPaletteType(FxPaletteType paletteType);
    void SetTransitionType(FxTransitionType transitionType);
    void SetParticlesLoc(int loc);
    void SetParticlesRunning(bool isRunning);
    void SetParticlesColor(uint32_t rgb);
    void SetParticlesDirection(int dir);
    void SetParticlesLength(int len);
    void SetParticleMode(FxParticleMode mode);  
    void PrintStateName();
    void PrintStatus();
    void SetPaletteSpeed(int v);
    void ResetPaletteSpeed();
    void ResetPaletteLocation();
    void ChangePaletteSpeed(int ps);
    void SetPaletteDirection(int c);
};

#endif

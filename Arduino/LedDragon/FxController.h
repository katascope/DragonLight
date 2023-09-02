/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef FX_CONTROLLER_DEF
#define FX_CONTROLLER_DEF
#include "FxCore.h"
#include "FxStrip.h"

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
  bool IsAnimating()
  {
    for (int i=0;i<NUM_STRIPS;i++)
        if (strip[i]->paletteSpeed > 0)
          return true;
    if (HasRunning())
      return true;
    
    return false;
  }
  FxController()
  {
    for (int i=0;i<NUM_STRIPS;i++)
    {
      strip[i] = NULL;
      switch (i)
      {
        case 0: strip[i] = new FxStrip(NUM_LEDS_0); break;
#if ENABLE_MULTISTRIP        
        case 1: strip[i] = new FxStrip(NUM_LEDS_1); break;
        case 2: strip[i] = new FxStrip(NUM_LEDS_2); break;
        case 3: strip[i] = new FxStrip(NUM_LEDS_3); break;
        case 4: strip[i] = new FxStrip(NUM_LEDS_4); break;
        case 5: strip[i] = new FxStrip(NUM_LEDS_5); break;
        case 6: strip[i] = new FxStrip(NUM_LEDS_6); break;
        case 7: strip[i] = new FxStrip(NUM_LEDS_7); break;
#endif        
      }
    }
  }
  void Toggle(int channel)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->fxSystem.Toggle(channel);
  }
  void ToggleOn(int channel)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->fxSystem.ToggleOn(channel);
  }
  void ToggleOff(int channel)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->fxSystem.ToggleOff(channel);
  }
  void Excite(int channel)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->fxSystem.Excite(channel);
  }
  void Reset(int channel)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->fxSystem.Reset(channel);
  }
  void KillFX()
  {
    for (int s=0;s<NUM_STRIPS;s++)
    {
      strip[s]->fxSystem.KillFX();
      for (int i=0;i<strip[s]->numleds;i++)
      {
        strip[s]->palette[i] = LEDRGB(0,0,0);
        strip[s]->nextPalette[i] = LEDRGB(0,0,0);
        strip[s]->initialPalette[i] = LEDRGB(0,0,0);
        strip[s]->sideFXPalette[i] = LEDRGB(0,0,0);
      }
    }
  }
  bool HasRunning()
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        if (strip[s]->HasRunning())
          return true;
    return false;
  }
  void SetPaletteType(FxPaletteType paletteType)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->SetPaletteType(paletteType);
  }
  void SetTransitionType(FxTransitionType transitionType)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->SetTransitionType(transitionType);
  }
  void SetParticlesLoc(int loc)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))
        strip[s]->SetParticlesLoc(loc);
  }
  void SetParticlesRunning(bool isRunning)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->SetParticlesRunning(isRunning);
  }  
  void SetParticlesColor(uint32_t rgb)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->SetParticlesColor(rgb);
  }  
  void SetParticlesDirection(int dir)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->SetParticlesDirection(dir);
  }  
  void SetParticlesLength(int len)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->SetParticlesLength(len);
  }
  void SetParticleMode(FxParticleMode mode)
  {
    for (int s=0;s<NUM_STRIPS;s++)
      if (stripMask & (1<<s))   
        strip[s]->SetParticleMode(mode);
  }
  
};

#endif

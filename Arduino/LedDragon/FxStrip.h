/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef FX_STRIP_DEF
#define FX_STRIP_DEF
#include "SideFX.h"
#include "FxParticle.h"

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
  uint32_t *sideFXPalette;
  int paletteId = 0;
  unsigned int *sequence;
  int paletteSpeed = 0;
  int paletteDirection = 1;
  int paletteIndex = 0;

  FxParticle particles[NUM_PARTICLES];
  FxChannelSystem fxSystem;
public:
  FxStrip(int nl)
  { 
    numleds = nl;
    palette = (uint32_t *)malloc(sizeof(uint32_t) * numleds);
    nextPalette = (uint32_t *)malloc(sizeof(uint32_t) * numleds);
    initialPalette = (uint32_t *)malloc(sizeof(uint32_t) * numleds);
    sideFXPalette = (uint32_t *)malloc(sizeof(uint32_t) * numleds);
    sequence = (unsigned int *)malloc(sizeof(sequence) * numleds);
    for (int i=0;i<numleds;i++)
    {
      palette[i] = LEDRGB(0,0,0);
      nextPalette[i] = LEDRGB(0,0,0);
      initialPalette[i] = LEDRGB(0,0,0);
      sideFXPalette[i] = LEDRGB(0,0,0);
      sequence[i] = i;
    }
  } 
  bool HasRunning()
  {
    for (int i=0;i<NUM_PARTICLES;i++)
      if (particles[i].on) 
        return true;
    return false;
  }
  void SetTransitionType(FxTransitionType tt)
  {
    transitionType = tt;
  }
  void SetPaletteType(FxPaletteType pt)
  {
    paletteType = pt;
  }
  void SetParticlesLoc(int loc)
  {
    for (int i=0;i<NUM_PARTICLES;i++)
      particles[i].loc = loc;
  }
  void SetParticlesRunning(bool state)
  {
    for (int i=0;i<NUM_PARTICLES;i++)
      particles[i].on = state;
  }
  void SetParticlesLength(int len)
  {
    for (int i=0;i<NUM_PARTICLES;i++)
      particles[i].len = len;
  }
  void SetParticlesColor(uint32_t rgb)
  {
    for (int i=0;i<NUM_PARTICLES;i++)
      particles[i].rgb = rgb;
  }
  void SetParticlesDirection(int dir)//-1 or 1
  {
    Serial.println(F("RandomDir"));
    RandomizeParticles();
    for (int i=0;i<NUM_PARTICLES;i++)
      particles[i].loc = dir * particles[i].loc;
  }
  void SetParticleMode(FxParticleMode mode)
  {
    Serial.println(F("RandomMode"));
    for (int i=0;i<NUM_PARTICLES;i++)
      particles[i].mode = mode;
    RandomizeParticles();
  }
  void RandomizeParticles()
  {
    Serial.println(F("RandomParticles"));
    for (int i=0;i<NUM_PARTICLES;i++)
    {
      particles[i].loc = rand() % (numleds-1);
      if (particles[i].mode == FX_PARTICLEMODE_RND)
         particles[i].loc = rand() % 10;
    }
  }
};

#endif

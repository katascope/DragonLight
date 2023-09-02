/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef FX_DEF
#define FX_DEF
#include <Arduino.h>
#include "Config.h"
#include "FxCore.h"

#define NUM_PARTICLES 4

enum FxParticleMode
{
  FX_PARTICLEMODE_STAR,
  FX_PARTICLEMODE_RND,
};

class FxParticle
{
public:  
  float on = false;
  FxParticleMode mode = FX_PARTICLEMODE_STAR;
  float loc = 0;
  float vel = 0;
  unsigned int len = 2;
  uint32_t rgb = 0xFFFFFFFF;
  FxParticle()
  {
      loc = 0;
      vel = 1.5f + (float)(rand()%40) / 40.0f;
//      vel = 0.5f + (float)(rand()%40) / 40.0f;
  }
};

class FxChannel
{
public:  
  bool on = false;
  int state = 0;
};

#define NUM_FX_CHANNELS 100

class FxChannelSystem
{
public:
  FxChannel channels[NUM_FX_CHANNELS];
  void KillFX()
  {
    for (int channel=0;channel<NUM_FX_CHANNELS;channel++)
    {
      channels[channel].on = false;
      channels[channel].state = 0;
    }    
  }
  void Toggle(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    channels[channel].on = !channels[channel].on;      
  }
  void ToggleOn(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    channels[channel].on = true;
  }
  void ToggleOff(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    channels[channel].on = false;
  }
  void Excite(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    if (channel == 2)
    {
     channels[channel].state++;
     if (channels[channel].state > 2)
      channels[channel].state = 0;
    }
    if (channel == 4)    
    {
     channels[channel].state++;
     if (channels[channel].state > 2)
      channels[channel].state = 0;
    }
    if (channel == 5)
    {
     channels[channel].state++;
     if (channels[channel].state > 1)
      channels[channel].state = 0;
    }
    if (channel == 7)    
    {
     channels[channel].state++;
     if (channels[channel].state > 1)
      channels[channel].state = 0;
    }
  }
  void Reset(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    channels[channel].state = 0;
  }
};

class FxStrip
{
public:  
  int numleds = 0;
  unsigned char brightness = BRIGHTNESS;
  FxTransitionType transitionType = Transition_TimedFade;
  FxPaletteType paletteType = FxPaletteType::Smoothed;
  FxPaletteUpdateType paletteUpdateType = FxPaletteUpdateType::None;
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

void FxAnimatePalette(struct FxController &fxc, bool useSideFXPalette);
void FxUpdatePalette(struct FxController &fxc);
void FxInstantEvent(FxController &fxc, int event);
void FxProcessParticles(FxController &fxc);
void FxEventProcess(FxController &fxc,int event);
void FxDisplaySideFX(FxController &fxc);
void FxPrintStatus(FxController &fxc);
void FxPrintPalette(FxController &fxc);
void FxPaletteById(FxController &fxc, int paletteId);
void FxActivateSideFXTrack(FxController &fxc, int trackId);

#endif

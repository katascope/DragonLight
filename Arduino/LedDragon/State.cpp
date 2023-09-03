/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "DevNeo.h"
#include "State.h"
#include "FxEvent.h"
#include "FxPalette.h"
#include "FxController.h"
#include "SideFX.h"

void State_Poll_TestPattern(FxController &fxc)
{
#if ENABLE_NEOPIXEL
    FxEvent(fxc, fx_strip_all);
    FxEvent(fxc, fx_transition_fast);
    for (int strip=0;strip<NUM_STRIPS;strip++)
    {
      fxc.strip[strip]->paletteSpeed = 0;
    }

    FxEvent(fxc, fx_strip_all);//fx_strip + LEDS_0);
    FxEvent(fxc, fx_palette_rainbow);
#if !ENABLE_MULTISTRIP
    FxEvent(fxc, fx_palette_half);
#endif    
    neopixelSetPalette(0, fxc.strip[0]->numleds, fxc.strip[0]->palette, fxc.strip[0]->paletteIndex);
    neopixelSetPixel(0, 0, CRGB_WHITE);
    
#if ENABLE_MULTISTRIP    
    FxEvent(fxc, fx_strip + LEDS_1);
    FxEvent(fxc, fx_palette_red);
    neopixelSetPalette(1, fxc.strip[1]->numleds, fxc.strip[1]->palette, fxc.strip[1]->paletteIndex);
    
    FxEvent(fxc, fx_strip + LEDS_2);
    FxEvent(fxc, fx_palette_yellow);
    neopixelSetPalette(2, fxc.strip[2]->numleds, fxc.strip[2]->palette, fxc.strip[2]->paletteIndex);
    
    FxEvent(fxc, fx_strip + LEDS_3);
    FxEvent(fxc, fx_palette_green);
    neopixelSetPalette(3, fxc.strip[3]->numleds, fxc.strip[3]->palette, fxc.strip[3]->paletteIndex);
    
    FxEvent(fxc, fx_strip + LEDS_4);
    FxEvent(fxc, fx_palette_cyan);
    neopixelSetPalette(4, fxc.strip[4]->numleds, fxc.strip[4]->palette, fxc.strip[4]->paletteIndex);
    
    FxEvent(fxc, fx_strip + LEDS_5);
    FxEvent(fxc, fx_palette_blue);
    neopixelSetPalette(5, fxc.strip[5]->numleds, fxc.strip[5]->palette, fxc.strip[5]->paletteIndex);
    
    FxEvent(fxc, fx_strip + LEDS_6);
    FxEvent(fxc, fx_palette_magenta);
    neopixelSetPalette(6, fxc.strip[6]->numleds, fxc.strip[6]->palette, fxc.strip[6]->paletteIndex);
    
    FxEvent(fxc, fx_strip + LEDS_7);
    FxEvent(fxc, fx_palette_orange);
    neopixelSetPalette(7, fxc.strip[7]->numleds, fxc.strip[7]->palette, fxc.strip[7]->paletteIndex);
#endif    
    FxEvent(fxc, fx_strip_all);

    for (int strip=0;strip<NUM_STRIPS;strip++)
    {
        fxc.strip[strip]->palette[0] = CRGB_ORANGE;    
        fxc.strip[strip]->initialPalette[0] = CRGB_ORANGE;
        fxc.strip[strip]->nextPalette[0] = CRGB_ORANGE;
        int nl = fxc.strip[strip]->numleds;
        fxc.strip[strip]->palette[nl-1] = CRGB_ORANGE;    
        fxc.strip[strip]->initialPalette[nl-1] = CRGB_ORANGE;
        fxc.strip[strip]->nextPalette[nl-1] = CRGB_ORANGE;
    }
#endif    
}

void CopyFromNext(FxController &fxc, int strip, int first, int last)
{
  if (last >= fxc.strip[strip]->numleds)
   last = fxc.strip[strip]->numleds;
  if (first < 0)
   first = 0;
  
  if (first < last)
    for (int i = first;i<last; i++)
      fxc.strip[strip]->palette[i] = fxc.strip[strip]->nextPalette[i];
  else
    for (int i = first;i>=last;i--)
      fxc.strip[strip]->palette[i] = fxc.strip[strip]->nextPalette[i];
}

void CopyFromPrev(FxController &fxc, int strip, int first, int last)
{
  if (last >= fxc.strip[strip]->numleds)
   last = fxc.strip[strip]->numleds;
  if (first < 0)
   first = 0;
  
  if (first < last)
    for (int i = first;i<last; i++)
      fxc.strip[strip]->palette[i] = fxc.strip[strip]->initialPalette[i];
  else
    for (int i = first;i>=last;i--)
      fxc.strip[strip]->palette[i] = fxc.strip[strip]->initialPalette[i];
}

void Do_Transition(FxController &fxc)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    int numleds = fxc.strip[strip]->numleds;
    if (fxc.stripMask & (1<<strip))
    {    
      if (fxc.strip[strip]->transitionType == Transition_TimedFade)
      {
        for (int i = 0; i < numleds; i++)
          fxc.strip[strip]->palette[i] = LerpRGB(fxc.transitionMux,fxc.strip[strip]->initialPalette[i],fxc.strip[strip]->nextPalette[i]);
      }
      if (fxc.strip[strip]->transitionType == Transition_TimedWipePos)
      {
        int limit = fxc.transitionMux * (numleds -1);
        CopyFromNext(fxc, strip, numleds - 1, numleds -1 - limit);      
      }
      if (fxc.strip[strip]->transitionType == Transition_TimedWipeNeg)
      {
        int limit = fxc.transitionMux * (numleds -1);
        CopyFromNext(fxc,strip, 0,limit);
      }
      if (fxc.strip[strip]->transitionType == Transition_TimedWipeOutIn)
      {
        int limit = fxc.transitionMux * (numleds/2);
        CopyFromNext(fxc,strip, 0,limit);
        CopyFromNext(fxc,strip, numleds-1,numleds-limit);
      }
      if (fxc.strip[strip]->transitionType == Transition_TimedWipeInOut)
      {
        int limit = fxc.transitionMux * (numleds/2);
        CopyFromNext(fxc,strip, numleds/2,numleds/2-limit);
        CopyFromNext(fxc,strip, numleds/2,numleds/2+limit);
      }
      if (fxc.strip[strip]->transitionType == Transition_TimedWipeRandom)
      {
        int limit = fxc.transitionMux * (numleds -1);
        for (int i=0;i<limit;i++)
        {
          int remap = fxc.strip[strip]->sequence[i];
          fxc.strip[strip]->palette[remap] = LerpRGB(fxc.transitionMux,fxc.strip[strip]->initialPalette[i],fxc.strip[strip]->nextPalette[i]);
        }
      }
      if (fxc.strip[strip]->transitionType == Transition_TimedFadeSin)
      {
        float sinMux = abs(sin(fxc.transitionMux * 3.14159f * 4 * 1.5f));
        for (int i = 0; i < numleds; i++)
          fxc.strip[strip]->palette[i] = LerpRGB(sinMux,fxc.strip[strip]->initialPalette[i],fxc.strip[strip]->nextPalette[i]);
      }
      if (fxc.strip[strip]->transitionType == Transition_TimedFadeCos)
      {
        float sinMux = cos(fxc.transitionMux * 3.14159f * 2 * 1.5f);
        for (int i = 0; i < numleds; i++)
          fxc.strip[strip]->palette[i] = LerpRGB(sinMux,fxc.strip[strip]->initialPalette[i],fxc.strip[strip]->nextPalette[i]);
      }
    }    
  }
}

void State_Poll(FxController &fxc)
{
  if (fxc.fxState == FxState_TestPattern)
  {
    FxEvent(fxc, fx_palette_rainbow); 
    FxEvent(fxc, fx_palanim_speed_neg);
    FxEvent(fxc, fx_palanim_speed_3);
  }

  if (fxc.fxState == FxState_MultiTestPattern)
    State_Poll_TestPattern(fxc);

  if (fxc.fxState == FxState_SideFX)
    SideFXPollState(fxc);
}

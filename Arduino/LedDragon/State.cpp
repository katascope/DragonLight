/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "Devices.h"
#include "State.h"
#include "FxCore.h"
#include "Fx.h"
#include "Track.h"
#include "Time.h"

void State_Poll_TestPattern(FxController &fxc)
{
#if ENABLE_NEOPIXEL
    FxEventProcess(fxc, fx_strip_all);
    FxEventProcess(fxc, fx_transition_fast);
    for (int strip=0;strip<NUM_STRIPS;strip++)
    {
      fxc.strip[strip]->paletteSpeed = 0;
      fxc.strip[strip]->paletteUpdateType = FxPaletteUpdateType::Always;
    }

    FxEventProcess(fxc, fx_strip_all);//fx_strip + LEDS_0);
    FxEventProcess(fxc, fx_palette_rainbow);
#if !ENABLE_MULTISTRIP
    FxEventProcess(fxc, fx_palette_half);
#endif    
    neopixelSetPalette(0, fxc.strip[0]->numleds, fxc.strip[0]->palette, fxc.strip[0]->paletteIndex);
    neopixelSetPixel(0, 0, CRGB_WHITE);
    
#if ENABLE_MULTISTRIP    
    FxEventProcess(fxc, fx_strip + LEDS_1);
    FxEventProcess(fxc, fx_palette_red);
    neopixelSetPalette(1, fxc.strip[1]->numleds, fxc.strip[1]->palette, fxc.strip[1]->paletteIndex);
    
    FxEventProcess(fxc, fx_strip + LEDS_2);
    FxEventProcess(fxc, fx_palette_yellow);
    neopixelSetPalette(2, fxc.strip[2]->numleds, fxc.strip[2]->palette, fxc.strip[2]->paletteIndex);
    
    FxEventProcess(fxc, fx_strip + LEDS_3);
    FxEventProcess(fxc, fx_palette_green);
    neopixelSetPalette(3, fxc.strip[3]->numleds, fxc.strip[3]->palette, fxc.strip[3]->paletteIndex);
    
    FxEventProcess(fxc, fx_strip + LEDS_4);
    FxEventProcess(fxc, fx_palette_cyan);
    neopixelSetPalette(4, fxc.strip[4]->numleds, fxc.strip[4]->palette, fxc.strip[4]->paletteIndex);
    
    FxEventProcess(fxc, fx_strip + LEDS_5);
    FxEventProcess(fxc, fx_palette_blue);
    neopixelSetPalette(5, fxc.strip[5]->numleds, fxc.strip[5]->palette, fxc.strip[5]->paletteIndex);
    
    FxEventProcess(fxc, fx_strip + LEDS_6);
    FxEventProcess(fxc, fx_palette_magenta);
    neopixelSetPalette(6, fxc.strip[6]->numleds, fxc.strip[6]->palette, fxc.strip[6]->paletteIndex);
    
    FxEventProcess(fxc, fx_strip + LEDS_7);
    FxEventProcess(fxc, fx_palette_orange);
    neopixelSetPalette(7, fxc.strip[7]->numleds, fxc.strip[7]->palette, fxc.strip[7]->paletteIndex);
#endif    
    FxEventProcess(fxc, fx_strip_all);

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

void PrintPalette(FxController &fxc)
{
  int limit = 4;
  Serial.print(F(" ini("));     
  for (int i=0;i<limit;i++)
  {
    Serial.print(fxc.strip[0]->initialPalette[i], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F("), ")); 
  Serial.print(F(" pal("));
  for (int i=0;i<limit;i++)
  {
    Serial.print(fxc.strip[0]->palette[i], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F(")")); 
  Serial.print(F(" next("));
  for (int i=0;i<limit;i++)
  {
    Serial.print(fxc.strip[0]->nextPalette[i], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F(")"));   
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

void State_Poll_Play(FxController &fxc, unsigned long timecode)
{
  
  unsigned long finalTimeCode = GetFinalTimeCodeEntry();
  if (GetTime() > finalTimeCode)
  {
    Serial.print(F("Done playing, time = "));
    Serial.print(GetTime());
    Serial.print(F(", restart = "));
    Serial.print(finalTimeCode);
    Serial.print(F(" "));

    switch (fxc.fxTrackEndAction) 
    {
      case FxTrackEndAction::StopAtEnd:
        Serial.println(F("StopAtEnd"));
        fxc.fxState = FxState_Default;    
        break;
      case FxTrackEndAction::LoopAtEnd:
        Serial.println(F("LoopAtEnd"));
        //trackStart(fxc, 0, timecode, fxc.trackEndAction); break;
        trackStart(fxc, 0, (unsigned long)(millis() - (signed long)TRACK_START_DELAY), FxTrackEndAction::LoopAtEnd);         
        timecode = GetTime();
        break;
    }
  }
  
  int match = GetCurrentTimeCodeMatch(timecode);
  int nextmatch = GetNextTimeCodeMatch(match);
  unsigned long matchedTimecode = SongTrack_timecode(match);
  unsigned long nextMatchedTimecode = SongTrack_timecode(nextmatch);

  //FxTrackSay(timecode, matchedTimecode, nextMatchedTimecode);
    
  if (matchedTimecode > getTimecodeLastMatched())
  {
    for (int strip=0;strip<NUM_STRIPS;strip++)
    {
      if (fxc.strip[strip]->transitionType >= Transition_TimedWipePos         
          && fxc.strip[strip]->transitionType <= Transition_TimedFadeCos)
      {
        CopyPalette(fxc.strip[strip]->numleds, fxc.strip[strip]->palette, fxc.strip[strip]->nextPalette);
      }
      fxc.strip[strip]->paletteUpdateType = FxPaletteUpdateType::Always;
    }

    for (int i = 0; i < GetNumSongTracks(); i++)
      if (SongTrack_timecode(i) == matchedTimecode)
      {
        /*
        Serial.println();
        Serial.print(F(" @"));
        Serial.print(GetTime());
        Serial.print(F(" "));
        Serial.print(matchedTimecode);
        Serial.print(F(" "));
        PrintFxEventName(SongTrack_event(i));        
        Serial.print(F(" : "));
        Serial.print(fxc.transitionMux);
        Serial.print(F(" "));
        for (int strip=0;strip<NUM_STRIPS;strip++)
        {
          PrintFxTransitionName(fxc.strip[strip]->transitionType);
          Serial.print(F(", "));
        }       
        Serial.print(F(" => "));
        */
        FxEventProcess(fxc, SongTrack_event(i));
      }
      
    setTimecodeLastMatched(timecode);//timeController.lastMatchedTimecode = timecode;
  }

  unsigned long totalSpan = nextMatchedTimecode - getTimecodeLastMatched();
  fxc.transitionMux = ((float)timecode - (float)getTimecodeLastMatched() ) / (float)totalSpan;

//For debugging palette/transitions

  Serial.print(F("Tr="));
  PrintFxTransitionName(fxc.strip[0]->transitionType);
  Serial.print(F(" Mux="));
  Serial.print(fxc.transitionMux);
  Serial.print(F(" .. "));
  PrintPalette(fxc);
  Serial.println();

  Do_Transition(fxc);
}

void DoPaletteMirror(FxController &fxc, int strip)
{  
  int height = fxc.strip[strip]->numleds;
  for (int i=0;i<height/2;i++)
  {
    fxc.strip[strip]->sideFXPalette[i] = fxc.strip[strip]->nextPalette[i*2];
    fxc.strip[strip]->sideFXPalette[height-1-i] = fxc.strip[strip]->nextPalette[i*2];
  }
}

void State_Poll_SideFX(FxController &fxc)
{
  int b = (int)(fxc.vol * 50.0f);
  
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.strip[strip]->fxSystem.channels[1].on) 
    {
      for (int i=0;i<fxc.strip[strip]->numleds;i++)
        fxc.strip[strip]->sideFXPalette[i] = fxc.strip[strip]->palette[i];
    }
    
    if (fxc.strip[strip]->fxSystem.channels[2].on) //sound bar
    {
      int len = (int)(fxc.vol * fxc.strip[strip]->numleds);
      int height = fxc.strip[strip]->numleds;
      if (fxc.strip[strip]->fxSystem.channels[2].state == 0)
      {
        for (int i=0;i<len;i++)        
        {
          float f = 255 * ((float)i/(float)1);
          fxc.strip[strip]->sideFXPalette[i] = fxc.strip[strip]->palette[i];
        }
        for (int i=len;i<fxc.strip[strip]->numleds;i++)
          fxc.strip[strip]->sideFXPalette[i] = LEDRGB(0,0,0);
      }
      else if (fxc.strip[strip]->fxSystem.channels[2].state == 1)
      {
        for (int i=0;i<fxc.strip[strip]->numleds;i++)
          fxc.strip[strip]->sideFXPalette[i] = LEDRGB(0,0,0);
        for (int i=0;i<len;i++)
        {
          float f = 255 * ((float)i/(float)1);
          fxc.strip[strip]->sideFXPalette[height/2+i/2] = fxc.strip[strip]->palette[i];
          fxc.strip[strip]->sideFXPalette[height/2-i/2] = fxc.strip[strip]->palette[i];
        }
      }
      else if (fxc.strip[strip]->fxSystem.channels[2].state == 2)
      {
        for (int i=0;i<fxc.strip[strip]->numleds;i++)
          fxc.strip[strip]->sideFXPalette[i] = LEDRGB(0,0,0);
        for (int i=0;i<len;i++)
        {
          float f = 255 * ((float)i/(float)1);
          fxc.strip[strip]->sideFXPalette[i/2] = fxc.strip[strip]->palette[i];
          fxc.strip[strip]->sideFXPalette[height-1-i/2] = fxc.strip[strip]->palette[i];
        }
      }
    }    
    if (fxc.strip[strip]->fxSystem.channels[3].on) 
        neopixelSetBrightness(strip,b);
    else
        neopixelSetBrightness(strip,BRIGHTNESS);
    
    if (fxc.strip[strip]->fxSystem.channels[4].on) 
    {
      if (fxc.strip[strip]->paletteSpeed == 0) fxc.strip[strip]->paletteSpeed = 1;
      if (fxc.strip[strip]->fxSystem.channels[4].state == 0) fxc.strip[strip]->paletteDirection = 1;
      if (fxc.strip[strip]->fxSystem.channels[4].state == 1) fxc.strip[strip]->paletteDirection = -1;
      if (fxc.strip[strip]->fxSystem.channels[4].state == 2) 
      {
        if (fxc.strip[strip]->paletteDirection > 0 && fxc.strip[strip]->paletteIndex+fxc.strip[strip]->paletteSpeed >= fxc.strip[strip]->numleds)
        {
          fxc.strip[strip]->paletteDirection = -1;
        }
        else if (fxc.strip[strip]->paletteDirection < 0 && fxc.strip[strip]->paletteIndex-fxc.strip[strip]->paletteSpeed <= 0)
        {
          fxc.strip[strip]->paletteDirection = 1;
        }
      }
      fxc.strip[strip]->paletteUpdateType = FxPaletteUpdateType::Once;
    }
    else 
      fxc.strip[strip]->paletteUpdateType = FxPaletteUpdateType::None;  

    if (fxc.strip[strip]->fxSystem.channels[5].on) 
    {
      int advance = (fxc.vol * 2) * (fxc.vol *2);
      if (fxc.strip[strip]->fxSystem.channels[5].state == 0) fxc.strip[strip]->paletteIndex += advance;
      if (fxc.strip[strip]->fxSystem.channels[5].state == 1) fxc.strip[strip]->paletteIndex -= advance;
      if (fxc.strip[strip]->paletteIndex >= fxc.strip[strip]->numleds)
        fxc.strip[strip]->paletteIndex -= fxc.strip[strip]->numleds;
      if (fxc.strip[strip]->paletteIndex < 0)
        fxc.strip[strip]->paletteIndex = fxc.strip[strip]->numleds - 1;      
    }
    
    if (fxc.strip[strip]->fxSystem.channels[6].on) 
    {
      DoPaletteMirror(fxc, strip);
    }
    
  }

  if (fxc.transitionMux >= 1)
  {
    FxAnimatePalette(fxc,true);
  }
}

void State_Poll(FxController &fxc)
{
  if (fxc.fxState == FxState_TestPattern)
  {
    FxInstantEvent(fxc, fx_palette_rainbow, FxPaletteUpdateType::Once); 
    FxInstantEvent(fxc, fx_speed_neg,       FxPaletteUpdateType::Once);
    FxInstantEvent(fxc, fx_speed_3,         FxPaletteUpdateType::Once);
  }

  if (fxc.fxState == FxState_MultiTestPattern)
    State_Poll_TestPattern(fxc);

  if (fxc.fxState == FxState_PlayingTrack)
    State_Poll_Play(fxc, GetTime());

  if (fxc.fxState == FxState_SideFX)
    State_Poll_SideFX(fxc);
}

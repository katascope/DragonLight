/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include <Arduino.h>
#include "Config.h"
#include "Devices.h"
#include "Fx.h"
#include "Track.h"
#include "Time.h"
#include "SideFXTrack.h"

void FxAnimatePalette(struct FxController &fxc, bool useSideFXPalette)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if ((int)fxc.strip[strip]->paletteUpdateType != 0)
    {
      fxc.strip[strip]->paletteIndex = fxc.strip[strip]->paletteIndex + (fxc.strip[strip]->paletteSpeed * fxc.strip[strip]->paletteDirection);
      if (fxc.strip[strip]->paletteIndex >= fxc.strip[strip]->numleds)
        fxc.strip[strip]->paletteIndex -= fxc.strip[strip]->numleds;
      if (fxc.strip[strip]->paletteIndex < 0)
        fxc.strip[strip]->paletteIndex = fxc.strip[strip]->numleds - 1;
    }
#if ENABLE_NEOPIXEL
    if (useSideFXPalette)
      neopixelSetPalette(strip, fxc.strip[strip]->numleds, fxc.strip[strip]->sideFXPalette, fxc.strip[strip]->paletteIndex);
    else
      neopixelSetPalette(strip, fxc.strip[strip]->numleds, fxc.strip[strip]->palette, fxc.strip[strip]->paletteIndex);
#endif    
  }
}

void FxUpdatePalette(struct FxController &fxc)
{
  FxProcessParticles(fxc);
  if (fxc.fxState != FxState::FxState_SideFX)
    FxAnimatePalette(fxc,false);  
}

void FxInstantEvent(FxController &fxc, int event, FxPaletteUpdateType paletteUpdateType)
{
  //fxc.fxState = FxState_Default;

  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
      fxc.strip[strip]->paletteUpdateType = paletteUpdateType;
//      fxc.strip[strip]->transitionType = Transition_Instant;
    }    
  }

  FxEventProcess(fxc, event);
  FxUpdatePalette(fxc);
}

void FxDisplayStatus(FxController &fxc)
{
      Serial.print(F("v="));
      Serial.print(fxc.vol);
      Serial.print(F("["));
      Serial.print(GetTime());
      Serial.print(F(":"));
      Serial.print(getTimecodeSongOffset());
      Serial.print(F(":"));
      Serial.print(getTimecodeTimeOffset());
      Serial.print(F("[state="));
      PrintFxStateName(fxc.fxState);
      Serial.print(F(",strip&="));
      Serial.print(fxc.stripMask);
      Serial.print(F(",mux="));
      Serial.print(fxc.transitionMux);

#if ENABLE_BLE      
      Serial.print(F(",BLE on"));
#else
      Serial.print(F(",BLE off"));
#endif

      //Strip debugging
#if ENABLE_NEOPIXEL  
      for (int strip=0;strip<NUM_STRIPS;strip++)
      {
        Serial.print(F("["));
        PrintFxTransitionName(fxc.strip[strip]->transitionType);
        Serial.print(F(",b="));
        Serial.print(fxc.strip[strip]->brightness);
        Serial.print(F(",ps="));
        Serial.print(fxc.strip[strip]->paletteSpeed);
        Serial.print(F(",pd="));
        Serial.print(fxc.strip[strip]->paletteDirection);
        Serial.print(F(",pi="));
        Serial.print(fxc.strip[strip]->paletteIndex);
        Serial.print(F(",u="));
        Serial.print(fxc.strip[strip]->paletteUpdateType);
        Serial.print(F("] "));
      }
#endif    
      Serial.print(F(",endAction="));
      PrintFxTrackEndAction(fxc.fxTrackEndAction);
      Serial.print(F(",ftc="));
      Serial.print(GetFinalTimeCodeEntry());      
      Serial.print(F(",tclm="));
      Serial.print(getTimecodeLastMatched());
      Serial.print(F(")]"));
      /*
      if (fxc.fxState == FxState_PlayingTrack)
      {
        int match = GetCurrentTimeCodeMatch(GetTime());
        Serial.print(F("="));
        PrintFxEventName(SongTrack_event(match));
        Serial.print(F(" "));
        Serial.print(SongTrack_timecode(match));
      }*/

}

void FxCreatePalette(FxController &fxController, int strip, uint32_t *pal16, unsigned int palSize)
{
  int numleds = fxController.strip[strip]->numleds;
  
  if (fxController.strip[strip]->paletteType == FxPaletteType::Smoothed)
  {
    if (fxController.strip[strip]->transitionType == Transition_Instant)
    {
      LerpPaletteFromMicroPalette(fxController.strip[strip]->palette, numleds, pal16, palSize);
      CopyPalette(numleds, fxController.strip[strip]->nextPalette, fxController.strip[strip]->palette);
    }
    else 
    {
      fxController.transitionMux = 0.0f;
      LerpPaletteFromMicroPalette(fxController.strip[strip]->nextPalette, numleds, pal16, palSize);
    }
  }  
  else if (fxController.strip[strip]->paletteType == FxPaletteType::Literal
        || fxController.strip[strip]->paletteType == FxPaletteType::Literal2
        || fxController.strip[strip]->paletteType == FxPaletteType::Literal3
        || fxController.strip[strip]->paletteType == FxPaletteType::Literal4)
  {
    int steps = 1;
    switch (fxController.strip[strip]->paletteType)
    {
      case FxPaletteType::Literal:  steps=1;break;
      case FxPaletteType::Literal2: steps=2;break;
      case FxPaletteType::Literal3: steps=3;break;
      case FxPaletteType::Literal4: steps=4;break;
    }
    if (fxController.strip[strip]->transitionType == Transition_Instant)
    {
      LiteralPaletteFromMicroPalette(fxController.strip[strip]->palette, numleds, pal16, palSize,steps);
      CopyPalette(numleds, fxController.strip[strip]->nextPalette, fxController.strip[strip]->palette);
    }
    else 
    {
      fxController.transitionMux = 0.0f;
      LiteralPaletteFromMicroPalette(fxController.strip[strip]->nextPalette, numleds, pal16, palSize,steps);      
    }
  }  
  CopyPalette(numleds, fxController.strip[strip]->initialPalette, fxController.strip[strip]->palette);
}

void CreateSingleColor(FxController &fxc, uint8_t r, uint8_t g, uint8_t b)
{  
  uint32_t palette[16];
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
       for (int i=0;i<16;i++)
       {
         palette[i] = LEDRGB(r,g,b);
       }
       FxCreatePalette(fxc, strip, palette,16);
    }
  }
}

void CreateDoubleColor(FxController &fxc, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2 )
{  
  uint32_t palette[16];
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
        palette[0] = LEDRGB(r1,g1,b1);
        palette[1] = LEDRGB(r2,g2,b2);
        FxCreatePalette(fxc,strip, palette,2);
    }
  }
}

void CreateQuadColor(FxController &fxc, 
  byte r1, byte g1, byte b1, 
  byte r2, byte g2, byte b2,
  byte r3, byte g3, byte b3,
  byte r4, byte g4, byte b4)
{  
  uint32_t palette[16];
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
        palette[0] = LEDRGB(r1,g1,b1);
        palette[1] = LEDRGB(r2,g2,b2);
        palette[2] = LEDRGB(r3,g3,b3);
        palette[3] = LEDRGB(r4,g4,b4);
        FxCreatePalette(fxc,strip, palette,4);
    }
  }
}

void Create16Color(FxController &fxc, uint32_t b0,uint32_t b1,uint32_t b2,uint32_t b3, uint32_t b4,uint32_t b5,uint32_t b6,uint32_t b7,
                        uint32_t b8,uint32_t b9,uint32_t b10,uint32_t b11, uint32_t b12,uint32_t b13,uint32_t b14,uint32_t b15)
{
  uint32_t palette[16];
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
        palette[0] = b0;
        palette[1] = b1;
        palette[2] = b2;
        palette[3] = b3;
        palette[4] = b4;
        palette[5] = b5;
        palette[6] = b6;
        palette[7] = b7;
        palette[8] = b8;
        palette[9] = b9;
        palette[10] = b10;
        palette[11] = b11;
        palette[12] = b12;
        palette[13] = b13;
        palette[14] = b14;
        palette[15] = b15;
        FxCreatePalette(fxc,strip, palette,16);
    }
  }
}

void CreateSinglePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b) {
  Create16Color(fxc, LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0));
}

void CreateDoublePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b) {
  Create16Color(fxc, LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0));
}

void CreateTriplePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b) {
  Create16Color(fxc,  LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0));
}

void CreateQuadPulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b) {
  Create16Color(fxc,  LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0));
}

void sequence_linear(unsigned int arr[], int n)
{
  for (unsigned int i=0;i < n;i++)
    arr[i] = n;
}

void swap(unsigned int *a, unsigned int *b)
{
  unsigned int temp = *a;
  *a = *b;
  *b = temp;
}

void sequence_random(unsigned int arr[], int n)
{
  //srand(time(NULL));
  for (int i = n - 1; i > 0; i--)
  {
    unsigned int j = rand() % (i + 1);
    swap(&arr[i], &arr[j]);
  }
}

void SetTransitionType(FxController &fxc, FxTransitionType t)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
      fxc.strip[strip]->transitionType = t;
      if (t == Transition_TimedWipePos || t == Transition_TimedWipeNeg
        || t == Transition_TimedWipeOutIn || t == Transition_TimedWipeInOut
        || t == Transition_TimedFadeSin || t == Transition_TimedFadeCos) 
        {
          fxc.strip[strip]->paletteUpdateType = FxPaletteUpdateType::None;
          fxc.strip[strip]->paletteIndex = 0;
        }
        if (t == fx_transition_timed_wipe_neg)
          fxc.strip[strip]->paletteIndex = 15;
    }
    if (t == Transition_TimedWipeRandom)
    {
      fxc.strip[strip]->paletteUpdateType = FxPaletteUpdateType::None;
      fxc.strip[strip]->paletteIndex = 0;
      //sequence_linear(fxc.strip[strip]->sequence, fxc.strip[strip]->numleds);
      sequence_random(fxc.strip[strip]->sequence, fxc.strip[strip]->numleds);
    }    
  }
}

void ResetPaletteSpeed(FxController &fxc)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
      fxc.strip[strip]->paletteSpeed = 0;
      fxc.strip[strip]->paletteIndex = 0;
      fxc.strip[strip]->paletteDirection = 1;
    }
  }
}

void SetPaletteSpeed(FxController &fxc, int v)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
    if (fxc.stripMask & (1<<strip)) 
      fxc.strip[strip]->paletteSpeed = v;
}

void ChangePaletteSpeed(FxController &fxc, int ps)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
    if (fxc.stripMask & (1<<strip)) 
    {
      fxc.strip[strip]->paletteSpeed += ps;
      if (fxc.strip[strip]->paletteSpeed < 0)
       fxc.strip[strip]->paletteSpeed = 0;
    }
}

void SetPaletteDirection(FxController &fxc, int c)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
      fxc.strip[strip]->paletteDirection = c;
      if (fxc.strip[strip]->paletteSpeed >= 18)
        fxc.strip[strip]->paletteSpeed = 18;
      if (fxc.strip[strip]->paletteSpeed < 0)
        fxc.strip[strip]->paletteSpeed = 0;
    }
  }
}

void FxSelectLeds(FxController &fxc)
{
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    unsigned long rgb1 = WEBRGB::Black;
    unsigned long rgb2 = WEBRGB::Black;
    unsigned long rgb3 = WEBRGB::Black;
    unsigned long rgb4 = WEBRGB::Black;
    if (fxc.select & 1) rgb1 = WEBRGB::Green;
    if (fxc.select & 2) rgb2 = WEBRGB::Green;
    if (fxc.select & 4) rgb3 = WEBRGB::Green;
    if (fxc.select & 8) rgb4 = WEBRGB::Green;
    
    fxc.stripMask = LEDS_0;
    Create16Color(fxc, 
      rgb1,rgb1,rgb1,rgb1,
      rgb2,rgb2,rgb2,rgb2,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      
    fxc.stripMask = LEDS_1;
    Create16Color(fxc, 
      rgb3,rgb3,rgb3,rgb3,
      rgb4,rgb4,rgb4,rgb4,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
    fxc.stripMask = (LEDS_0|LEDS_1);
}

void FxEventProcess(FxController &fxc,int event)
{
  bool updatePal = false;

  if (event >= fx_stripmask_0 && event <= fx_stripmask_255)
  {
    fxc.stripMask = (unsigned int)event-(unsigned int)fx_stripmask_0;
    return;
  }

  if (event >= fx_status_section_0 && event <= fx_status_section_15)
  {
    fxc.select = (unsigned int)event-(unsigned int)fx_status_section_0;
    FxSelectLeds(fxc);
    return;
  }
  
  switch (event)
  {
    case fx_strip_all:   fxc.stripMask = (unsigned int)(LEDS_0|LEDS_1|LEDS_2|LEDS_3|LEDS_4|LEDS_5|LEDS_6|LEDS_7); fxc.SetParticlesLoc(0);break;
    case fx_strip_odds:  fxc.stripMask = (unsigned int)(LEDS_1|LEDS_3|LEDS_5|LEDS_7); break;
    case fx_strip_evens: fxc.stripMask = (unsigned int)(LEDS_0|LEDS_2|LEDS_4|LEDS_6); break;
    case fx_strip_none:  fxc.stripMask = (unsigned int)0; break;
    case fx_strip_inv:   fxc.stripMask = (unsigned int)~(fxc.stripMask); break;
    
    case fx_speed_0:
    case fx_speed_1:
    case fx_speed_2:
    case fx_speed_3:
    case fx_speed_4:
    case fx_speed_5:
    case fx_speed_6:
    case fx_speed_7:
    case fx_speed_8:
    case fx_speed_9:
    case fx_speed_10:
    case fx_speed_11:
    case fx_speed_12:
    case fx_speed_13:
    case fx_speed_14:
    case fx_speed_15:
    case fx_speed_16:
    case fx_speed_17:
    case fx_speed_18:
      SetPaletteSpeed(fxc,event-fx_speed_0);
      break;
    case fx_speed_32:
      SetPaletteSpeed(fxc,32);
      break;

    case fx_sidefx_full_reset:
    {    
      fxc.fxState = FxState_SideFX;
      fxc.stripMask = (unsigned int)(LEDS_0|LEDS_1|LEDS_2|LEDS_3|LEDS_4|LEDS_5|LEDS_6|LEDS_7); fxc.SetParticlesLoc(0);break;
      fxc.KillFX();
      ResetPaletteSpeed(fxc);
      SetPaletteSpeed(fxc,fx_speed_0);
      SetTransitionType(fxc,Transition_Instant);
      fxc.transitionMux = 0.0f;
      CreateSingleColor(fxc, DARK);
      FxAnimatePalette(fxc,true);
      FxAnimatePalette(fxc,false);
      break;    
    }

    case fx_speed_pos:SetPaletteDirection(fxc,1);break;
    case fx_speed_neg:SetPaletteDirection(fxc,-1);break;
    case fx_speed_inc:ChangePaletteSpeed(fxc,1);break;
    case fx_speed_dec:ChangePaletteSpeed(fxc,-1);break;
    case fx_speed_rst:
      ResetPaletteSpeed(fxc);
      break;
      
    case fx_transition_fast:              SetTransitionType(fxc,Transition_Instant);break;
    case fx_transition_timed_fade:        SetTransitionType(fxc,Transition_TimedFade);break;
    case fx_transition_timed_wipe_pos:    SetTransitionType(fxc,Transition_TimedWipePos);break;
    case fx_transition_timed_wipe_neg:    SetTransitionType(fxc,Transition_TimedWipeNeg);break;
    case fx_transition_timed_wipe_outin:  SetTransitionType(fxc,Transition_TimedWipeOutIn);break;
    case fx_transition_timed_wipe_inout:  SetTransitionType(fxc,Transition_TimedWipeInOut);break;
    case fx_transition_timed_wipe_random: SetTransitionType(fxc,Transition_TimedWipeRandom);break;
    case fx_transition_timed_fade_sin:    SetTransitionType(fxc,Transition_TimedFadeSin);break;
    case fx_transition_timed_fade_cos:    SetTransitionType(fxc,Transition_TimedFadeCos);break;

    case fx_palette_lead:   CreateSingleColor(fxc, BLUE);break;
    case fx_palette_follow: CreateSingleColor(fxc, RED);break;
    
    case fx_palette_dark:   CreateSingleColor(fxc, DARK);break;
    case fx_palette_white:  CreateSingleColor(fxc, WHITE);break;
    case fx_palette_red:    CreateSingleColor(fxc, RED);break;
    case fx_palette_yellow: CreateSingleColor(fxc, YELLOW);break;
    case fx_palette_green:  CreateSingleColor(fxc, GREEN);break;
    case fx_palette_cyan:   CreateSingleColor(fxc, CYAN);break;
    case fx_palette_blue:   CreateSingleColor(fxc, BLUE);break;
    case fx_palette_magenta:CreateSingleColor(fxc, MAGENTA);break;
    case fx_palette_orange: CreateSingleColor(fxc, ORANGE);break;
    case fx_palette_half:   CreateSingleColor(fxc, HALF);break;
    case fx_palette_lowhalf:CreateSingleColor(fxc, LOWHALF);break;

    case fx_palette_pulse_dark:CreateSinglePulseBand(fxc, DARK);break;
    case fx_palette_pulse_white:CreateSinglePulseBand(fxc, WHITE);break;
    case fx_palette_pulse_red:CreateSinglePulseBand(fxc, RED);break;
    case fx_palette_pulse_yellow:CreateSinglePulseBand(fxc, YELLOW);break;
    case fx_palette_pulse_green:CreateSinglePulseBand(fxc, GREEN);break;
    case fx_palette_pulse_cyan:CreateSinglePulseBand(fxc, CYAN);break;
    case fx_palette_pulse_blue:CreateSinglePulseBand(fxc, BLUE);break;
    case fx_palette_pulse_magenta:CreateSinglePulseBand(fxc, MAGENTA);break;
    case fx_palette_pulse_orange:CreateSinglePulseBand(fxc, ORANGE);break;
    case fx_palette_pulse_half:CreateSinglePulseBand(fxc, HALF);break;
    case fx_palette_pulse_lowhalf:CreateSinglePulseBand(fxc, LOWHALF);break;

    case fx_palette_pulse2_dark:CreateDoublePulseBand(fxc, DARK);break;
    case fx_palette_pulse2_white:CreateDoublePulseBand(fxc, WHITE);break;
    case fx_palette_pulse2_red:CreateDoublePulseBand(fxc, RED);break;
    case fx_palette_pulse2_yellow:CreateDoublePulseBand(fxc, YELLOW);break;
    case fx_palette_pulse2_green:CreateDoublePulseBand(fxc, GREEN);break;
    case fx_palette_pulse2_cyan:CreateDoublePulseBand(fxc, CYAN);break;
    case fx_palette_pulse2_blue:CreateDoublePulseBand(fxc, BLUE);break;
    case fx_palette_pulse2_magenta:CreateDoublePulseBand(fxc, MAGENTA);break;
    case fx_palette_pulse2_orange:CreateDoublePulseBand(fxc, ORANGE);break;
    case fx_palette_pulse2_half:CreateDoublePulseBand(fxc, HALF);break;
    case fx_palette_pulse2_lowhalf:CreateDoublePulseBand(fxc, LOWHALF);break;    

    case fx_palette_pulse3_dark:CreateTriplePulseBand(fxc, DARK);break;
    case fx_palette_pulse3_white:CreateTriplePulseBand(fxc, WHITE);break;
    case fx_palette_pulse3_red:CreateTriplePulseBand(fxc, RED);break;
    case fx_palette_pulse3_yellow:CreateTriplePulseBand(fxc, YELLOW);break;
    case fx_palette_pulse3_green:CreateTriplePulseBand(fxc, GREEN);break;
    case fx_palette_pulse3_cyan:CreateTriplePulseBand(fxc, CYAN);break;
    case fx_palette_pulse3_blue:CreateTriplePulseBand(fxc, BLUE);break;
    case fx_palette_pulse3_magenta:CreateTriplePulseBand(fxc, MAGENTA);break;
    case fx_palette_pulse3_orange:CreateTriplePulseBand(fxc, ORANGE);break;
    case fx_palette_pulse3_half:CreateTriplePulseBand(fxc, HALF);break;
    case fx_palette_pulse3_lowhalf:CreateTriplePulseBand(fxc, LOWHALF);break;    

    case fx_palette_pulse4_dark:CreateQuadPulseBand(fxc, DARK);break;
    case fx_palette_pulse4_white:CreateQuadPulseBand(fxc, WHITE);break;
    case fx_palette_pulse4_red:CreateQuadPulseBand(fxc, RED);break;
    case fx_palette_pulse4_yellow:CreateQuadPulseBand(fxc, YELLOW);break;
    case fx_palette_pulse4_green:CreateQuadPulseBand(fxc, GREEN);break;
    case fx_palette_pulse4_cyan:CreateQuadPulseBand(fxc, CYAN);break;
    case fx_palette_pulse4_blue:CreateQuadPulseBand(fxc, BLUE);break;
    case fx_palette_pulse4_magenta:CreateQuadPulseBand(fxc, MAGENTA);break;
    case fx_palette_pulse4_orange:CreateQuadPulseBand(fxc, ORANGE);break;
    case fx_palette_pulse4_half:CreateQuadPulseBand(fxc, HALF);break;
    case fx_palette_pulse4_lowhalf:CreateQuadPulseBand(fxc, LOWHALF);break;    

    case fx_palette_dr: CreateDoubleColor(fxc, DARK, RED); break;
    case fx_palette_dy: CreateDoubleColor(fxc, DARK, YELLOW); break;
    case fx_palette_dg: CreateDoubleColor(fxc, DARK, GREEN); break;
    case fx_palette_dc: CreateDoubleColor(fxc, DARK, CYAN); break;
    case fx_palette_db: CreateDoubleColor(fxc, DARK, BLUE); break;
    case fx_palette_dm: CreateDoubleColor(fxc, DARK, MAGENTA); break;
    case fx_palette_wr: CreateDoubleColor(fxc, WHITE, RED); break;
    case fx_palette_wy: CreateDoubleColor(fxc, WHITE, YELLOW); break;
    case fx_palette_wg: CreateDoubleColor(fxc, WHITE, GREEN); break;
    case fx_palette_wc: CreateDoubleColor(fxc, WHITE, CYAN); break;
    case fx_palette_wb: CreateDoubleColor(fxc, WHITE, BLUE); break;
    case fx_palette_wm: CreateDoubleColor(fxc, WHITE, MAGENTA); break;
    case fx_palette_ry: CreateDoubleColor(fxc, RED, YELLOW); break;
    case fx_palette_rg: CreateDoubleColor(fxc, RED, GREEN); break;
    case fx_palette_rc: CreateDoubleColor(fxc, RED, CYAN); break;
    case fx_palette_rb: CreateDoubleColor(fxc, RED, BLUE); break;
    case fx_palette_rm: CreateDoubleColor(fxc, RED, MAGENTA); break;
    case fx_palette_yg: CreateDoubleColor(fxc, YELLOW, GREEN); break;
    case fx_palette_yc: CreateDoubleColor(fxc, YELLOW, CYAN); break;
    case fx_palette_yb: CreateDoubleColor(fxc, YELLOW, BLUE); break;
    case fx_palette_ym: CreateDoubleColor(fxc, YELLOW, MAGENTA); break;
    case fx_palette_gc: CreateDoubleColor(fxc, GREEN, CYAN); break;
    case fx_palette_gb: CreateDoubleColor(fxc, GREEN, BLUE); break;
    case fx_palette_gm: CreateDoubleColor(fxc, GREEN, MAGENTA); break;
    case fx_palette_cb: CreateDoubleColor(fxc, CYAN, BLUE); break;
    case fx_palette_cm: CreateDoubleColor(fxc, CYAN, MAGENTA); break;
    case fx_palette_bm: CreateDoubleColor(fxc, BLUE, MAGENTA); break;

    case fx_palette_wry: CreateQuadColor(fxc, WHITE, RED,    WHITE, YELLOW); break;
    case fx_palette_wrg: CreateQuadColor(fxc, WHITE, RED,    WHITE, GREEN); break;
    case fx_palette_wrc: CreateQuadColor(fxc, WHITE, RED,    WHITE, CYAN); break;
    case fx_palette_wrb: CreateQuadColor(fxc, WHITE, RED,    WHITE, BLUE); break;
    case fx_palette_wrm: CreateQuadColor(fxc, WHITE, RED,    WHITE, MAGENTA); break;
    case fx_palette_wyg: CreateQuadColor(fxc, WHITE, YELLOW, WHITE, GREEN); break;
    case fx_palette_wyc: CreateQuadColor(fxc, WHITE, YELLOW, WHITE, CYAN); break;
    case fx_palette_wyb: CreateQuadColor(fxc, WHITE, YELLOW, WHITE, BLUE); break;
    case fx_palette_wym: CreateQuadColor(fxc, WHITE, YELLOW, WHITE, MAGENTA); break;
    case fx_palette_wgc: CreateQuadColor(fxc, WHITE, GREEN,  WHITE, CYAN); break;
    case fx_palette_wgb: CreateQuadColor(fxc, WHITE, GREEN,  WHITE, BLUE); break;
    case fx_palette_wgm: CreateQuadColor(fxc, WHITE, GREEN,  WHITE, MAGENTA); break;
    case fx_palette_wcb: CreateQuadColor(fxc, WHITE, CYAN,   WHITE, BLUE); break;
    case fx_palette_wcm: CreateQuadColor(fxc, WHITE, CYAN,   WHITE, MAGENTA); break;
    case fx_palette_wbm: CreateQuadColor(fxc, WHITE, BLUE,   WHITE, MAGENTA); break;

    case fx_palette_dry: CreateQuadColor(fxc, DARK, RED,    DARK, YELLOW); break;
    case fx_palette_drg: CreateQuadColor(fxc, DARK, RED,    DARK, GREEN); break;
    case fx_palette_drc: CreateQuadColor(fxc, DARK, RED,    DARK, CYAN); break;
    case fx_palette_drb: CreateQuadColor(fxc, DARK, RED,    DARK, BLUE); break;
    case fx_palette_drm: CreateQuadColor(fxc, DARK, RED,    DARK, MAGENTA); break;
    case fx_palette_dyg: CreateQuadColor(fxc, DARK, YELLOW, DARK, GREEN); break;
    case fx_palette_dyc: CreateQuadColor(fxc, DARK, YELLOW, DARK, CYAN); break;
    case fx_palette_dyb: CreateQuadColor(fxc, DARK, YELLOW, DARK, BLUE); break;
    case fx_palette_dym: CreateQuadColor(fxc, DARK, YELLOW, DARK, MAGENTA); break;
    case fx_palette_dgc: CreateQuadColor(fxc, DARK, GREEN,  DARK, CYAN); break;
    case fx_palette_dgb: CreateQuadColor(fxc, DARK, GREEN,  DARK, BLUE); break;
    case fx_palette_dgm: CreateQuadColor(fxc, DARK, GREEN,  DARK, MAGENTA); break;
    case fx_palette_dcb: CreateQuadColor(fxc, DARK, CYAN,   DARK, BLUE); break;
    case fx_palette_dcm: CreateQuadColor(fxc, DARK, CYAN,   DARK, MAGENTA); break;
    case fx_palette_dbm: CreateQuadColor(fxc, DARK, BLUE,   DARK, MAGENTA); break;

    case fx_palette_rbm: CreateQuadColor(fxc, RED, MAGENTA, BLUE,    DARK); break;
    case fx_palette_rgb: CreateQuadColor(fxc, RED, GREEN, BLUE,      DARK); break;
    case fx_palette_cmy: CreateQuadColor(fxc, CYAN, MAGENTA, YELLOW, DARK); break;
    case fx_palette_cbm: CreateQuadColor(fxc, CYAN, BLUE, MAGENTA,   DARK); break;

    case fx_palette_lava:Create16Color(fxc,
        WEBRGB::Black,WEBRGB::Maroon,WEBRGB::Black,WEBRGB::Maroon,    
        WEBRGB::DarkRed,WEBRGB::Maroon,WEBRGB::DarkRed,WEBRGB::DarkRed,    
        WEBRGB::DarkRed,WEBRGB::DarkRed,WEBRGB::Red,WEBRGB::Orange,    
        WEBRGB::White,WEBRGB::Orange,WEBRGB::Red,WEBRGB::DarkRed);
      break;
    case fx_palette_cloud:Create16Color(fxc,  
      WEBRGB::Blue,WEBRGB::DarkBlue,WEBRGB::DarkBlue,WEBRGB::DarkBlue,
      WEBRGB::DarkBlue,WEBRGB::DarkBlue,WEBRGB::DarkBlue,WEBRGB::DarkBlue,
      WEBRGB::Blue,WEBRGB::DarkBlue,WEBRGB::SkyBlue,WEBRGB::SkyBlue,
      WEBRGB::LightBlue,WEBRGB::White,WEBRGB::LightBlue,WEBRGB::SkyBlue);
      break;
    case fx_palette_ocean:Create16Color(fxc,    
      WEBRGB::DarkGreen,
      WEBRGB::DarkGreen,
      WEBRGB::DarkOliveGreen,
      WEBRGB::DarkGreen,
  
      WEBRGB::Green,
      WEBRGB::ForestGreen,
      WEBRGB::OliveDrab,
      WEBRGB::Green,
  
      WEBRGB::SeaGreen,
      WEBRGB::MediumAquamarine,
      WEBRGB::LimeGreen,
      WEBRGB::YellowGreen,
  
      WEBRGB::LightGreen,
      WEBRGB::LawnGreen,
      WEBRGB::MediumAquamarine,
      WEBRGB::ForestGreen);break;
    case fx_palette_forest:Create16Color(fxc,    
      WEBRGB::DarkGreen,
      WEBRGB::DarkGreen,
      WEBRGB::DarkOliveGreen,
      WEBRGB::DarkGreen,
  
      WEBRGB::Green,
      WEBRGB::ForestGreen,
      WEBRGB::OliveDrab,
      WEBRGB::Green,
  
      WEBRGB::SeaGreen,
      WEBRGB::MediumAquamarine,
      WEBRGB::LimeGreen,
      WEBRGB::YellowGreen,
  
      WEBRGB::LightGreen,
      WEBRGB::LawnGreen,
      WEBRGB::MediumAquamarine,
      WEBRGB::ForestGreen);break;
    case fx_palette_rainbow:Create16Color(fxc, 
      0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00,
      0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
      0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
      0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B);break;
    case fx_palette_rainbowstripe:Create16Color(fxc,    
      0xFF0000, 0x000000, 0xAB5500, 0x000000,
      0xABAB00, 0x000000, 0x00FF00, 0x000000,
      0x00AB55, 0x000000, 0x0000FF, 0x000000,
      0x5500AB, 0x000000, 0xAB0055, 0x000000);break;
    case fx_palette_party:Create16Color(fxc,   
      0x5500AB, 0x84007C, 0xB5004B, 0xE5001B,
      0xE81700, 0xB84700, 0xAB7700, 0xABAB00,
      0xAB5500, 0xDD2200, 0xF2000E, 0xC2003E,
      0x8F0071, 0x5F00A1, 0x2F00D0, 0x0007F9);break;
    case fx_palette_heat:Create16Color(fxc,
      0x000000, 0x330000, 0x660000, 0x990000, 
      0xCC0000, 0xFF0000, 0xFF3300, 0xFF6600, 
      0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33, 
      0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF);break;

    case fx_particles_off: fxc.SetParticlesRunning(false); break;
    case fx_particles_pos: fxc.SetParticlesRunning(true); fxc.SetParticleMode(FX_PARTICLEMODE_STAR); fxc.SetParticlesDirection(1); break;
    case fx_particles_neg: fxc.SetParticlesRunning(true); fxc.SetParticleMode(FX_PARTICLEMODE_STAR); fxc.SetParticlesDirection(-1); break;
    case fx_particles_rnd: fxc.SetParticlesRunning(true); fxc.SetParticleMode(FX_PARTICLEMODE_RND); fxc.SetParticlesLength(1);break;//
    
    case fx_particles_length_1: fxc.SetParticlesLength(1); break;
    case fx_particles_length_2: fxc.SetParticlesLength(2); break;
    case fx_particles_length_3: fxc.SetParticlesLength(3); break;
    case fx_particles_length_4: fxc.SetParticlesLength(4); break;

    case fx_particles_color_dark:    fxc.SetParticlesColor(CRGB_DARK); break;
    case fx_particles_color_white:   fxc.SetParticlesColor(CRGB_WHITE); break;
    case fx_particles_color_red:     fxc.SetParticlesColor(CRGB_RED); break;
    case fx_particles_color_yellow:  fxc.SetParticlesColor(CRGB_YELLOW); break;
    case fx_particles_color_green:   fxc.SetParticlesColor(CRGB_GREEN); break;
    case fx_particles_color_cyan:    fxc.SetParticlesColor(CRGB_CYAN); break;
    case fx_particles_color_blue:    fxc.SetParticlesColor(CRGB_BLUE); break;
    case fx_particles_color_magenta: fxc.SetParticlesColor(CRGB_MAGENTA); break;
    case fx_particles_color_orange:  fxc.SetParticlesColor(CRGB_ORANGE); break;
    case fx_particles_color_half:    fxc.SetParticlesColor(CRGB_HALF); break;
    case fx_particles_color_lowhalf: fxc.SetParticlesColor(CRGB_LOWHALF); break;


    case fx_status_section_1:       
    fxc.select = 1;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_0;
    Create16Color(fxc, WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;
    case fx_status_section_2:       
    fxc.select = 2;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_0;
    Create16Color(fxc, WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
          break;

    case fx_status_section_3:
    fxc.select = 3;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_1;
    Create16Color(fxc, WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;
    case fx_status_section_4:
    fxc.select = 4;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_1;
    Create16Color(fxc, WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;

    case fx_status_section_5:
    fxc.select = 5;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_0;
    Create16Color(fxc, WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
    fxc.stripMask = LEDS_1;
    Create16Color(fxc, WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;

    case fx_status_section_6:
    fxc.select = 6;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_0;
    Create16Color(fxc, 
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,      
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
    fxc.stripMask = LEDS_1;
    Create16Color(fxc,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;

    case fx_status_section_7:
    fxc.select = 7;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_0;
    Create16Color(fxc, 
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,      
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,      
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;

    case fx_status_section_8:
    fxc.select = 8;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_1;
    Create16Color(fxc, 
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,      
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,      
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;                  

    case fx_status_section_9:
    fxc.select = 9;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_0;
    Create16Color(fxc, WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
    fxc.stripMask = LEDS_1;
    Create16Color(fxc, 
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,      
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;

    case fx_status_section_10:
    fxc.select = 10;
    fxc.stripMask = (LEDS_0|LEDS_1);ResetPaletteSpeed(fxc);CreateSingleColor(fxc, DARK);
    fxc.stripMask = LEDS_0;
    Create16Color(fxc, 
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,      
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
    fxc.stripMask = LEDS_1;
    Create16Color(fxc,
      WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,WEBRGB::Green,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,      
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,
      WEBRGB::Black,WEBRGB::Black,WEBRGB::Black,WEBRGB::Black);
      break;      

    case fx_palette_type_smoothed:
      fxc.SetPaletteType(FxPaletteType::Smoothed);      
      break;
      
    case fx_palette_type_literal:  fxc.SetPaletteType(FxPaletteType::Literal); break;
    case fx_palette_type_literal2: fxc.SetPaletteType(FxPaletteType::Literal2); break;
    case fx_palette_type_literal3: fxc.SetPaletteType(FxPaletteType::Literal3); break;
    case fx_palette_type_literal4: fxc.SetPaletteType(FxPaletteType::Literal4); break;
      
 }
}


void FxProcessParticles(FxController &fxc)
{
  if (fxc.fxState == FxState_Default && fxc.HasRunning())
  {
    for (int strip=0;strip<NUM_STRIPS;strip++)
      for (int led=0;led<fxc.strip[strip]->numleds;led++)
        fxc.strip[strip]->palette[led] = fxc.strip[strip]->initialPalette[led];
  }

  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    
            for (int particle=0;particle<NUM_PARTICLES;particle++)
            {
              if (fxc.strip[strip]->particles[particle].on)
              {
               if (fxc.strip[strip]->particles[particle].mode == FX_PARTICLEMODE_RND)
                {                
                  fxc.strip[strip]->particles[particle].len += 2;                  
                  int len = fxc.strip[strip]->particles[particle].len;
                  
                  //for (int i=0;i<fxc.strip[strip]->particles[particle].len;i++)
                  //{
                    int loc = fxc.strip[strip]->particles[particle].loc;
                    //fxc.strip[strip]->particles[particle].rgb;
                    unsigned int r = (fxc.strip[strip]->palette[ loc ]>> 16) & 0xFF;
                    unsigned int g = (fxc.strip[strip]->palette[ loc ] >> 8) & 0xFF;
                    unsigned int b = (fxc.strip[strip]->palette[ loc ] >> 0) & 0xFF;
                    if (len < 2){ r *= 1; g *= 1; b *= 1; }
                    if (len < 4){ r *= 2; g *= 2; b *= 2; }
                    if (len < 6){ r *= 3; g *= 3; b *= 3; }
                    if (len < 8){ r *= 4; g *= 4; b *= 4; }
                    if (len < 10){ r *= 6; g *= 6; b *= 6; }
                    if (r > 255) r = 255;
                    if (g > 255) g = 255;
                    if (b > 255) b = 255;
                   
                    fxc.strip[strip]->palette[ loc ] = LEDRGB(r,g,b);
                    //if (loc + len < fxc.strip[strip]->numleds) fxc.strip[strip]->palette[ loc + len ] = LEDRGB(r,g,b);
                    //if (loc - len > 0) fxc.strip[strip]->palette[ loc - len ] = LEDRGB(r,g,b);
                  
                  if ((loc + len) >= fxc.strip[strip]->numleds || (loc - len) < 0)
                  {
                    fxc.strip[strip]->particles[particle].loc = rand() % (fxc.strip[strip]->numleds-1);
                    fxc.strip[strip]->particles[particle].len = 1;
                  }                   
                }
              }
            }
  }  
}

void FxPaletteById(FxController &fxc, int paletteId)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
      fxc.strip[strip]->paletteId = paletteId;    
    }
  }
  
    switch (paletteId)
    {
      case 1: FxInstantEvent(fxc, fx_palette_dry,     FxPaletteUpdateType::Once); break;
      case 2: FxInstantEvent(fxc, fx_palette_drg,     FxPaletteUpdateType::Once); break;
      case 3: FxInstantEvent(fxc, fx_palette_drc,     FxPaletteUpdateType::Once); break;
      case 4: FxInstantEvent(fxc, fx_palette_drb,     FxPaletteUpdateType::Once); break;
      case 5: FxInstantEvent(fxc, fx_palette_drm,     FxPaletteUpdateType::Once); break;
      case 6: FxInstantEvent(fxc, fx_palette_wry,     FxPaletteUpdateType::Once); break;
      case 7: FxInstantEvent(fxc, fx_palette_wrg,     FxPaletteUpdateType::Once); break;
      case 8: FxInstantEvent(fxc, fx_palette_wrc,     FxPaletteUpdateType::Once); break;
      case 9: FxInstantEvent(fxc, fx_palette_wrb,     FxPaletteUpdateType::Once); break;
      case 10: FxInstantEvent(fxc, fx_palette_wrm,     FxPaletteUpdateType::Once); break;

      case 11: FxInstantEvent(fxc, fx_palette_dyg,     FxPaletteUpdateType::Once); break;
      case 12: FxInstantEvent(fxc, fx_palette_dyc,     FxPaletteUpdateType::Once); break;
      case 13: FxInstantEvent(fxc, fx_palette_dyb,     FxPaletteUpdateType::Once); break;
      case 14: FxInstantEvent(fxc, fx_palette_dym,     FxPaletteUpdateType::Once); break;
      case 15: FxInstantEvent(fxc, fx_palette_wyg,     FxPaletteUpdateType::Once); break;
      case 16: FxInstantEvent(fxc, fx_palette_wyc,     FxPaletteUpdateType::Once); break;
      case 17: FxInstantEvent(fxc, fx_palette_wyb,     FxPaletteUpdateType::Once); break;
      case 18: FxInstantEvent(fxc, fx_palette_wym,     FxPaletteUpdateType::Once); break;

      case 19: FxInstantEvent(fxc, fx_palette_dgc,     FxPaletteUpdateType::Once); break;
      case 20: FxInstantEvent(fxc, fx_palette_dgb,     FxPaletteUpdateType::Once); break;
      case 21: FxInstantEvent(fxc, fx_palette_dgm,     FxPaletteUpdateType::Once); break;
      case 22: FxInstantEvent(fxc, fx_palette_wgc,     FxPaletteUpdateType::Once); break;
      case 23: FxInstantEvent(fxc, fx_palette_wgb,     FxPaletteUpdateType::Once); break;
      case 24: FxInstantEvent(fxc, fx_palette_wgm,     FxPaletteUpdateType::Once); break;

      case 25: FxInstantEvent(fxc, fx_palette_dcb,     FxPaletteUpdateType::Once); break;
      case 26: FxInstantEvent(fxc, fx_palette_dcm,     FxPaletteUpdateType::Once); break;
      case 27: FxInstantEvent(fxc, fx_palette_wcb,     FxPaletteUpdateType::Once); break;
      case 28: FxInstantEvent(fxc, fx_palette_wcm,     FxPaletteUpdateType::Once); break;

      case 29: FxInstantEvent(fxc, fx_palette_dbm,     FxPaletteUpdateType::Once); break;
      case 30: FxInstantEvent(fxc, fx_palette_wbm,     FxPaletteUpdateType::Once); break;

      case 31: FxInstantEvent(fxc, fx_palette_rgb,     FxPaletteUpdateType::Once); break;
      case 32: FxInstantEvent(fxc, fx_palette_rbm,     FxPaletteUpdateType::Once); break;
      case 33: FxInstantEvent(fxc, fx_palette_cmy,     FxPaletteUpdateType::Once); break;
      case 34: FxInstantEvent(fxc, fx_palette_cbm,     FxPaletteUpdateType::Once); break;
                           
      case 101: FxInstantEvent(fxc, fx_palette_dr,     FxPaletteUpdateType::Once); break;
      case 102: FxInstantEvent(fxc, fx_palette_red,    FxPaletteUpdateType::Once); break;
      case 103: FxInstantEvent(fxc, fx_palette_ry,     FxPaletteUpdateType::Once); break;
      case 104: FxInstantEvent(fxc, fx_palette_rg,     FxPaletteUpdateType::Once); break;
      case 105: FxInstantEvent(fxc, fx_palette_rc,     FxPaletteUpdateType::Once); break;
      case 106: FxInstantEvent(fxc, fx_palette_rb,     FxPaletteUpdateType::Once); break;
      case 107: FxInstantEvent(fxc, fx_palette_rm,     FxPaletteUpdateType::Once); break;
      case 108: FxInstantEvent(fxc, fx_palette_wr,     FxPaletteUpdateType::Once); break;

      case 109: FxInstantEvent(fxc, fx_palette_dy,     FxPaletteUpdateType::Once); break;
      case 110: FxInstantEvent(fxc, fx_palette_yellow, FxPaletteUpdateType::Once); break;
      case 111: FxInstantEvent(fxc, fx_palette_yg,     FxPaletteUpdateType::Once); break;
      case 112: FxInstantEvent(fxc, fx_palette_yc,     FxPaletteUpdateType::Once); break;
      case 113: FxInstantEvent(fxc, fx_palette_yb,     FxPaletteUpdateType::Once); break;
      case 114: FxInstantEvent(fxc, fx_palette_ym,     FxPaletteUpdateType::Once); break;
      case 115: FxInstantEvent(fxc, fx_palette_wy,     FxPaletteUpdateType::Once); break;

      case 116: FxInstantEvent(fxc, fx_palette_dg,     FxPaletteUpdateType::Once); break;
      case 117: FxInstantEvent(fxc, fx_palette_green,  FxPaletteUpdateType::Once); break;
      case 118: FxInstantEvent(fxc, fx_palette_gc,     FxPaletteUpdateType::Once); break;
      case 119: FxInstantEvent(fxc, fx_palette_gb,     FxPaletteUpdateType::Once); break;
      case 120: FxInstantEvent(fxc, fx_palette_gm,     FxPaletteUpdateType::Once); break;
      case 121: FxInstantEvent(fxc, fx_palette_wg,     FxPaletteUpdateType::Once); break;

      case 122: FxInstantEvent(fxc, fx_palette_dc,     FxPaletteUpdateType::Once); break;
      case 123: FxInstantEvent(fxc, fx_palette_cyan,   FxPaletteUpdateType::Once); break;
      case 124: FxInstantEvent(fxc, fx_palette_cb,     FxPaletteUpdateType::Once); break;
      case 125: FxInstantEvent(fxc, fx_palette_cm,     FxPaletteUpdateType::Once); break;
      case 126: FxInstantEvent(fxc, fx_palette_wc,     FxPaletteUpdateType::Once); break;

      case 127: FxInstantEvent(fxc, fx_palette_db,     FxPaletteUpdateType::Once); break;
      case 128: FxInstantEvent(fxc, fx_palette_blue,   FxPaletteUpdateType::Once); break;
      case 129: FxInstantEvent(fxc, fx_palette_bm,     FxPaletteUpdateType::Once); break;
      case 130: FxInstantEvent(fxc, fx_palette_wb,     FxPaletteUpdateType::Once); break;

      case 131: FxInstantEvent(fxc, fx_palette_dm,     FxPaletteUpdateType::Once); break;
      case 132: FxInstantEvent(fxc, fx_palette_magenta,FxPaletteUpdateType::Once); break;
      case 133: FxInstantEvent(fxc, fx_palette_wm,     FxPaletteUpdateType::Once); break;

      case 150: FxInstantEvent(fxc, fx_palette_rbm,     FxPaletteUpdateType::Once); break;
      case 151: FxInstantEvent(fxc, fx_palette_rgb,     FxPaletteUpdateType::Once); break;
      case 152: FxInstantEvent(fxc, fx_palette_cmy,     FxPaletteUpdateType::Once); break;
      case 153: FxInstantEvent(fxc, fx_palette_cbm,     FxPaletteUpdateType::Once); break;

      case 201: FxInstantEvent(fxc, fx_palette_lava,          FxPaletteUpdateType::Once); break;
      case 202: FxInstantEvent(fxc, fx_palette_cloud,         FxPaletteUpdateType::Once); break;
      case 203: FxInstantEvent(fxc, fx_palette_ocean,         FxPaletteUpdateType::Once); break;
      case 204: FxInstantEvent(fxc, fx_palette_forest,        FxPaletteUpdateType::Once); break;
      case 205: FxInstantEvent(fxc, fx_palette_rainbow,       FxPaletteUpdateType::Once); break;
      case 206: FxInstantEvent(fxc, fx_palette_rainbowstripe, FxPaletteUpdateType::Once); break;
      case 207: FxInstantEvent(fxc, fx_palette_party,         FxPaletteUpdateType::Once); break;
      case 208: FxInstantEvent(fxc, fx_palette_heat,          FxPaletteUpdateType::Once); break;
    }
}

word FxGetSideFXTrackSize(FxController &fxc, int trackId)
{
  switch (trackId)
  {
    case 0: return sizeof(SideFxTrackCM);
    case 1: return sizeof(SideFxTrackUSA);
    case 2: return sizeof(SideFxTrackFirePit);
    case 3: return sizeof(SideFxTrackPurpleParty);
    case 4: return sizeof(SideFxTrackRainbow);
    case 5: return sizeof(SideFxTrackHeat);
    case 6: return sizeof(SideFxTrackCMY);
  }
  return 0;
}

word FxGetSideFXTrackValue(FxController &fxc, int trackId, int o)
{
  switch (trackId)
  {
    case 0: return pgm_read_dword(&(SideFxTrackCM[o]));
    case 1: return pgm_read_dword(&(SideFxTrackUSA[o]));
    case 2: return pgm_read_dword(&(SideFxTrackFirePit[o]));    
    case 3: return pgm_read_dword(&(SideFxTrackPurpleParty[o]));
    case 4: return pgm_read_dword(&(SideFxTrackRainbow[o]));
    case 5: return pgm_read_dword(&(SideFxTrackHeat[o]));
    case 6: return pgm_read_dword(&(SideFxTrackCMY[o]));
  }
  return 0;
}

void FxActivateSideFXTrack(FxController &fxc, int trackId)
{
  fxc.KillFX();
  for (int strip=0;strip<NUM_STRIPS;strip++)
    if (fxc.stripMask & (1<<strip)) 
      fxc.strip[strip]->paletteIndex = 0;
  
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {  
    for (int channel = 0;channel < NUM_FX_CHANNELS;channel++)
    {
      fxc.strip[strip]->fxSystem.channels[channel].on = false;
      fxc.strip[strip]->fxSystem.channels[channel].state = 0;
    }
  }

#if ENABLE_MULTISTRIP  
  int HeaderSize = 8+16;
  
  int len = FxGetSideFXTrackSize(fxc, trackId);
  if (len == 0)
   return;

  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    fxc.strip[strip]->paletteType  = (FxPaletteType)FxGetSideFXTrackValue(fxc, trackId, strip*2+0);
    fxc.strip[strip]->paletteSpeed = (int)FxGetSideFXTrackValue(fxc, trackId, strip*2+1);
  }
   
  word palId0 = FxGetSideFXTrackValue(fxc, trackId, 16);
  word palId1 = FxGetSideFXTrackValue(fxc, trackId, 17);
  word palId2 = FxGetSideFXTrackValue(fxc, trackId, 18);
  word palId3 = FxGetSideFXTrackValue(fxc, trackId, 19);
  word palId4 = FxGetSideFXTrackValue(fxc, trackId, 20);
  word palId5 = FxGetSideFXTrackValue(fxc, trackId, 21);
  word palId6 = FxGetSideFXTrackValue(fxc, trackId, 22);
  word palId7 = FxGetSideFXTrackValue(fxc, trackId, 23);

  fxc.stripMask = LEDS_0;FxPaletteById(fxc,palId0);
  fxc.stripMask = LEDS_1;FxPaletteById(fxc,palId1);
  fxc.stripMask = LEDS_2;FxPaletteById(fxc,palId2);
  fxc.stripMask = LEDS_3;FxPaletteById(fxc,palId3);
  fxc.stripMask = LEDS_4;FxPaletteById(fxc,palId4);
  fxc.stripMask = LEDS_5;FxPaletteById(fxc,palId5);
  fxc.stripMask = LEDS_6;FxPaletteById(fxc,palId6);
  fxc.stripMask = LEDS_7;FxPaletteById(fxc,palId7);

  int parts = (len/2-HeaderSize)/3;
  //Serial.println(F("Parts:"));
  //Serial.println(parts);
  for (int i=0;i<parts;i++)
  {
    int stripId   = FxGetSideFXTrackValue(fxc, trackId, HeaderSize+i*3+0);
    int channelId = FxGetSideFXTrackValue(fxc, trackId, HeaderSize+i*3+1);
    int stateId   = FxGetSideFXTrackValue(fxc, trackId, HeaderSize+i*3+2);
    /*Serial.print(stripId);
    Serial.print(F("/"));
    Serial.print(channelId);
    Serial.print(F("/"));
    Serial.print(stateId);
    Serial.print(F(" "));*/
    fxc.strip[stripId]->fxSystem.channels[channelId].on = true;
    fxc.strip[stripId]->fxSystem.channels[channelId].state = stateId;
  }
  //Serial.println();
  fxc.stripMask = LEDS_0|LEDS_1|LEDS_2|LEDS_3|LEDS_4|LEDS_5|LEDS_6|LEDS_7;
#endif  
}  

void FxDisplaySideFX(FxController &fxc)
{  
#if ENABLE_MULTISTRIP  
  Serial.println(F("const word SideFxTrack[] PROGMEM =\n{"));

  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    Serial.print(fxc.strip[strip]->paletteType);
    Serial.print(",");
    Serial.print(abs(fxc.strip[strip]->paletteSpeed));
    Serial.print(", ");
  }
  Serial.println();

  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    Serial.print(fxc.strip[strip]->paletteId);
    Serial.print(",");
  }
  Serial.println();    

  for (int strip=0;strip<NUM_STRIPS;strip++)
  {  
    int count = 0;
    for (int channel = 0;channel < NUM_FX_CHANNELS;channel++)
    {
      if (fxc.strip[strip]->fxSystem.channels[channel].on)
      {
        if (count++ > 0)
          Serial.print(F(", "));
        Serial.print(strip);
        Serial.print(F(","));
        Serial.print(channel);
        Serial.print(F(","));
        Serial.print(fxc.strip[strip]->fxSystem.channels[channel].state);
      }
    }
    if (strip < NUM_STRIPS-1)
      Serial.print(",");
    Serial.println();    
  }
  Serial.println(F("};"));
#endif  
}

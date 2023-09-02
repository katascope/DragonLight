/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "SideFX.h"
#include "Fx.h"
#include "FxCore.h"
#include "FxController.h"
#include "DevNeo.h"
#include <avr/pgmspace.h>

const word SideFx_Preset_CM[] PROGMEM =
{
0,7, 0,7, 0,7, 0,7, 0,7, 0,7, 0,7, 0,7,
125,125,125,125,125,125,125,125,
0,1,0, 0,4,0,
1,1,0, 1,4,0,
2,1,0, 2,4,0,
3,1,0, 3,4,0,
4,1,0, 4,4,0,
5,1,0, 5,4,0,
6,1,0, 6,4,0,
7,1,0, 7,4,0
};

const word SideFx_Preset_USA[] PROGMEM =
{
0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 
9,9,9,9,9,9,9,9,
0,1,0, 0,3,0, 0,4,0, 0,5,0, 0,7,0,
1,1,0, 1,3,0, 1,4,0, 1,5,0, 1,7,0,
2,1,0, 2,3,0, 2,4,0, 2,5,0, 2,7,0,
3,1,0, 3,3,0, 3,4,0, 3,5,0, 3,7,0,
4,1,0, 4,3,0, 4,4,0, 4,5,0, 4,7,0,
5,1,0, 5,3,0, 5,4,0, 5,5,0, 5,7,0,
6,1,0, 6,3,0, 6,4,0, 6,5,0, 6,7,0,
7,1,0, 7,3,0, 7,4,0, 7,5,0, 7,7,0
};

const word SideFx_Preset_FirePit[] PROGMEM =
{
0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 
208,208,208,208,208,208,208,208,
0,1,0, 0,2,1, 0,3,0,
1,1,0, 1,2,1, 1,3,0,
2,1,0, 2,2,1, 2,3,0,
3,1,0, 3,2,1, 3,3,0,
4,1,0, 4,2,1, 4,3,0,
5,1,0, 5,2,1, 5,3,0,
6,1,0, 6,2,1, 6,3,0,
7,1,0, 7,2,1, 7,3,0
};

const word SideFx_Preset_PurpleParty[] PROGMEM =
{
0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 
21,21,21,21,21,21,21,21,
0,1,0, 0,3,0, 0,4,0, 0,5,0, 0,7,0,
1,1,0, 1,3,0, 1,4,0, 1,5,0, 1,7,0,
2,1,0, 2,3,0, 2,4,0, 2,5,0, 2,7,0,
3,1,0, 3,3,0, 3,4,0, 3,5,0, 3,7,0,
4,1,0, 4,3,0, 4,4,0, 4,5,0, 4,7,0,
5,1,0, 5,3,0, 5,4,0, 5,5,0, 5,7,0,
6,1,0, 6,3,0, 6,4,0, 6,5,0, 6,7,0,
7,1,0, 7,3,0, 7,4,0, 7,5,0, 7,7,0
};

const word SideFx_Preset_Rainbow[] PROGMEM =
{
0,3, 0,3, 0,3, 0,3, 0,3, 0,3, 0,3, 0,3, 
205,205,205,205,205,205,205,205,
0,1,0, 0,5,0,
1,1,0, 1,5,0,
2,1,0, 2,5,0,
3,1,0, 3,5,0,
4,1,0, 4,5,0,
5,1,0, 5,5,0,
6,1,0, 6,5,0,
7,1,0, 7,5,0
};

const word SideFx_Preset_Heat[] PROGMEM =
{
0,2, 0,2, 0,2, 0,2, 0,2, 0,2, 0,2, 0,2, 
201,201,201,201,201,201,201,201,
0,1,0, 0,3,0, 0,4,1, 0,5,1,
1,1,0, 1,3,0, 1,4,1, 1,5,1,
2,1,0, 2,3,0, 2,4,1, 2,5,1,
3,1,0, 3,3,0, 3,4,1, 3,5,1,
4,1,0, 4,3,0, 4,4,1, 4,5,1,
5,1,0, 5,3,0, 5,4,1, 5,5,1,
6,1,0, 6,3,0, 6,4,1, 6,5,1,
7,1,0, 7,3,0, 7,4,1, 7,5,1
};

const word SideFx_Preset_CMY[] PROGMEM =
{
0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 
152,152,152,152,152,152,152,152,
0,1,0, 0,2,0, 0,3,0, 0,4,0, 0,5,0, 0,6,0, 0,7,0,
1,1,0, 1,2,0, 1,3,0, 1,4,0, 1,5,0, 1,6,0, 1,7,0,
2,1,0, 2,2,0, 2,3,0, 2,4,0, 2,5,0, 2,6,0, 2,7,0,
3,1,0, 3,2,0, 3,3,0, 3,4,0, 3,5,0, 3,6,0, 3,7,0,
4,1,0, 4,2,0, 4,3,0, 4,4,0, 4,5,0, 4,6,0, 4,7,0,
5,1,0, 5,2,0, 5,3,0, 5,4,0, 5,5,0, 5,6,0, 5,7,0,
6,1,0, 6,2,0, 6,3,0, 6,4,0, 6,5,0, 6,6,0, 6,7,0,
7,1,0, 7,2,0, 7,3,0, 7,4,0, 7,5,0, 7,6,0, 7,7,0
};


void DoPaletteMirror(FxController &fxc, int strip)
{  
  int height = fxc.strip[strip]->numleds;
  for (int i=0;i<height/2;i++)
  {
    fxc.strip[strip]->sideFXPalette[i] = fxc.strip[strip]->nextPalette[i*2];
    fxc.strip[strip]->sideFXPalette[height-1-i] = fxc.strip[strip]->nextPalette[i*2];
  }  
}

void DoParticles(FxController &fxc, int strip, int state)
{  
  float boost = fxc.vol*2;              

  for (int p=0;p<NUM_PARTICLES;p++)
  {
    if (state == 0)
      fxc.strip[strip]->particles[p].vel = abs(fxc.strip[strip]->particles[p].vel);
    else
    fxc.strip[strip]->particles[p].vel = -abs(fxc.strip[strip]->particles[p].vel);
    
    fxc.strip[strip]->particles[p].loc += fxc.strip[strip]->particles[p].vel*boost;
    if (fxc.strip[strip]->particles[p].loc >= fxc.strip[strip]->numleds)
      fxc.strip[strip]->particles[p].loc = 0 + fxc.strip[strip]->particles[p].vel;
    if (fxc.strip[strip]->particles[p].loc < 0)
    fxc.strip[strip]->particles[p].loc = fxc.strip[strip]->numleds - fxc.strip[strip]->particles[p].vel;

    int loc = fxc.strip[strip]->particles[p].loc;    
    fxc.strip[strip]->sideFXPalette[loc] = LEDRGB(255,255,255);
    for (int i=0;i<boost*2;i++)
    {
      if (i+loc < fxc.strip[strip]->numleds)
        fxc.strip[strip]->sideFXPalette[loc+i] = LEDRGB(255,255,255);      
    }
    
  }  
}

word FxGetSideFXTrackSize(FxController &fxc, int trackId)
{
  switch (trackId)
  {
    case 0: return sizeof(SideFx_Preset_CM);
    case 1: return sizeof(SideFx_Preset_USA);
    case 2: return sizeof(SideFx_Preset_FirePit);
    case 3: return sizeof(SideFx_Preset_PurpleParty);
    case 4: return sizeof(SideFx_Preset_Rainbow);
    case 5: return sizeof(SideFx_Preset_Heat);
    case 6: return sizeof(SideFx_Preset_CMY);
  }
  return 0;
}

word FxGetSideFXTrackValue(FxController &fxc, int trackId, int o)
{
  switch (trackId)
  {
    case 0: return pgm_read_dword(&(SideFx_Preset_CM[o]));
    case 1: return pgm_read_dword(&(SideFx_Preset_USA[o]));
    case 2: return pgm_read_dword(&(SideFx_Preset_FirePit[o]));    
    case 3: return pgm_read_dword(&(SideFx_Preset_PurpleParty[o]));
    case 4: return pgm_read_dword(&(SideFx_Preset_Rainbow[o]));
    case 5: return pgm_read_dword(&(SideFx_Preset_Heat[o]));
    case 6: return pgm_read_dword(&(SideFx_Preset_CMY[o]));
  }
  return 0;
}

void SideFXActivatePreset(FxController &fxc, int trackId)
{  
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
      fxc.strip[strip]->paletteIndex = 0;
      fxc.strip[strip]->paletteSpeed = 0;
      fxc.strip[strip]->paletteDirection = 1;
    }
  }
  
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

void SideFXPrintState(FxController &fxc)
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


void SideFXPollState(FxController &fxc)
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
        neopixelSetBrightness(strip,(int)(fxc.vol * fxc.brightness));
    else
        neopixelSetBrightness(strip, fxc.brightness);
    
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
    }

    if (fxc.strip[strip]->fxSystem.channels[5].on) 
    {
      int advance = (fxc.vol * fxc.strip[strip]->paletteSpeed) * (fxc.vol * fxc.strip[strip]->paletteSpeed);
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
    
    if (fxc.strip[strip]->fxSystem.channels[7].on) 
    {
      DoParticles(fxc, strip, fxc.strip[strip]->fxSystem.channels[7].state);
    }
  }

  if (fxc.transitionMux >= 1)
  {
    FxAnimatePalette(fxc,true);
  }
}

/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include <Arduino.h>
#include "Config.h"
#include "DevNeo.h"
#include "FxPalette.h"
#include "SideFX.h"
#include "FxController.h"


uint32_t LEDRGB(uint8_t r, uint8_t g, uint8_t b) { return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b; }
unsigned char lerp(float mux, unsigned char a, unsigned char b) { return (unsigned char)(a * (1.0 - mux) + b * mux); }
uint32_t LerpRGB(float t, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2) { return LEDRGB(lerp(t, r1, r2),lerp(t, g1, g2),lerp(t, b1, b2)); }

uint32_t LerpRGB(float t, uint32_t rgb1, uint32_t rgb2) {
  unsigned char r1 = (rgb1 >> 16) & 0xFF;
  unsigned char g1 = (rgb1 >> 8) & 0xFF;
  unsigned char b1 = (rgb1 >> 0) & 0xFF;
  unsigned char r2 = (rgb2 >> 16) & 0xFF;
  unsigned char g2 = (rgb2 >> 8) & 0xFF;
  unsigned char b2 = (rgb2 >> 0) & 0xFF;
  return LEDRGB(lerp(t, r1, r2),lerp(t, g1, g2),lerp(t, b1, b2));
}

void FxCreatePalette(FxController &fxController, int strip, uint32_t *pal16, unsigned int palSize)
{
  int numleds = fxController.strip[strip]->numleds;
  
  if (fxController.strip[strip]->paletteType == FxPaletteType::Smoothed)
  {
    if (fxController.strip[strip]->transitionType == Transition_Instant)
    {
      PaletteLerpFromMicroPalette(fxController.strip[strip]->palette, numleds, pal16, palSize);
      PaletteCopy(numleds, fxController.strip[strip]->nextPalette, fxController.strip[strip]->palette);
    }
    else 
    {
      fxController.transitionMux = 0.0f;
      PaletteLerpFromMicroPalette(fxController.strip[strip]->nextPalette, numleds, pal16, palSize);
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
      PaletteLiteralFromMicroPalette(fxController.strip[strip]->palette, numleds, pal16, palSize,steps);
      PaletteCopy(numleds, fxController.strip[strip]->nextPalette, fxController.strip[strip]->palette);
    }
    else 
    {
      fxController.transitionMux = 0.0f;
      PaletteLiteralFromMicroPalette(fxController.strip[strip]->nextPalette, numleds, pal16, palSize,steps);      
    }
  }  
  PaletteCopy(numleds, fxController.strip[strip]->initialPalette, fxController.strip[strip]->palette);
}

void PaletteLerpFromMicroPalette(uint32_t *palette, unsigned int paletteSize, uint32_t* microPalette, unsigned int microPaletteSize)
{
    if (microPaletteSize == 0) //Nothing TO do..
        return;

    float stepSize = (float)paletteSize / (float)microPaletteSize;
    for (unsigned int phase = 0; phase < microPaletteSize; phase++)
    {
        unsigned int offset = (unsigned int)((float)(phase) * (float)stepSize);
        unsigned int offsetNext = (unsigned int)((float)(phase+1) * (float)stepSize) ;

        unsigned long to = 0;
        if (phase == microPaletteSize-1) //last phase, so loop back to 0 
            to = microPalette[0]; 
        else
            to = microPalette[phase + 1];

        unsigned int len = offsetNext - offset;
        for (unsigned int i = 0; i < len; i++)
        {
            float mux = (float)i / (float)(len-1);
            unsigned int palIndex = offset + i;
            unsigned long val = LerpRGB(mux, microPalette[phase], to);
            palette[palIndex] = val;
        }
    }
}

void PaletteLiteralFromMicroPalette(uint32_t *palette, unsigned int paletteSize, uint32_t* microPalette, unsigned int microPaletteSize, int steps)
{
    if (microPaletteSize == 0) //Nothing TO do..
        return;

    float srcOffset = 0.0f;
    for (unsigned int dstOffset = 0; dstOffset < paletteSize; dstOffset++)
    {
      if (srcOffset >= microPaletteSize)
        srcOffset = 0.0f;
      palette[dstOffset] = microPalette[(int)srcOffset];
      switch (steps)
      {
        case 1: srcOffset += 1.0f;break;
        case 2: srcOffset += 0.5f;break;
        case 3: srcOffset += 0.25f;break;
        case 4: srcOffset += 0.125f;break;
      }
    }
}

void PaletteCopy(int numleds, uint32_t *dst, uint32_t *src)
{
  for (int i=0;i<numleds;i++)
    dst[i] = src[i];
}

void PaletteCreateSingleColor(FxController &fxc, uint8_t r, uint8_t g, uint8_t b)
{  
  uint32_t palette[16];
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    if (fxc.stripMask & (1<<strip)) 
    {
       for (int i=0;i<16;i++)
         palette[i] = LEDRGB(r,g,b);
       FxCreatePalette(fxc, strip, palette,16);
    }
  }
}

void PaletteCreateDoubleColor(FxController &fxc, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2 )
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

void PaletteCreateQuadColor(FxController &fxc, 
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

void PaletteCreate16Color(FxController &fxc, uint32_t b0,uint32_t b1,uint32_t b2,uint32_t b3, uint32_t b4,uint32_t b5,uint32_t b6,uint32_t b7,
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

void PaletteCreateSinglePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b) {
  PaletteCreate16Color(fxc, LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0));
}

void PaletteCreateDoublePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b) {
  PaletteCreate16Color(fxc, LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0));
}

void PaletteCreateTriplePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b) {
  PaletteCreate16Color(fxc,  LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0),LEDRGB(0,0,0));
}

void PaletteCreateQuadPulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b) {
  PaletteCreate16Color(fxc,  LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0),
                      LEDRGB(r/2,g/2,b/2),LEDRGB(r,g,b),LEDRGB(r/2,g/2,b/2),LEDRGB(0,0,0));
}

void PaletteCreateById(FxController &fxc, int paletteId)
{
  //Record last set paletteId
  for (int strip=0;strip<NUM_STRIPS;strip++)
    if (fxc.stripMask & (1<<strip)) 
      fxc.strip[strip]->paletteId = paletteId;    
  
  switch (paletteId)
  {
    case 1:  FxEvent(fxc, fx_palette_dry); break;
    case 2:  FxEvent(fxc, fx_palette_drg); break;
    case 3:  FxEvent(fxc, fx_palette_drc); break;
    case 4:  FxEvent(fxc, fx_palette_drb); break;
    case 5:  FxEvent(fxc, fx_palette_drm); break;
    case 6:  FxEvent(fxc, fx_palette_wry); break;
    case 7:  FxEvent(fxc, fx_palette_wrg); break;
    case 8:  FxEvent(fxc, fx_palette_wrc); break;
    case 9:  FxEvent(fxc, fx_palette_wrb); break;
    case 10: FxEvent(fxc, fx_palette_wrm); break;

    case 11: FxEvent(fxc, fx_palette_dyg); break;
    case 12: FxEvent(fxc, fx_palette_dyc); break;
    case 13: FxEvent(fxc, fx_palette_dyb); break;
    case 14: FxEvent(fxc, fx_palette_dym); break;
    case 15: FxEvent(fxc, fx_palette_wyg); break;
    case 16: FxEvent(fxc, fx_palette_wyc); break;
    case 17: FxEvent(fxc, fx_palette_wyb); break;
    case 18: FxEvent(fxc, fx_palette_wym); break;

    case 19: FxEvent(fxc, fx_palette_dgc); break;
    case 20: FxEvent(fxc, fx_palette_dgb); break;
    case 21: FxEvent(fxc, fx_palette_dgm); break;
    case 22: FxEvent(fxc, fx_palette_wgc); break;
    case 23: FxEvent(fxc, fx_palette_wgb); break;
    case 24: FxEvent(fxc, fx_palette_wgm); break;

    case 25: FxEvent(fxc, fx_palette_dcb); break;
    case 26: FxEvent(fxc, fx_palette_dcm); break;
    case 27: FxEvent(fxc, fx_palette_wcb); break;
    case 28: FxEvent(fxc, fx_palette_wcm); break;

    case 29: FxEvent(fxc, fx_palette_dbm); break;
    case 30: FxEvent(fxc, fx_palette_wbm); break;

    case 31: FxEvent(fxc, fx_palette_rgb); break;
    case 32: FxEvent(fxc, fx_palette_rbm); break;
    case 33: FxEvent(fxc, fx_palette_cmy); break;
    case 34: FxEvent(fxc, fx_palette_cbm); break;

    case 50:  FxEvent(fxc, fx_palette_dark); break;
    case 51:  FxEvent(fxc, fx_palette_white); break;

    case 101: FxEvent(fxc, fx_palette_dr); break;
    case 102: FxEvent(fxc, fx_palette_red); break;
    case 103: FxEvent(fxc, fx_palette_ry); break;
    case 104: FxEvent(fxc, fx_palette_rg); break;
    case 105: FxEvent(fxc, fx_palette_rc); break;
    case 106: FxEvent(fxc, fx_palette_rb); break;
    case 107: FxEvent(fxc, fx_palette_rm); break;
    case 108: FxEvent(fxc, fx_palette_wr); break;

    case 109: FxEvent(fxc, fx_palette_dy); break;
    case 110: FxEvent(fxc, fx_palette_yellow); break;
    case 111: FxEvent(fxc, fx_palette_yg); break;
    case 112: FxEvent(fxc, fx_palette_yc); break;
    case 113: FxEvent(fxc, fx_palette_yb); break;
    case 114: FxEvent(fxc, fx_palette_ym); break;
    case 115: FxEvent(fxc, fx_palette_wy); break;

    case 116: FxEvent(fxc, fx_palette_dg); break;
    case 117: FxEvent(fxc, fx_palette_green); break;
    case 118: FxEvent(fxc, fx_palette_gc); break;
    case 119: FxEvent(fxc, fx_palette_gb); break;
    case 120: FxEvent(fxc, fx_palette_gm); break;
    case 121: FxEvent(fxc, fx_palette_wg); break;

    case 122: FxEvent(fxc, fx_palette_dc); break;
    case 123: FxEvent(fxc, fx_palette_cyan); break;
    case 124: FxEvent(fxc, fx_palette_cb); break;
    case 125: FxEvent(fxc, fx_palette_cm); break;
    case 126: FxEvent(fxc, fx_palette_wc); break;

    case 127: FxEvent(fxc, fx_palette_db); break;
    case 128: FxEvent(fxc, fx_palette_blue); break;
    case 129: FxEvent(fxc, fx_palette_bm); break;
    case 130: FxEvent(fxc, fx_palette_wb); break;

    case 131: FxEvent(fxc, fx_palette_dm); break;
    case 132: FxEvent(fxc, fx_palette_magenta); break;
    case 133: FxEvent(fxc, fx_palette_wm); break;

    case 150: FxEvent(fxc, fx_palette_rbm); break;
    case 151: FxEvent(fxc, fx_palette_rgb); break;
    case 152: FxEvent(fxc, fx_palette_cmy); break;
    case 153: FxEvent(fxc, fx_palette_cbm); break;

    case 201: FxEvent(fxc, fx_palette_lava); break;
    case 202: FxEvent(fxc, fx_palette_cloud); break;
    case 203: FxEvent(fxc, fx_palette_ocean); break;
    case 204: FxEvent(fxc, fx_palette_forest); break;
    case 205: FxEvent(fxc, fx_palette_rainbow); break;
    case 206: FxEvent(fxc, fx_palette_rainbowstripe); break;
    case 207: FxEvent(fxc, fx_palette_party); break;
    case 208: FxEvent(fxc, fx_palette_heat); break;
  }
}

void FxUpdatePalette(struct FxController &fxc)
{
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
#if ENABLE_NEOPIXEL
      neopixelSetSequencePalette(strip, fxc.strip[strip]->numleds, fxc.strip[strip]->palette, fxc.strip[strip]->paletteIndex, fxc.strip[strip]->sequence);
#endif    
  }
}

void FxAnimateStrip(struct FxController &fxc, int strip)
{
  fxc.strip[strip]->paletteIndex = fxc.strip[strip]->paletteIndex + (fxc.strip[strip]->paletteSpeed * fxc.strip[strip]->paletteDirection);
  if (fxc.strip[strip]->paletteIndex >= fxc.strip[strip]->numleds)
    fxc.strip[strip]->paletteIndex -= fxc.strip[strip]->numleds;
  if (fxc.strip[strip]->paletteIndex < 0)
    fxc.strip[strip]->paletteIndex = fxc.strip[strip]->numleds - 1;
}

void PalettePrint(FxController &fxc)
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

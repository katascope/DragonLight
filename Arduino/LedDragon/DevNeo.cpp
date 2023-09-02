/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "Fx.h"

/////////////////////////////////////////////////////////////////////////
//WS2811 - led strip lights

#if ENABLE_NEOPIXEL
#include <Adafruit_NeoPixel.h>
static Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(NUM_LEDS_0, LED_PIN+0, NEO_GRB + NEO_KHZ800);
#if ENABLE_MULTISTRIP
static Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS_1, LED_PIN+1, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS_2, LED_PIN+2, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS_3, LED_PIN+3, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_LEDS_4, LED_PIN+4, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(NUM_LEDS_5, LED_PIN+5, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(NUM_LEDS_6, LED_PIN+6, NEO_GRB + NEO_KHZ800);
static Adafruit_NeoPixel strip7 = Adafruit_NeoPixel(NUM_LEDS_7, LED_PIN+7, NEO_GRB + NEO_KHZ800);
#endif
void neopixelSetup()
{
  strip0.begin();
  strip0.setBrightness(BRIGHTNESS);
#if ENABLE_MULTISTRIP  
  strip1.begin();
  strip1.setBrightness(BRIGHTNESS);
  strip2.begin();
  strip2.setBrightness(BRIGHTNESS);
  strip3.begin();
  strip3.setBrightness(BRIGHTNESS);
  strip4.begin();
  strip4.setBrightness(BRIGHTNESS);
  strip5.begin();
  strip5.setBrightness(BRIGHTNESS);
  strip6.begin();
  strip6.setBrightness(BRIGHTNESS);
  strip7.begin();
  strip7.setBrightness(BRIGHTNESS);
#endif  
}
void neopixelSetBrightness(int strip, unsigned char brightness)
{
  if (brightness > BRIGHTNESS_LIMIT)
    brightness = BRIGHTNESS_LIMIT;
    
  switch (strip)
  {
    case 0:
      strip0.setBrightness(brightness);
      strip0.show();
    break;
#if ENABLE_MULTISTRIP  
    case 1:
      strip1.setBrightness(brightness);
      strip1.show();
    break;
    case 2:
      strip2.setBrightness(brightness);
      strip2.show();
    break;
    case 3:
      strip3.setBrightness(brightness);
      strip3.show();
    break;
    case 4:
      strip4.setBrightness(brightness);
      strip4.show();
    break;
    case 5:
      strip5.setBrightness(brightness);
      strip5.show();
    break;
    case 6:
      strip6.setBrightness(brightness);
      strip6.show();
    break;
    case 7:
      strip7.setBrightness(brightness);
      strip7.show();
    break;
#endif  
  }
}

void neopixelSetPixel(int slot, int offset, uint32_t rgb)
{
    switch (slot)
    {
      case 0: strip0.setPixelColor(offset, rgb); break;
#if ENABLE_MULTISTRIP  
      case 1: strip1.setPixelColor(offset, rgb); break;
      case 2: strip2.setPixelColor(offset, rgb); break;
      case 3: strip3.setPixelColor(offset, rgb); break;
      case 4: strip4.setPixelColor(offset, rgb); break;
      case 5: strip5.setPixelColor(offset, rgb); break;
      case 6: strip6.setPixelColor(offset, rgb); break;
      case 7: strip7.setPixelColor(offset, rgb); break;
#endif      
    }
}

void neopixelSetPalette(int slot, int numleds, uint32_t *palette, int paletteIndex)
{  
  uint32_t offset = paletteIndex;
  unsigned char r,g,b;
  unsigned char h,s,v;
  for(uint16_t i=0; i<numleds; i++)
  {
    if (offset >= numleds)
     offset=0;    
    uint32_t rgb = palette[i];
    r = (rgb >> 16) & 0xFF;
    g = (rgb >> 8) & 0xFF;
    b = (rgb >> 0) & 0xFF;
    neopixelSetPixel(slot,offset,rgb);
    offset++;    
  }
  
    switch (slot)
    {
      case 0: strip0.show();break;
#if ENABLE_MULTISTRIP  
      case 1: strip1.show();break;
      case 2: strip2.show();break;
      case 3: strip3.show();break;
      case 4: strip4.show();break;
      case 5: strip5.show();break;
      case 6: strip6.show();break;
      case 7: strip7.show();break;
#endif  
    }
}
#endif

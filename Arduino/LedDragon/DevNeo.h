/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef DEVICE_NEOPIXEL_DEF
#define DEVICE_NEOPIXEL_DEF

//////////////// NeoPixel Section ////////////////
#if ENABLE_NEOPIXEL
#include <Adafruit_NeoPixel.h>
void neopixelSetup();
void neopixelSetBrightness(int strip, unsigned char brightness);
void neopixelSetPalette(int slot, int numleds , uint32_t *palette, int paletteIndex);
void neopixelSetPixel(int slot, int offset, uint32_t rgb);
#endif
//////////////// NeoPixel Section ////////////////

#endif

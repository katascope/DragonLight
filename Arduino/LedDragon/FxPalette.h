/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef FX_PALETTE_DEF
#define FX_PALETTE_DEF
#include <Arduino.h>
#include "Config.h"
#include "FxEvent.h"
#include "FxController.h"

struct FxController;

#if ENABLE_SAFETY
#define DARK    0x00,0x00,0x00
#define WHITE   0x5F,0x5F,0x5F
#define RED     0xFF,0x00,0x00
#define YELLOW  0x9F,0x9F,0x00
#define GREEN   0x00,0xFF,0x00
#define CYAN    0x00,0x9F,0x9F
#define BLUE    0x00,0x00,0xFF
#define MAGENTA 0x9F,0x00,0x9F
#define ORANGE  0xFF,0x7F,0x00
#define HALF    0x3F,0x3F,0x3F
#define LOWHALF 0x1F,0x1F,0x1F
#else
#define DARK    0x00,0x00,0x00
#define WHITE   0x7F,0x7F,0x7F
#define RED     0xFF,0x00,0x00
#define YELLOW  0xFF,0xFF,0x00
#define GREEN   0x00,0xFF,0x00
#define CYAN    0x00,0xFF,0xFF
#define BLUE    0x00,0x00,0xFF
#define MAGENTA 0x7F,0x00,0xFF
#define ORANGE  0xFF,0x7F,0x00
#define HALF    0x7F,0x7F,0x7F
#define LOWHALF 0x4F,0x4F,0x4F
#endif

#define CRGB_DARK    LEDRGB(DARK)
#define CRGB_WHITE   LEDRGB(WHITE)
#define CRGB_RED     LEDRGB(RED)
#define CRGB_YELLOW  LEDRGB(YELLOW)
#define CRGB_GREEN   LEDRGB(GREEN)
#define CRGB_CYAN    LEDRGB(CYAN)
#define CRGB_BLUE    LEDRGB(BLUE)
#define CRGB_MAGENTA LEDRGB(MAGENTA)
#define CRGB_ORANGE  LEDRGB(ORANGE)
#define CRGB_HALF    LEDRGB(HALF)
#define CRGB_LOWHALF LEDRGB(LOWHALF)

enum    WEBRGB {
  AliceBlue = 0xF0F8FF, Amethyst = 0x9966CC, AntiqueWhite = 0xFAEBD7, Aqua = 0x00FFFF,
  Aquamarine = 0x7FFFD4, Azure = 0xF0FFFF, Beige = 0xF5F5DC, Bisque = 0xFFE4C4,
  Black = 0x000000, BlanchedAlmond = 0xFFEBCD, Blue = 0x0000FF, BlueViolet = 0x8A2BE2,
  Brown = 0xA52A2A, BurlyWood = 0xDEB887, CadetBlue = 0x5F9EA0, Chartreuse = 0x7FFF00,
  Chocolate = 0xD2691E, Coral = 0xFF7F50, CornflowerBlue = 0x6495ED, Cornsilk = 0xFFF8DC,
  Crimson = 0xDC143C, Cyan = 0x00FFFF, DarkBlue = 0x00008B, DarkCyan = 0x008B8B,
  DarkGoldenrod = 0xB8860B, DarkGray = 0xA9A9A9, DarkGrey = 0xA9A9A9, DarkGreen = 0x006400,
  DarkKhaki = 0xBDB76B, DarkMagenta = 0x8B008B, DarkOliveGreen = 0x556B2F, DarkOrange = 0xFF8C00,
  DarkOrchid = 0x9932CC, DarkRed = 0x8B0000, DarkSalmon = 0xE9967A, DarkSeaGreen = 0x8FBC8F,
  DarkSlateBlue = 0x483D8B, DarkSlateGray = 0x2F4F4F, DarkSlateGrey = 0x2F4F4F, DarkTurquoise = 0x00CED1,
  DarkViolet = 0x9400D3, DeepPink = 0xFF1493, DeepSkyBlue = 0x00BFFF, DimGray = 0x696969,
  DimGrey = 0x696969, DodgerBlue = 0x1E90FF, FireBrick = 0xB22222, FloralWhite = 0xFFFAF0,
  ForestGreen = 0x228B22, Fuchsia = 0xFF00FF, Gainsboro = 0xDCDCDC, GhostWhite = 0xF8F8FF,
  Gold = 0xFFD700, Goldenrod = 0xDAA520, Gray = 0x808080, Grey = 0x808080,
  Green = 0x008000, GreenYellow = 0xADFF2F, Honeydew = 0xF0FFF0, HotPink = 0xFF69B4,
  IndianRed = 0xCD5C5C, Indigo = 0x4B0082, Ivory = 0xFFFFF0, Khaki = 0xF0E68C,
  Lavender = 0xE6E6FA, LavenderBlush = 0xFFF0F5, LawnGreen = 0x7CFC00, LemonChiffon = 0xFFFACD,
  LightBlue = 0xADD8E6, LightCoral = 0xF08080, LightCyan = 0xE0FFFF, LightGoldenrodYellow = 0xFAFAD2,
  LightGreen = 0x90EE90, LightGrey = 0xD3D3D3, LightPink = 0xFFB6C1, LightSalmon = 0xFFA07A,
  LightSeaGreen = 0x20B2AA, LightSkyBlue = 0x87CEFA, LightSlateGray = 0x778899, LightSlateGrey = 0x778899,
  LightSteelBlue = 0xB0C4DE, LightYellow = 0xFFFFE0, Lime = 0x00FF00, LimeGreen = 0x32CD32,
  Linen = 0xFAF0E6, Magenta = 0xFF00FF, Maroon = 0x800000, MediumAquamarine = 0x66CDAA,
  MediumBlue = 0x0000CD, MediumOrchid = 0xBA55D3, MediumPurple = 0x9370DB, MediumSeaGreen = 0x3CB371,
  MediumSlateBlue = 0x7B68EE, MediumSpringGreen = 0x00FA9A, MediumTurquoise = 0x48D1CC, MediumVioletRed = 0xC71585,
  MidnightBlue = 0x191970, MintCream = 0xF5FFFA, MistyRose = 0xFFE4E1, Moccasin = 0xFFE4B5,
  NavajoWhite = 0xFFDEAD, Navy = 0x000080, OldLace = 0xFDF5E6, Olive = 0x808000,
  OliveDrab = 0x6B8E23, Orange = 0xFFA500, OrangeRed = 0xFF4500, Orchid = 0xDA70D6,
  PaleGoldenrod = 0xEEE8AA, PaleGreen = 0x98FB98, PaleTurquoise = 0xAFEEEE, PaleVioletRed = 0xDB7093,
  PapayaWhip = 0xFFEFD5, PeachPuff = 0xFFDAB9, Peru = 0xCD853F, Pink = 0xFFC0CB,
  Plaid = 0xCC5533, Plum = 0xDDA0DD, PowderBlue = 0xB0E0E6, Purple = 0x800080,
  Red = 0xFF0000, RosyBrown = 0xBC8F8F, RoyalBlue = 0x4169E1, SaddleBrown = 0x8B4513,
  Salmon = 0xFA8072, SandyBrown = 0xF4A460, SeaGreen = 0x2E8B57, Seashell = 0xFFF5EE,
  Sienna = 0xA0522D, Silver = 0xC0C0C0, SkyBlue = 0x87CEEB, SlateBlue = 0x6A5ACD,
  SlateGray = 0x708090, SlateGrey = 0x708090, Snow = 0xFFFAFA, SpringGreen = 0x00FF7F,
  SteelBlue = 0x4682B4, Tan = 0xD2B48C, Teal = 0x008080, Thistle = 0xD8BFD8,
  Tomato = 0xFF6347, Turquoise = 0x40E0D0, Violet = 0xEE82EE, Wheat = 0xF5DEB3,
  White = 0xFFFFFF, WhiteSmoke = 0xF5F5F5, Yellow = 0xFFFF00, YellowGreen = 0x9ACD32,
  FairyLight = 0xFFE42D, FairyLightNCC = 0xFF9D2A
};

uint32_t LEDRGB(uint8_t r, uint8_t g, uint8_t b);
unsigned char lerp(float mux, unsigned char a, unsigned char b);
uint32_t LerpRGB(float t, unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2);
uint32_t LerpRGB(float t, uint32_t rgb1, uint32_t rgb2);

void PaletteCopy(int numleds, uint32_t *dst, uint32_t *src);
void PaletteLerpFromMicroPalette(uint32_t *palette, unsigned int paletteSize, uint32_t* microPalette, unsigned int microPaletteSize);
void PaletteLiteralFromMicroPalette(uint32_t *palette, unsigned int paletteSize, uint32_t* microPalette, unsigned int microPaletteSize, int steps);

void PaletteCreateSingleColor(FxController &fxc, uint8_t r, uint8_t g, uint8_t b);
void PaletteCreateDoubleColor(FxController &fxc, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2 );
void PaletteCreateQuadColor(FxController &fxc, byte r1, byte g1, byte b1, byte r2, byte g2, byte b2,byte r3, byte g3, byte b3,byte r4, byte g4, byte b4);
void PaletteCreate16Color(FxController &fxc, uint32_t b0,uint32_t b1,uint32_t b2,uint32_t b3, uint32_t b4,uint32_t b5,uint32_t b6,uint32_t b7,uint32_t b8,uint32_t b9,uint32_t b10,uint32_t b11, uint32_t b12,uint32_t b13,uint32_t b14,uint32_t b15);
void PaletteCreateSinglePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b);
void PaletteCreateDoublePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b);
void PaletteCreateTriplePulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b);
void PaletteCreateQuadPulseBand(FxController &fxc, uint8_t r, uint8_t g, uint8_t b);
void PaletteCreateById(FxController &fxc, int paletteId);

void FxUpdatePalette(struct FxController &fxc);
void FxAnimateStrip(struct FxController &fxc, int strip);

void PalettePrint(FxController &fxc);

#endif

/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "Cmd.h"
#include "Fx.h"
#include "Track.h"
#include "Devices.h"

void UpdatePalette(FxController &fxc);
unsigned long GetTime();
void ComplexUserCommandInput(FxController &fxc, String data);

void UserCommandExecute(FxController &fxc, int cmd)
{
  switch (cmd)
  {
    case Cmd_State_Default:  fxc.fxState = FxState_Default;break;
    case Cmd_State_Test:     fxc.fxState = FxState_TestPattern;break;
    case Cmd_State_MultiTest:fxc.fxState = FxState_MultiTestPattern;break;
      
    case Cmd_PlayFromStart: trackStart(fxc, 0, (unsigned long)(millis() - (signed long)TRACK_START_DELAY), FxTrackEndAction::LoopAtEnd); break;
    //case Cmd_PlayFromStart: trackStart(fxc, 0, (unsigned long)(millis() - (signed long)TRACK_START_DELAY), FxTrackEndAction::StopAtEnd); break;
    case Cmd_PlayFrom:      fxc.fxState = FxState_PlayingTrack;break;
    case Cmd_PlayStop:      trackStop(fxc); break;

    case Cmd_SpeedNeg:      FxInstantEvent(fxc, fx_speed_neg, FxPaletteUpdateType::Once); break;
    case Cmd_SpeedPos:      FxInstantEvent(fxc, fx_speed_pos, FxPaletteUpdateType::Once); break;
    case Cmd_SpeedDec:      FxInstantEvent(fxc, fx_speed_dec, FxPaletteUpdateType::Once); break;
    case Cmd_SpeedInc:      FxInstantEvent(fxc, fx_speed_inc, FxPaletteUpdateType::Once); break;
    case Cmd_SpeedRst:      FxInstantEvent(fxc, fx_speed_0,   FxPaletteUpdateType::Once); break;

    case Cmd_Speed1:        FxInstantEvent(fxc, fx_speed_1,   FxPaletteUpdateType::Once); break;
    case Cmd_Speed2:        FxInstantEvent(fxc, fx_speed_2,   FxPaletteUpdateType::Once); break;
    case Cmd_Speed3:        FxInstantEvent(fxc, fx_speed_3,   FxPaletteUpdateType::Once); break;
    case Cmd_Speed4:        FxInstantEvent(fxc, fx_speed_4,   FxPaletteUpdateType::Once); break;
    
    case Cmd_ColorDark:     FxInstantEvent(fxc, fx_palette_dark,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorWhite:    FxInstantEvent(fxc, fx_palette_white,    FxPaletteUpdateType::Once); break;
    case Cmd_ColorRed:      FxInstantEvent(fxc, fx_palette_red,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorYellow:   FxInstantEvent(fxc, fx_palette_yellow,   FxPaletteUpdateType::Once); break;
    case Cmd_ColorGreen:    FxInstantEvent(fxc, fx_palette_green,    FxPaletteUpdateType::Once); break;
    case Cmd_ColorCyan:     FxInstantEvent(fxc, fx_palette_cyan,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorBlue:     FxInstantEvent(fxc, fx_palette_blue,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorMagenta:  FxInstantEvent(fxc, fx_palette_magenta,  FxPaletteUpdateType::Once); break;
    case Cmd_ColorOrange:   FxInstantEvent(fxc, fx_palette_orange,   FxPaletteUpdateType::Once); break;
    case Cmd_ColorHalf:     FxInstantEvent(fxc, fx_palette_half,     FxPaletteUpdateType::Once); break;

    case Cmd_ColorWhiteMagenta:    FxInstantEvent(fxc, fx_palette_wm,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorWhiteCyan:       FxInstantEvent(fxc, fx_palette_wc,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorWhiteYellow:     FxInstantEvent(fxc, fx_palette_wy,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorWhiteBlue:       FxInstantEvent(fxc, fx_palette_wb,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorRedBlue:         FxInstantEvent(fxc, fx_palette_rb,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorCyanMagenta:     FxInstantEvent(fxc, fx_palette_cm,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorCyanBlue:        FxInstantEvent(fxc, fx_palette_cb,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorBlueMagenta:     FxInstantEvent(fxc, fx_palette_bm,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorGreenMagenta:    FxInstantEvent(fxc, fx_palette_gm,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorDarkRedBlue:     FxInstantEvent(fxc, fx_palette_drb,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorDarkCyanMagenta: FxInstantEvent(fxc, fx_palette_dcm,     FxPaletteUpdateType::Once); break;    

    case Cmd_ColorPulseDark:      FxInstantEvent(fxc, fx_palette_pulse_dark,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseWhite:     FxInstantEvent(fxc, fx_palette_pulse_white,    FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseRed:       FxInstantEvent(fxc, fx_palette_pulse_red,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseYellow:    FxInstantEvent(fxc, fx_palette_pulse_yellow,   FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseGreen:     FxInstantEvent(fxc, fx_palette_pulse_green,    FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseCyan:      FxInstantEvent(fxc, fx_palette_pulse_cyan,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseBlue:      FxInstantEvent(fxc, fx_palette_pulse_blue,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseMagenta:   FxInstantEvent(fxc, fx_palette_pulse_magenta,  FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseOrange:    FxInstantEvent(fxc, fx_palette_pulse_orange,   FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulseHalf:      FxInstantEvent(fxc, fx_palette_pulse_half,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Dark:     FxInstantEvent(fxc, fx_palette_pulse2_dark,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2White:    FxInstantEvent(fxc, fx_palette_pulse2_white,    FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Red:      FxInstantEvent(fxc, fx_palette_pulse2_red,      FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Yellow:   FxInstantEvent(fxc, fx_palette_pulse2_yellow,   FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Green:    FxInstantEvent(fxc, fx_palette_pulse2_green,    FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Cyan:     FxInstantEvent(fxc, fx_palette_pulse2_cyan,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Blue:     FxInstantEvent(fxc, fx_palette_pulse2_blue,     FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Magenta:  FxInstantEvent(fxc, fx_palette_pulse2_magenta,  FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Orange:   FxInstantEvent(fxc, fx_palette_pulse2_orange,   FxPaletteUpdateType::Once); break;
    case Cmd_ColorPulse2Half:     FxInstantEvent(fxc, fx_palette_pulse2_half,   FxPaletteUpdateType::Once); break;
       
    case Cmd_ColorLava:           FxInstantEvent(fxc, fx_palette_lava,          FxPaletteUpdateType::Once); break;
    case Cmd_ColorCloud:          FxInstantEvent(fxc, fx_palette_cloud,         FxPaletteUpdateType::Once); break;
    case Cmd_ColorOcean:          FxInstantEvent(fxc, fx_palette_ocean,         FxPaletteUpdateType::Once); break;
    case Cmd_ColorForest:         FxInstantEvent(fxc, fx_palette_forest,        FxPaletteUpdateType::Once); break;
    case Cmd_ColorRainbow:        FxInstantEvent(fxc, fx_palette_rainbow,       FxPaletteUpdateType::Once); break;
    case Cmd_ColorRainbowstripe:  FxInstantEvent(fxc, fx_palette_rainbowstripe, FxPaletteUpdateType::Once); break;
    case Cmd_ColorParty:          FxInstantEvent(fxc, fx_palette_party,         FxPaletteUpdateType::Once); break;
    case Cmd_ColorHeat:           FxInstantEvent(fxc, fx_palette_heat,          FxPaletteUpdateType::Once); break;

    case Cmd_StripAll:            FxInstantEvent(fxc, fx_strip_all,     FxPaletteUpdateType::Once); break;
    case Cmd_StripNone:           FxInstantEvent(fxc, fx_strip_none,    FxPaletteUpdateType::Once); break;
    case Cmd_StripOdds:           FxInstantEvent(fxc, fx_strip_odds,    FxPaletteUpdateType::Once); break;
    case Cmd_StripEvens:          FxInstantEvent(fxc, fx_strip_evens,   FxPaletteUpdateType::Once); break;
    case Cmd_Strip0: fxc.stripMask = LEDS_0; break;
    case Cmd_Strip1: fxc.stripMask = LEDS_1; break;
    case Cmd_Strip2: fxc.stripMask = LEDS_2; break;
    case Cmd_Strip3: fxc.stripMask = LEDS_3; break;
    case Cmd_Strip4: fxc.stripMask = LEDS_4; break;
    case Cmd_Strip5: fxc.stripMask = LEDS_5; break;
    case Cmd_Strip6: fxc.stripMask = LEDS_6; break;
    case Cmd_Strip7: fxc.stripMask = LEDS_7; break;

    case Cmd_Particles_On:  fxc.SetParticlesRunning(true);break;
    case Cmd_Particles_Off: fxc.SetParticlesRunning(false);break;

    case Cmd_PalTypeSmoothed: FxInstantEvent(fxc, fx_palette_type_smoothed, FxPaletteUpdateType::Once); break;
    case Cmd_PalTypeLiteral:  FxInstantEvent(fxc, fx_palette_type_literal,  FxPaletteUpdateType::Once); break;

    case Cmd_PlayTrack0: SetSongTrackContext(0); trackStart(fxc, 0, (unsigned long)(millis() - (signed long)TRACK_START_DELAY), FxTrackEndAction::LoopAtEnd); break;
    case Cmd_PlayTrack1: SetSongTrackContext(1); trackStart(fxc, 0, (unsigned long)(millis() - (signed long)TRACK_START_DELAY), FxTrackEndAction::LoopAtEnd); break;
    case Cmd_PlayTrack2: SetSongTrackContext(2); trackStart(fxc, 0, (unsigned long)(millis() - (signed long)TRACK_START_DELAY), FxTrackEndAction::LoopAtEnd); break;
    case Cmd_PlayTrack3: SetSongTrackContext(3); trackStart(fxc, 0, (unsigned long)(millis() - (signed long)TRACK_START_DELAY), FxTrackEndAction::LoopAtEnd); break;

    case Cmd_TransitionFast: FxInstantEvent(fxc, fx_transition_fast,   FxPaletteUpdateType::Once); break;
#if ENABLE_NEOPIXEL

    case Cmd_Brightness_Max: 
    {
      for (int strip=0;strip<NUM_STRIPS;strip++)
        neopixelSetBrightness(strip,BRIGHTNESS_LIMIT);
      break;
    }
    case Cmd_Brightness_Normal:
    {
      for (int strip=0;strip<NUM_STRIPS;strip++)
        neopixelSetBrightness(strip,BRIGHTNESS);
      break;
    }
    case Cmd_Brightness_Half:
    {
      for (int strip=0;strip<NUM_STRIPS;strip++)
        neopixelSetBrightness(strip,BRIGHTNESS/2);
      break;
    }
    
#endif  
    case Cmd_Help:
      Serial.println(F("? : Help Menu"));
      Serial.println(F("[ ! @] default/test"));
      Serial.println(F("( * ) : Track Start/StartFrom/Stop"));
      Serial.println(F("@code : Time code"));
      Serial.println(F("[ % | ^ | &] : Brightness Half/Normal/Full"));
      Serial.println(F("[_ + - = ~] Neg Pos Dec Inc Rst"));
      Serial.println(F("0:dark 1:white 2:red 3:yellow 4:green 5:cyan 6:blue 7:magenta 8:orange 9:half"));
      Serial.println(F("[Q to P] Strips Q:strip0 W:strip1 E:strip2 R:strip3 T:strip4 Y:strip5 U:strip6 I:strip7 O:None P:All"));
      Serial.println(F("Mix1 z:lava x:cloud c:rainbow v:rainbowstripe b:party n:heat"));
      Serial.println(F("Mix2 Z:wm X:rb C:cm V:cb B:gm"));
      Serial.println(F("Pulse1 a:white s:red d:yellow e:green f:cyan g:blue j:magenta k:orange l:half"));
      Serial.println(F("Pulse2 A:white S:red D:yellow E:green F:cyan G:blue J:magenta K:orange L:half"));
      Serial.println(F("[< >] Particles Off/On"));
      break;
    default: break;
  }
}

void UserCommandInput(FxController &fxc, int data)
{
  switch (data)
  {
    case '?': UserCommandExecute(fxc, Cmd_Help); break;
    
    case '!': UserCommandExecute(fxc, Cmd_State_Default);break;
    case '@': UserCommandExecute(fxc, Cmd_State_Test);break;
    case '#': UserCommandExecute(fxc, Cmd_State_MultiTest);break;
    
    case '%': UserCommandExecute(fxc, Cmd_Brightness_Half);break;
    case '^': UserCommandExecute(fxc, Cmd_Brightness_Normal);break;
    case '&': UserCommandExecute(fxc, Cmd_Brightness_Max);break;

    case ',': UserCommandExecute(fxc, Cmd_PalTypeSmoothed); break;
    case '.': UserCommandExecute(fxc, Cmd_PalTypeLiteral);  break;

    case ')': UserCommandExecute(fxc, Cmd_PlayFromStart); break;
    case '*': UserCommandExecute(fxc, Cmd_PlayFrom); break;
    case '(': UserCommandExecute(fxc, Cmd_PlayStop); break;
    
    case '0': UserCommandExecute(fxc, Cmd_ColorDark); break;
    case '1': UserCommandExecute(fxc, Cmd_ColorWhite); break;
    case '2': UserCommandExecute(fxc, Cmd_ColorRed); break;
    case '3': UserCommandExecute(fxc, Cmd_ColorYellow); break;
    case '4': UserCommandExecute(fxc, Cmd_ColorGreen); break;
    case '5': UserCommandExecute(fxc, Cmd_ColorCyan); break;
    case '6': UserCommandExecute(fxc, Cmd_ColorBlue); break;
    case '7': UserCommandExecute(fxc, Cmd_ColorMagenta); break;
    case '8': UserCommandExecute(fxc, Cmd_ColorOrange); break;
    case '9': UserCommandExecute(fxc, Cmd_ColorHalf); break;

    case 'a': UserCommandExecute(fxc, Cmd_PlayTrack0); break;
    case 's': UserCommandExecute(fxc, Cmd_PlayTrack1); break;
    case 'd': UserCommandExecute(fxc, Cmd_PlayTrack2); break;
    case 'f': UserCommandExecute(fxc, Cmd_PlayTrack3); break;
    /*case 'a': UserCommandExecute(fxc, Cmd_ColorPulseWhite); break;
    case 's': UserCommandExecute(fxc, Cmd_ColorPulseRed); break;
    case 'd': UserCommandExecute(fxc, Cmd_ColorPulseYellow); break;
    case 'f': UserCommandExecute(fxc, Cmd_ColorPulseGreen); break;
    case 'g': UserCommandExecute(fxc, Cmd_ColorPulseCyan); break;
    case 'h': UserCommandExecute(fxc, Cmd_ColorPulseBlue); break;
    case 'j': UserCommandExecute(fxc, Cmd_ColorPulseMagenta); break;
    case 'k': UserCommandExecute(fxc, Cmd_ColorPulseOrange); break;
    case 'l': UserCommandExecute(fxc, Cmd_ColorPulseHalf); break;
    
    case 'A': UserCommandExecute(fxc, Cmd_ColorPulse2White); break;
    case 'S': UserCommandExecute(fxc, Cmd_ColorPulse2Red); break;
    case 'D': UserCommandExecute(fxc, Cmd_ColorPulse2Yellow); break;
    case 'F': UserCommandExecute(fxc, Cmd_ColorPulse2Green); break;
    case 'G': UserCommandExecute(fxc, Cmd_ColorPulse2Cyan); break;
    case 'H': UserCommandExecute(fxc, Cmd_ColorPulse2Blue); break;    
    case 'J': UserCommandExecute(fxc, Cmd_ColorPulse2Magenta); break;
    case 'K': UserCommandExecute(fxc, Cmd_ColorPulse2Orange); break;
    case 'L': UserCommandExecute(fxc, Cmd_ColorPulse2Half); break;*/

    case 'q': fxc.stripMask = DRAGON_TORSO_LEFT|DRAGON_TORSO_RIGHT; break;
    case 'w': fxc.stripMask = DRAGON_WING_LEFT|DRAGON_WING_RIGHT; break;
    case 'e': fxc.stripMask = DRAGON_HEAD; break;
    case 'r': fxc.stripMask = DRAGON_TAIL; break;
    case 't': fxc.stripMask = LEDS_0; break;
    case 'y': fxc.stripMask = LEDS_0; break;
    case 'u': fxc.stripMask = LEDS_0; break;
    case 'i': fxc.stripMask = LEDS_0; break;
    case 'o': fxc.stripMask = LEDS_0; break;
    case 'p': fxc.stripMask = LEDS_0; break;
    
    case 'Q': UserCommandExecute(fxc, Cmd_Strip0);break;
    case 'W': UserCommandExecute(fxc, Cmd_Strip1);break;
    case 'E': UserCommandExecute(fxc, Cmd_Strip2);break;
    case 'R': UserCommandExecute(fxc, Cmd_Strip3);break;
    case 'T': UserCommandExecute(fxc, Cmd_Strip4);break;
    case 'Y': UserCommandExecute(fxc, Cmd_Strip5);break;
    case 'U': UserCommandExecute(fxc, Cmd_Strip6);break;
    case 'I': UserCommandExecute(fxc, Cmd_Strip7);break;
    case 'O': UserCommandExecute(fxc, Cmd_StripNone);break;
    case 'P': UserCommandExecute(fxc, Cmd_StripAll);break;

    case 'z': UserCommandExecute(fxc, Cmd_ColorLava);break;
    case 'x': UserCommandExecute(fxc, Cmd_ColorCloud);break;
    case 'c': UserCommandExecute(fxc, Cmd_ColorRainbow);break;
    case 'v': UserCommandExecute(fxc, Cmd_ColorRainbowstripe);break;
    case 'b': UserCommandExecute(fxc, Cmd_ColorParty);break;
    case 'n': UserCommandExecute(fxc, Cmd_ColorHeat);break;

    case 'Z': UserCommandExecute(fxc, Cmd_ColorWhiteMagenta);break;
    case 'X': UserCommandExecute(fxc, Cmd_ColorRedBlue);break;
    case 'C': UserCommandExecute(fxc, Cmd_ColorCyanMagenta);break;
    case 'V': UserCommandExecute(fxc, Cmd_ColorCyanBlue);break;
    case 'B': UserCommandExecute(fxc, Cmd_ColorGreenMagenta);break;
    case 'N': UserCommandExecute(fxc, Cmd_ColorDarkCyanMagenta);break;
    case 'M': UserCommandExecute(fxc, Cmd_ColorDarkRedBlue);break;
    
    case '_': UserCommandExecute(fxc, Cmd_SpeedNeg);break;
    case '+': UserCommandExecute(fxc, Cmd_SpeedPos);break;
    case '-': UserCommandExecute(fxc, Cmd_SpeedDec);break;
    case '=': UserCommandExecute(fxc, Cmd_SpeedInc);break;
    case '~': UserCommandExecute(fxc, Cmd_SpeedRst);break;

    case '<': UserCommandExecute(fxc, Cmd_Particles_Off);break;
    case '>': UserCommandExecute(fxc, Cmd_Particles_On);break;

    case 10:
    case 13:
      break;
    case 0:
    case 225: break;
    default:
      Serial.print(F("unk:"));
      Serial.println(data);
      break;
  }
}

void ComplexUserCommandInput(FxController &fxc, String data)
{
  //Remove carriage returns; 
  if (data[data.length()-1]=='\n') data.remove(data.length()-1,1);
  if (data[data.length()-1]=='\r') data.remove(data.length()-1,1);
}

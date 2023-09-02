/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "Cmd.h"
#include "Fx.h"
#include "Devices.h"
#include "SideFX.h"

void UpdatePalette(FxController &fxc);
void ComplexUserCommandInput(FxController &fxc, String data);

void SimpleUserCommandInput(FxController &fxc, int data)
{
  switch (data)
  {
    case '`': 
      Serial.println(F("? : Help Menu"));
      Serial.println(F(" $ % ^  Brightness Half/Normal/Full"));
      Serial.println(F("_ + - = ~ PalSpeed Neg Pos Dec Inc Rst"));
      Serial.println(F(", . / ? PalType Smooth Literal/2/4"));
      Serial.println(F("0:dark 1:white 2:red 3:yellow 4:green 5:cyan 6:blue 7:magenta 8:orange 9:half"));
      Serial.println(F("Q to P = Strips Q:strip0 W:strip1 E:strip2 R:strip3 T:strip4 Y:strip5 U:strip6 I:strip7 O:None P:All"));
      Serial.println(F(" | ] KillFX PrintFX"));
      break;
        
    case '$': FxInstantEvent(fxc, fx_brightness_half); break;
    case '%': FxInstantEvent(fxc, fx_brightness_normal); break;
    case '^': FxInstantEvent(fxc, fx_brightness_max); break;

    case ',': FxInstantEvent(fxc, fx_palette_type_smoothed); break;
    case '.': FxInstantEvent(fxc, fx_palette_type_literal); break;
    case '/': FxInstantEvent(fxc, fx_palette_type_literal2); break;
    case '?': FxInstantEvent(fxc, fx_palette_type_literal4); break;

    case '|': 
      fxc.KillFX();
      FxInstantEvent(fxc, fx_palette_dark);
      FxInstantEvent(fxc, fx_speed_rst);
      FxInstantEvent(fxc, fx_speed_0);
      FxInstantEvent(fxc, fx_strip_all);
      FxInstantEvent(fxc, fx_transition_fast);
      break;
    case '[': 
      for (int strip=0;strip<NUM_STRIPS;strip++)
        if (fxc.stripMask & (1<<strip)) 
          fxc.strip[strip]->paletteIndex = 0;
      break;
    case ']': SideFX_PrintState(fxc); break;
    
    case '0': FxInstantEvent(fxc, fx_palette_dark);break;
    case '1': FxInstantEvent(fxc, fx_palette_white);break;
    case '2': FxInstantEvent(fxc, fx_palette_red);break;
    case '3': FxInstantEvent(fxc, fx_palette_yellow);break;
    case '4': FxInstantEvent(fxc, fx_palette_green);break;
    case '5': FxInstantEvent(fxc, fx_palette_cyan);break;
    case '6': FxInstantEvent(fxc, fx_palette_blue);break;
    case '7': FxInstantEvent(fxc, fx_palette_magenta);break;
    case '8': FxInstantEvent(fxc, fx_palette_orange);break;
    case '9': FxInstantEvent(fxc, fx_palette_half);break;

    case 'a': SideFX_ActivatePreset(fxc, 0); break;
    case 's': SideFX_ActivatePreset(fxc, 1); break;
    case 'd': SideFX_ActivatePreset(fxc, 2); break;
    case 'f': SideFX_ActivatePreset(fxc, 3); break;
    case 'g': SideFX_ActivatePreset(fxc, 4); break;
    case 'h': SideFX_ActivatePreset(fxc, 5); break;
    case 'j': SideFX_ActivatePreset(fxc, 6); break;
    case 'k': SideFX_ActivatePreset(fxc, 7); break;
    case 'l': SideFX_ActivatePreset(fxc, 8); break;

    case 'q': fxc.stripMask = DRAGON_TORSO_LEFT|DRAGON_TORSO_RIGHT; break;
    case 'w': fxc.stripMask = DRAGON_WING_LEFT|DRAGON_WING_RIGHT; break;
    case 'e': fxc.stripMask = DRAGON_HEAD; break;
    case 'r': fxc.stripMask = DRAGON_TAIL; break;
    case 't': fxc.stripMask = DRAGON_TORSO_LEFT; break;
    case 'y': fxc.stripMask = DRAGON_TORSO_RIGHT; break;
    case 'u': fxc.stripMask = DRAGON_WING_LEFT; break;
    case 'i': fxc.stripMask = DRAGON_WING_RIGHT; break;
    case 'o': FxInstantEvent(fxc, fx_strip_all);break;
    case 'p': FxInstantEvent(fxc, fx_strip_inv);break;
    
    case 'Q': FxInstantEvent(fxc, fx_strip0); break;
    case 'W': FxInstantEvent(fxc, fx_strip1); break;
    case 'E': FxInstantEvent(fxc, fx_strip2); break;
    case 'R': FxInstantEvent(fxc, fx_strip3); break;
    case 'T': FxInstantEvent(fxc, fx_strip4); break;
    case 'Y': FxInstantEvent(fxc, fx_strip5); break;
    case 'U': FxInstantEvent(fxc, fx_strip6); break;
    case 'I': FxInstantEvent(fxc, fx_strip7); break;
    case 'O': FxInstantEvent(fxc, fx_strip_inv); break;
    case 'P': FxInstantEvent(fxc, fx_strip_all); break;

    case 'z': break;
    case 'x': break;
    case 'c': break;
    case 'v': break;
    case 'b': break;
    case 'n': break;

    case 'Z': break;
    case 'X': break;
    case 'C': break;
    case 'V': break;
    case 'B': break;
    case 'N': break;
    case 'M': break;
    
    case '_': FxInstantEvent(fxc, fx_speed_neg); break;
    case '+': FxInstantEvent(fxc, fx_speed_pos); break;
    case '-': FxInstantEvent(fxc, fx_speed_dec); break;
    case '=': FxInstantEvent(fxc, fx_speed_inc); break;
    case '~': FxInstantEvent(fxc, fx_speed_0); break;

    case '<':  break;
    case '>':  break;
    case ';':  break;
    case '\'': break;

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
  int len = data.length();
  Serial.print(F("ComplexCommand="));
  Serial.println(data);  

  //If's to avoid excessive string comparisons
  if (data[0] == 'p') //palette settings
  { 
    if (len == 4)
    {
      if (data == F("pryd")) FxInstantEvent(fxc, fx_palette_dry);
      if (data == F("prgd")) FxInstantEvent(fxc, fx_palette_drg);
      if (data == F("prcd")) FxInstantEvent(fxc, fx_palette_drc);
      if (data == F("prbd")) FxInstantEvent(fxc, fx_palette_drb);
      if (data == F("prmd")) FxInstantEvent(fxc, fx_palette_drm);
      if (data == F("pryw")) FxInstantEvent(fxc, fx_palette_wry);
      if (data == F("prgw")) FxInstantEvent(fxc, fx_palette_wrg);
      if (data == F("prcw")) FxInstantEvent(fxc, fx_palette_wrc);
      if (data == F("prbw")) FxInstantEvent(fxc, fx_palette_wrb);
      if (data == F("prmw")) FxInstantEvent(fxc, fx_palette_wrm);
      
      if (data == F("pygd")) FxInstantEvent(fxc, fx_palette_dyg);
      if (data == F("pycd")) FxInstantEvent(fxc, fx_palette_dyc);
      if (data == F("pybd")) FxInstantEvent(fxc, fx_palette_dyb);
      if (data == F("pymd")) FxInstantEvent(fxc, fx_palette_dym);
      if (data == F("pygw")) FxInstantEvent(fxc, fx_palette_wyg);
      if (data == F("pycw")) FxInstantEvent(fxc, fx_palette_wyc);
      if (data == F("pybw")) FxInstantEvent(fxc, fx_palette_wyb);
      if (data == F("pymw")) FxInstantEvent(fxc, fx_palette_wym);
                                                                
      if (data == F("pgcd")) FxInstantEvent(fxc, fx_palette_dgc);
      if (data == F("pgbd")) FxInstantEvent(fxc, fx_palette_dgb);
      if (data == F("pgmd")) FxInstantEvent(fxc, fx_palette_dgm);
      if (data == F("pgcw")) FxInstantEvent(fxc, fx_palette_wgc);
      if (data == F("pgbw")) FxInstantEvent(fxc, fx_palette_wgb);
      if (data == F("pgmw")) FxInstantEvent(fxc, fx_palette_wgm);
                                                                
      if (data == F("pcbd")) FxInstantEvent(fxc, fx_palette_dcb);
      if (data == F("pcmd")) FxInstantEvent(fxc, fx_palette_dcm);
      if (data == F("pcbw")) FxInstantEvent(fxc, fx_palette_wcb);
      if (data == F("pcmw")) FxInstantEvent(fxc, fx_palette_wcm);
                                                                
      if (data == F("pbmd")) FxInstantEvent(fxc, fx_palette_dbm);
      if (data == F("pbmw")) FxInstantEvent(fxc, fx_palette_wbm);
                                                                
      if (data == F("prgb")) FxInstantEvent(fxc, fx_palette_rgb);
      if (data == F("prbm")) FxInstantEvent(fxc, fx_palette_rbm);
      if (data == F("pcmy")) FxInstantEvent(fxc, fx_palette_cmy);
      if (data == F("pcbm")) FxInstantEvent(fxc, fx_palette_cbm);
    }
    else if (len == 3)
    {
      if (data == F("prd")) FxInstantEvent(fxc, fx_palette_dr);
      if (data == F("prr")) FxInstantEvent(fxc, fx_palette_red);
      if (data == F("pry")) FxInstantEvent(fxc, fx_palette_ry);
      if (data == F("prg")) FxInstantEvent(fxc, fx_palette_rg);
      if (data == F("prc")) FxInstantEvent(fxc, fx_palette_rc);
      if (data == F("prb")) FxInstantEvent(fxc, fx_palette_rb);
      if (data == F("prm")) FxInstantEvent(fxc, fx_palette_rm);
      if (data == F("prw")) FxInstantEvent(fxc, fx_palette_wr);
    
      if (data == F("pyd")) FxInstantEvent(fxc, fx_palette_dy);
      if (data == F("pyr")) FxInstantEvent(fxc, fx_palette_ry);
      if (data == F("pyy")) FxInstantEvent(fxc, fx_palette_yellow);
      if (data == F("pyg")) FxInstantEvent(fxc, fx_palette_yg);
      if (data == F("pyc")) FxInstantEvent(fxc, fx_palette_yc);
      if (data == F("pyb")) FxInstantEvent(fxc, fx_palette_yb);
      if (data == F("pym")) FxInstantEvent(fxc, fx_palette_ym);
      if (data == F("pyw")) FxInstantEvent(fxc, fx_palette_wy);
      
      if (data == F("pgd")) FxInstantEvent(fxc, fx_palette_dg);
      if (data == F("pgr")) FxInstantEvent(fxc, fx_palette_rg);
      if (data == F("pgy")) FxInstantEvent(fxc, fx_palette_yg);
      if (data == F("pgg")) FxInstantEvent(fxc, fx_palette_green);
      if (data == F("pgc")) FxInstantEvent(fxc, fx_palette_gc);
      if (data == F("pgb")) FxInstantEvent(fxc, fx_palette_gb);
      if (data == F("pgm")) FxInstantEvent(fxc, fx_palette_gm);
      if (data == F("pgw")) FxInstantEvent(fxc, fx_palette_wg);
    
      if (data == F("pcd")) FxInstantEvent(fxc, fx_palette_dc);
      if (data == F("pcr")) FxInstantEvent(fxc, fx_palette_rc);
      if (data == F("pcy")) FxInstantEvent(fxc, fx_palette_yc);
      if (data == F("pcg")) FxInstantEvent(fxc, fx_palette_gc);
      if (data == F("pcc")) FxInstantEvent(fxc, fx_palette_cyan);
      if (data == F("pcb")) FxInstantEvent(fxc, fx_palette_cb);
      if (data == F("pcm")) FxInstantEvent(fxc, fx_palette_cm);
      if (data == F("pcw")) FxInstantEvent(fxc, fx_palette_wc);
      
      if (data == F("pbd")) FxInstantEvent(fxc, fx_palette_db);
      if (data == F("pbr")) FxInstantEvent(fxc, fx_palette_rb);
      if (data == F("pby")) FxInstantEvent(fxc, fx_palette_yb);
      if (data == F("pbg")) FxInstantEvent(fxc, fx_palette_gb);
      if (data == F("pbc")) FxInstantEvent(fxc, fx_palette_cb);
      if (data == F("pbb")) FxInstantEvent(fxc, fx_palette_blue);
      if (data == F("pbm")) FxInstantEvent(fxc, fx_palette_bm);
      if (data == F("pbw")) FxInstantEvent(fxc, fx_palette_wb);
    
      if (data == F("pmd")) FxInstantEvent(fxc, fx_palette_dm);
      if (data == F("pmr")) FxInstantEvent(fxc, fx_palette_rm);
      if (data == F("pmy")) FxInstantEvent(fxc, fx_palette_ym);
      if (data == F("pmg")) FxInstantEvent(fxc, fx_palette_gm);
      if (data == F("pmc")) FxInstantEvent(fxc, fx_palette_cm);
      if (data == F("pmb")) FxInstantEvent(fxc, fx_palette_bm);
      if (data == F("pmm")) FxInstantEvent(fxc, fx_palette_magenta);
      if (data == F("pmw")) FxInstantEvent(fxc, fx_palette_wm);
    }
    else if (len == 2)
    {
      if (data == F("p0"))  FxInstantEvent(fxc, fx_speed_0);
      if (data == F("p1"))  FxInstantEvent(fxc, fx_speed_1);
      if (data == F("p2"))  FxInstantEvent(fxc, fx_speed_2);
      if (data == F("p3"))  FxInstantEvent(fxc, fx_speed_3);
      if (data == F("p4"))  FxInstantEvent(fxc, fx_speed_4);
      if (data == F("p5"))  FxInstantEvent(fxc, fx_speed_5);
      if (data == F("p6"))  FxInstantEvent(fxc, fx_speed_6);
      if (data == F("p7"))  FxInstantEvent(fxc, fx_speed_7);
      if (data == F("p8"))  FxInstantEvent(fxc, fx_speed_8);
    }
    return;
  }
  else if (data[0] == '!') //transitions
  {
    if (data == F("!f"))   FxInstantEvent(fxc, fx_transition_fast);
    if (data == F("!tf"))  FxInstantEvent(fxc, fx_transition_timed_fade);
    if (data == F("!twp")) FxInstantEvent(fxc, fx_transition_timed_wipe_pos);
    if (data == F("!twn")) FxInstantEvent(fxc, fx_transition_timed_wipe_neg);
    if (data == F("!two")) FxInstantEvent(fxc, fx_transition_timed_wipe_outin);
    if (data == F("!twi")) FxInstantEvent(fxc, fx_transition_timed_wipe_inout);
    if (data == F("!twr")) FxInstantEvent(fxc, fx_transition_timed_wipe_random);
    return;
  }
  else if (data[0] == '_')
  {
    if (data == F("_red"))      FxInstantEvent(fxc, fx_palette_red);
    if (data == F("_yellow"))   FxInstantEvent(fxc, fx_palette_yellow);
    if (data == F("_green"))    FxInstantEvent(fxc, fx_palette_green);
    if (data == F("_cyan"))     FxInstantEvent(fxc, fx_palette_cyan);
    if (data == F("_blue"))     FxInstantEvent(fxc, fx_palette_blue);
    if (data == F("_magenta"))  FxInstantEvent(fxc, fx_palette_magenta);
    if (data == F("_white"))    FxInstantEvent(fxc, fx_palette_white);
    if (data == F("_dark"))     FxInstantEvent(fxc, fx_palette_dark);

    if (data == F("_red2"))     FxInstantEvent(fxc, fx_palette_dr);
    if (data == F("_yellow2"))  FxInstantEvent(fxc, fx_palette_dy);
    if (data == F("_green2"))   FxInstantEvent(fxc, fx_palette_dg);
    if (data == F("_cyan2"))    FxInstantEvent(fxc, fx_palette_dc);
    if (data == F("_blue2"))    FxInstantEvent(fxc, fx_palette_db);
    if (data == F("_magenta2")) FxInstantEvent(fxc, fx_palette_dm);
    
    if (data == F("_lava"))     FxInstantEvent(fxc, fx_palette_lava);
    if (data == F("_cloud"))    FxInstantEvent(fxc, fx_palette_cloud);
    if (data == F("_ocean"))    FxInstantEvent(fxc, fx_palette_ocean);
    if (data == F("_forest"))   FxInstantEvent(fxc, fx_palette_forest);
    if (data == F("_rainbow"))  FxInstantEvent(fxc, fx_palette_rainbow);
    if (data == F("_rainbow2")) FxInstantEvent(fxc, fx_palette_rainbowstripe);
    if (data == F("_party"))    FxInstantEvent(fxc, fx_palette_party);
    if (data == F("_heat"))     FxInstantEvent(fxc, fx_palette_heat);
    return;
  }

  if (data == F("t01")) fxc.Toggle(1);
  if (data == F("t02")) fxc.Toggle(2);
  if (data == F("t03")) fxc.Toggle(3);
  if (data == F("t04")) fxc.Toggle(4);
  if (data == F("t05")) fxc.Toggle(5);
  if (data == F("t06")) fxc.Toggle(6);
  if (data == F("t07")) fxc.Toggle(7);

  if (data == F("x01")) fxc.Excite(1);
  if (data == F("x02")) fxc.Excite(2);
  if (data == F("x03")) fxc.Excite(3);
  if (data == F("x04")) fxc.Excite(4);
  if (data == F("x05")) fxc.Excite(5);
  if (data == F("x06")) fxc.Excite(6);
  
  if (data == F("@all"))   FxInstantEvent(fxc, fx_strip_all);
  if (data == F("@inv"))   FxInstantEvent(fxc, fx_strip_inv);
  if (data == F("@torso")) fxc.stripMask = DRAGON_TORSO_LEFT|DRAGON_TORSO_RIGHT;
  if (data == F("@wings")) fxc.stripMask = DRAGON_WING_LEFT|DRAGON_WING_RIGHT;
  if (data == F("@head"))  fxc.stripMask = DRAGON_HEAD;
  if (data == F("@tail"))  fxc.stripMask = DRAGON_TAIL;

  if (data == F("test"))  fxc.fxState = FxState_Default;
  if (data == F("test"))  fxc.fxState = FxState_TestPattern;
  if (data == F("mtest")) fxc.fxState = FxState_MultiTestPattern;
  if (data == F("fx"))    fxc.fxState = FxState_SideFX;
    
  if (data == F("kk"))
  {
    fxc.fxState = FxState_SideFX;
    FxInstantEvent(fxc, fx_palette_dark);
    FxInstantEvent(fxc, fx_speed_rst);
    FxInstantEvent(fxc, fx_speed_0);
    FxInstantEvent(fxc, fx_strip_all);
    FxInstantEvent(fxc, fx_transition_fast);
  }  
  
}

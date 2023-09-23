/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "Cmd.h"
#include "FxPalette.h"
#include "FxController.h"
#include "SideFX.h"

void SimpleUserCommandInput(FxController &fxc, int command)
{
  switch (command)
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
        
    case '$': FxEvent(fxc, fx_brightness_half); break;
    case '%': FxEvent(fxc, fx_brightness_normal); break;
    case '^': FxEvent(fxc, fx_brightness_max); break;

    case ',': FxEvent(fxc, fx_palette_type_smoothed); break;
    case '.': FxEvent(fxc, fx_palette_type_literal); break;
    case '/': FxEvent(fxc, fx_palette_type_literal2); break;
    case '?': FxEvent(fxc, fx_palette_type_literal4); break;

    case '0': FxEvent(fxc, fx_palette_dark);break;
    case '1': FxEvent(fxc, fx_palette_white);break;
    case '2': FxEvent(fxc, fx_palette_red);break;
    case '3': FxEvent(fxc, fx_palette_yellow);break;
    case '4': FxEvent(fxc, fx_palette_green);break;
    case '5': FxEvent(fxc, fx_palette_cyan);break;
    case '6': FxEvent(fxc, fx_palette_blue);break;
    case '7': FxEvent(fxc, fx_palette_magenta);break;
    case '8': FxEvent(fxc, fx_palette_orange);break;
    case '9': FxEvent(fxc, fx_palette_half);break;

    case '|': 
      fxc.KillFX();
      FxEvent(fxc, fx_palette_dark);
      FxEvent(fxc, fx_palanim_speed_rst);
      FxEvent(fxc, fx_palanim_speed_0);
      FxEvent(fxc, fx_strip_all);
      FxEvent(fxc, fx_transition_fast);
      break;    
    case 'a': SideFXActivatePreset(fxc, 0); break;
    case 's': SideFXActivatePreset(fxc, 1); break;
    case 'd': SideFXActivatePreset(fxc, 2); break;
    case 'f': SideFXActivatePreset(fxc, 3); break;
    case 'g': SideFXActivatePreset(fxc, 4); break;
    case 'h': SideFXActivatePreset(fxc, 5); break;
    case 'j': SideFXActivatePreset(fxc, 6); break;
    case 'k': SideFXActivatePreset(fxc, 7); break;
    case 'l': SideFXActivatePreset(fxc, 8); break;

    case 'q': fxc.stripMask = DRAGON_TORSO_LEFT|DRAGON_TORSO_RIGHT; break;
    case 'w': fxc.stripMask = DRAGON_WING_LEFT|DRAGON_WING_RIGHT; break;
    case 'e': fxc.stripMask = DRAGON_HEAD; break;
    case 'r': fxc.stripMask = DRAGON_TAIL; break;
    case 't': fxc.stripMask = DRAGON_TORSO_LEFT; break;
    case 'y': fxc.stripMask = DRAGON_TORSO_RIGHT; break;
    case 'u': fxc.stripMask = DRAGON_WING_LEFT; break;
    case 'i': fxc.stripMask = DRAGON_WING_RIGHT; break;
    case 'o': FxEvent(fxc, fx_strip_all);break;
    case 'p': FxEvent(fxc, fx_strip_inv);break;
    
    case 'Q': FxEvent(fxc, fx_strip0); break;
    case 'W': FxEvent(fxc, fx_strip1); break;
    case 'E': FxEvent(fxc, fx_strip2); break;
    case 'R': FxEvent(fxc, fx_strip3); break;
    case 'T': FxEvent(fxc, fx_strip4); break;
    case 'Y': FxEvent(fxc, fx_strip5); break;
    case 'U': FxEvent(fxc, fx_strip6); break;
    case 'I': FxEvent(fxc, fx_strip7); break;
    case 'O': FxEvent(fxc, fx_strip_inv); break;
    case 'P': FxEvent(fxc, fx_strip_all); break;

    case 'z': fxc.fxState = FxState_MultiTestPattern; break;
    case 'x': fxc.fxState = FxState_SideFX; break;
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
    
    case '_': FxEvent(fxc, fx_palanim_speed_neg); break;
    case '+': FxEvent(fxc, fx_palanim_speed_pos); break;
    case '-': FxEvent(fxc, fx_palanim_speed_dec); break;
    case '=': FxEvent(fxc, fx_palanim_speed_inc); break;
    case '~': FxEvent(fxc, fx_palanim_speed_0); break;
    case '[': FxEvent(fxc, fx_palanim_location_rst); break;
    case ']': SideFXPrintState(fxc); break;

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
      Serial.println(command);
      break;
  }
}

void ComplexUserCommandInput(FxController &fxc, String cmd)
{
  //Remove carriage returns; 
  if (cmd[cmd.length()-1]=='\n') cmd.remove(cmd.length()-1,1);
  if (cmd[cmd.length()-1]=='\r') cmd.remove(cmd.length()-1,1);
  int len = cmd.length();
  Serial.print(F("ComplexCommand="));
  Serial.println(cmd);  

  //If's to avoid excessive string comparisons
  if (cmd[0] == 'p') //palette settings
  { 
    if (len == 4)
    {
      if (cmd == F("pryd")) FxEvent(fxc, fx_palette_dry);
      if (cmd == F("prgd")) FxEvent(fxc, fx_palette_drg);
      if (cmd == F("prcd")) FxEvent(fxc, fx_palette_drc);
      if (cmd == F("prbd")) FxEvent(fxc, fx_palette_drb);
      if (cmd == F("prmd")) FxEvent(fxc, fx_palette_drm);
      if (cmd == F("pryw")) FxEvent(fxc, fx_palette_wry);
      if (cmd == F("prgw")) FxEvent(fxc, fx_palette_wrg);
      if (cmd == F("prcw")) FxEvent(fxc, fx_palette_wrc);
      if (cmd == F("prbw")) FxEvent(fxc, fx_palette_wrb);
      if (cmd == F("prmw")) FxEvent(fxc, fx_palette_wrm);
      
      if (cmd == F("pygd")) FxEvent(fxc, fx_palette_dyg);
      if (cmd == F("pycd")) FxEvent(fxc, fx_palette_dyc);
      if (cmd == F("pybd")) FxEvent(fxc, fx_palette_dyb);
      if (cmd == F("pymd")) FxEvent(fxc, fx_palette_dym);
      if (cmd == F("pygw")) FxEvent(fxc, fx_palette_wyg);
      if (cmd == F("pycw")) FxEvent(fxc, fx_palette_wyc);
      if (cmd == F("pybw")) FxEvent(fxc, fx_palette_wyb);
      if (cmd == F("pymw")) FxEvent(fxc, fx_palette_wym);
                                                                
      if (cmd == F("pgcd")) FxEvent(fxc, fx_palette_dgc);
      if (cmd == F("pgbd")) FxEvent(fxc, fx_palette_dgb);
      if (cmd == F("pgmd")) FxEvent(fxc, fx_palette_dgm);
      if (cmd == F("pgcw")) FxEvent(fxc, fx_palette_wgc);
      if (cmd == F("pgbw")) FxEvent(fxc, fx_palette_wgb);
      if (cmd == F("pgmw")) FxEvent(fxc, fx_palette_wgm);
                                                                
      if (cmd == F("pcbd")) FxEvent(fxc, fx_palette_dcb);
      if (cmd == F("pcmd")) FxEvent(fxc, fx_palette_dcm);
      if (cmd == F("pcbw")) FxEvent(fxc, fx_palette_wcb);
      if (cmd == F("pcmw")) FxEvent(fxc, fx_palette_wcm);
                                                                
      if (cmd == F("pbmd")) FxEvent(fxc, fx_palette_dbm);
      if (cmd == F("pbmw")) FxEvent(fxc, fx_palette_wbm);
                                                                
      if (cmd == F("prgb")) FxEvent(fxc, fx_palette_rgb);
      if (cmd == F("prbm")) FxEvent(fxc, fx_palette_rbm);
      if (cmd == F("pcmy")) FxEvent(fxc, fx_palette_cmy);
      if (cmd == F("pcbm")) FxEvent(fxc, fx_palette_cbm);
    }
    else if (len == 3)
    {
      if (cmd == F("prd")) FxEvent(fxc, fx_palette_dr);
      if (cmd == F("prr")) FxEvent(fxc, fx_palette_red);
      if (cmd == F("pry")) FxEvent(fxc, fx_palette_ry);
      if (cmd == F("prg")) FxEvent(fxc, fx_palette_rg);
      if (cmd == F("prc")) FxEvent(fxc, fx_palette_rc);
      if (cmd == F("prb")) FxEvent(fxc, fx_palette_rb);
      if (cmd == F("prm")) FxEvent(fxc, fx_palette_rm);
      if (cmd == F("prw")) FxEvent(fxc, fx_palette_wr);
    
      if (cmd == F("pyd")) FxEvent(fxc, fx_palette_dy);
      if (cmd == F("pyr")) FxEvent(fxc, fx_palette_ry);
      if (cmd == F("pyy")) FxEvent(fxc, fx_palette_yellow);
      if (cmd == F("pyg")) FxEvent(fxc, fx_palette_yg);
      if (cmd == F("pyc")) FxEvent(fxc, fx_palette_yc);
      if (cmd == F("pyb")) FxEvent(fxc, fx_palette_yb);
      if (cmd == F("pym")) FxEvent(fxc, fx_palette_ym);
      if (cmd == F("pyw")) FxEvent(fxc, fx_palette_wy);
      
      if (cmd == F("pgd")) FxEvent(fxc, fx_palette_dg);
      if (cmd == F("pgr")) FxEvent(fxc, fx_palette_rg);
      if (cmd == F("pgy")) FxEvent(fxc, fx_palette_yg);
      if (cmd == F("pgg")) FxEvent(fxc, fx_palette_green);
      if (cmd == F("pgc")) FxEvent(fxc, fx_palette_gc);
      if (cmd == F("pgb")) FxEvent(fxc, fx_palette_gb);
      if (cmd == F("pgm")) FxEvent(fxc, fx_palette_gm);
      if (cmd == F("pgw")) FxEvent(fxc, fx_palette_wg);
    
      if (cmd == F("pcd")) FxEvent(fxc, fx_palette_dc);
      if (cmd == F("pcr")) FxEvent(fxc, fx_palette_rc);
      if (cmd == F("pcy")) FxEvent(fxc, fx_palette_yc);
      if (cmd == F("pcg")) FxEvent(fxc, fx_palette_gc);
      if (cmd == F("pcc")) FxEvent(fxc, fx_palette_cyan);
      if (cmd == F("pcb")) FxEvent(fxc, fx_palette_cb);
      if (cmd == F("pcm")) FxEvent(fxc, fx_palette_cm);
      if (cmd == F("pcw")) FxEvent(fxc, fx_palette_wc);
      
      if (cmd == F("pbd")) FxEvent(fxc, fx_palette_db);
      if (cmd == F("pbr")) FxEvent(fxc, fx_palette_rb);
      if (cmd == F("pby")) FxEvent(fxc, fx_palette_yb);
      if (cmd == F("pbg")) FxEvent(fxc, fx_palette_gb);
      if (cmd == F("pbc")) FxEvent(fxc, fx_palette_cb);
      if (cmd == F("pbb")) FxEvent(fxc, fx_palette_blue);
      if (cmd == F("pbm")) FxEvent(fxc, fx_palette_bm);
      if (cmd == F("pbw")) FxEvent(fxc, fx_palette_wb);
    
      if (cmd == F("pmd")) FxEvent(fxc, fx_palette_dm);
      if (cmd == F("pmr")) FxEvent(fxc, fx_palette_rm);
      if (cmd == F("pmy")) FxEvent(fxc, fx_palette_ym);
      if (cmd == F("pmg")) FxEvent(fxc, fx_palette_gm);
      if (cmd == F("pmc")) FxEvent(fxc, fx_palette_cm);
      if (cmd == F("pmb")) FxEvent(fxc, fx_palette_bm);
      if (cmd == F("pmm")) FxEvent(fxc, fx_palette_magenta);
      if (cmd == F("pmw")) FxEvent(fxc, fx_palette_wm);
    }
    else if (len == 2)
    {
      if (cmd == F("p0"))  FxEvent(fxc, fx_palanim_speed_0);
      if (cmd == F("p1"))  FxEvent(fxc, fx_palanim_speed_1);
      if (cmd == F("p2"))  FxEvent(fxc, fx_palanim_speed_2);
      if (cmd == F("p3"))  FxEvent(fxc, fx_palanim_speed_3);
      if (cmd == F("p4"))  FxEvent(fxc, fx_palanim_speed_4);
      if (cmd == F("p5"))  FxEvent(fxc, fx_palanim_speed_5);
      if (cmd == F("p6"))  FxEvent(fxc, fx_palanim_speed_6);
      if (cmd == F("p7"))  FxEvent(fxc, fx_palanim_speed_7);
      if (cmd == F("p8"))  FxEvent(fxc, fx_palanim_speed_8);
    }
    return;
  }
  else if (cmd[0] == '!') //transitions
  {
    if (cmd == F("!f"))   FxEvent(fxc, fx_transition_fast);
    if (cmd == F("!tf"))  FxEvent(fxc, fx_transition_timed_fade);
    if (cmd == F("!twp")) FxEvent(fxc, fx_transition_timed_wipe_pos);
    if (cmd == F("!twn")) FxEvent(fxc, fx_transition_timed_wipe_neg);
    if (cmd == F("!two")) FxEvent(fxc, fx_transition_timed_wipe_outin);
    if (cmd == F("!twi")) FxEvent(fxc, fx_transition_timed_wipe_inout);
    if (cmd == F("!twr")) FxEvent(fxc, fx_transition_timed_wipe_random);
    return;
  }
  else if (cmd[0] == '_')
  {
    if (cmd == F("_red"))      FxEvent(fxc, fx_palette_red);
    if (cmd == F("_yellow"))   FxEvent(fxc, fx_palette_yellow);
    if (cmd == F("_green"))    FxEvent(fxc, fx_palette_green);
    if (cmd == F("_cyan"))     FxEvent(fxc, fx_palette_cyan);
    if (cmd == F("_blue"))     FxEvent(fxc, fx_palette_blue);
    if (cmd == F("_magenta"))  FxEvent(fxc, fx_palette_magenta);
    if (cmd == F("_white"))    FxEvent(fxc, fx_palette_white);
    if (cmd == F("_dark"))     FxEvent(fxc, fx_palette_dark);

    if (cmd == F("_red2"))     FxEvent(fxc, fx_palette_dr);
    if (cmd == F("_yellow2"))  FxEvent(fxc, fx_palette_dy);
    if (cmd == F("_green2"))   FxEvent(fxc, fx_palette_dg);
    if (cmd == F("_cyan2"))    FxEvent(fxc, fx_palette_dc);
    if (cmd == F("_blue2"))    FxEvent(fxc, fx_palette_db);
    if (cmd == F("_magenta2")) FxEvent(fxc, fx_palette_dm);
    
    if (cmd == F("_lava"))     FxEvent(fxc, fx_palette_lava);
    if (cmd == F("_cloud"))    FxEvent(fxc, fx_palette_cloud);
    if (cmd == F("_ocean"))    FxEvent(fxc, fx_palette_ocean);
    if (cmd == F("_forest"))   FxEvent(fxc, fx_palette_forest);
    if (cmd == F("_rainbow"))  FxEvent(fxc, fx_palette_rainbow);
    if (cmd == F("_rainbow2")) FxEvent(fxc, fx_palette_rainbowstripe);
    if (cmd == F("_party"))    FxEvent(fxc, fx_palette_party);
    if (cmd == F("_heat"))     FxEvent(fxc, fx_palette_heat);
    return;
  }

  if (cmd == F("qq"))
  {
    fxc.Toggle(1);
    fxc.Toggle(4);
    FxEvent(fxc, fx_palette_rgb);
  }

  if (cmd == F("t01"))   fxc.Toggle(1);
  if (cmd == F("t04"))   fxc.Toggle(4);
  
  if (cmd == F("@all"))   FxEvent(fxc, fx_strip_all);
  if (cmd == F("@inv"))   FxEvent(fxc, fx_strip_inv);
  if (cmd == F("@torso")) fxc.stripMask = DRAGON_TORSO_LEFT|DRAGON_TORSO_RIGHT;
  if (cmd == F("@wings")) fxc.stripMask = DRAGON_WING_LEFT|DRAGON_WING_RIGHT;
  if (cmd == F("@head"))  fxc.stripMask = DRAGON_HEAD;
  if (cmd == F("@tail"))  fxc.stripMask = DRAGON_TAIL;

  if (cmd == F("default")) fxc.fxState = FxState_Default;
  if (cmd == F("test"))    fxc.fxState = FxState_TestPattern;
  if (cmd == F("mtest"))   fxc.fxState = FxState_MultiTestPattern;
  if (cmd == F("fx"))      fxc.fxState = FxState_SideFX;
    
}

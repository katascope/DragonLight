/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "Cmd.h"
#include "Fx.h"
#include "SideFX.h"

void UpdatePalette(FxController &fxc);
void ComplexUserCommandInput(FxController &fxc, String command);

void SimpleUserCommandInput(FxController &fxc, int command)
{
  PrintFxEventName(command);
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
      if (cmd == F("pryd")) FxInstantEvent(fxc, fx_palette_dry);
      if (cmd == F("prgd")) FxInstantEvent(fxc, fx_palette_drg);
      if (cmd == F("prcd")) FxInstantEvent(fxc, fx_palette_drc);
      if (cmd == F("prbd")) FxInstantEvent(fxc, fx_palette_drb);
      if (cmd == F("prmd")) FxInstantEvent(fxc, fx_palette_drm);
      if (cmd == F("pryw")) FxInstantEvent(fxc, fx_palette_wry);
      if (cmd == F("prgw")) FxInstantEvent(fxc, fx_palette_wrg);
      if (cmd == F("prcw")) FxInstantEvent(fxc, fx_palette_wrc);
      if (cmd == F("prbw")) FxInstantEvent(fxc, fx_palette_wrb);
      if (cmd == F("prmw")) FxInstantEvent(fxc, fx_palette_wrm);
      
      if (cmd == F("pygd")) FxInstantEvent(fxc, fx_palette_dyg);
      if (cmd == F("pycd")) FxInstantEvent(fxc, fx_palette_dyc);
      if (cmd == F("pybd")) FxInstantEvent(fxc, fx_palette_dyb);
      if (cmd == F("pymd")) FxInstantEvent(fxc, fx_palette_dym);
      if (cmd == F("pygw")) FxInstantEvent(fxc, fx_palette_wyg);
      if (cmd == F("pycw")) FxInstantEvent(fxc, fx_palette_wyc);
      if (cmd == F("pybw")) FxInstantEvent(fxc, fx_palette_wyb);
      if (cmd == F("pymw")) FxInstantEvent(fxc, fx_palette_wym);
                                                                
      if (cmd == F("pgcd")) FxInstantEvent(fxc, fx_palette_dgc);
      if (cmd == F("pgbd")) FxInstantEvent(fxc, fx_palette_dgb);
      if (cmd == F("pgmd")) FxInstantEvent(fxc, fx_palette_dgm);
      if (cmd == F("pgcw")) FxInstantEvent(fxc, fx_palette_wgc);
      if (cmd == F("pgbw")) FxInstantEvent(fxc, fx_palette_wgb);
      if (cmd == F("pgmw")) FxInstantEvent(fxc, fx_palette_wgm);
                                                                
      if (cmd == F("pcbd")) FxInstantEvent(fxc, fx_palette_dcb);
      if (cmd == F("pcmd")) FxInstantEvent(fxc, fx_palette_dcm);
      if (cmd == F("pcbw")) FxInstantEvent(fxc, fx_palette_wcb);
      if (cmd == F("pcmw")) FxInstantEvent(fxc, fx_palette_wcm);
                                                                
      if (cmd == F("pbmd")) FxInstantEvent(fxc, fx_palette_dbm);
      if (cmd == F("pbmw")) FxInstantEvent(fxc, fx_palette_wbm);
                                                                
      if (cmd == F("prgb")) FxInstantEvent(fxc, fx_palette_rgb);
      if (cmd == F("prbm")) FxInstantEvent(fxc, fx_palette_rbm);
      if (cmd == F("pcmy")) FxInstantEvent(fxc, fx_palette_cmy);
      if (cmd == F("pcbm")) FxInstantEvent(fxc, fx_palette_cbm);
    }
    else if (len == 3)
    {
      if (cmd == F("prd")) FxInstantEvent(fxc, fx_palette_dr);
      if (cmd == F("prr")) FxInstantEvent(fxc, fx_palette_red);
      if (cmd == F("pry")) FxInstantEvent(fxc, fx_palette_ry);
      if (cmd == F("prg")) FxInstantEvent(fxc, fx_palette_rg);
      if (cmd == F("prc")) FxInstantEvent(fxc, fx_palette_rc);
      if (cmd == F("prb")) FxInstantEvent(fxc, fx_palette_rb);
      if (cmd == F("prm")) FxInstantEvent(fxc, fx_palette_rm);
      if (cmd == F("prw")) FxInstantEvent(fxc, fx_palette_wr);
    
      if (cmd == F("pyd")) FxInstantEvent(fxc, fx_palette_dy);
      if (cmd == F("pyr")) FxInstantEvent(fxc, fx_palette_ry);
      if (cmd == F("pyy")) FxInstantEvent(fxc, fx_palette_yellow);
      if (cmd == F("pyg")) FxInstantEvent(fxc, fx_palette_yg);
      if (cmd == F("pyc")) FxInstantEvent(fxc, fx_palette_yc);
      if (cmd == F("pyb")) FxInstantEvent(fxc, fx_palette_yb);
      if (cmd == F("pym")) FxInstantEvent(fxc, fx_palette_ym);
      if (cmd == F("pyw")) FxInstantEvent(fxc, fx_palette_wy);
      
      if (cmd == F("pgd")) FxInstantEvent(fxc, fx_palette_dg);
      if (cmd == F("pgr")) FxInstantEvent(fxc, fx_palette_rg);
      if (cmd == F("pgy")) FxInstantEvent(fxc, fx_palette_yg);
      if (cmd == F("pgg")) FxInstantEvent(fxc, fx_palette_green);
      if (cmd == F("pgc")) FxInstantEvent(fxc, fx_palette_gc);
      if (cmd == F("pgb")) FxInstantEvent(fxc, fx_palette_gb);
      if (cmd == F("pgm")) FxInstantEvent(fxc, fx_palette_gm);
      if (cmd == F("pgw")) FxInstantEvent(fxc, fx_palette_wg);
    
      if (cmd == F("pcd")) FxInstantEvent(fxc, fx_palette_dc);
      if (cmd == F("pcr")) FxInstantEvent(fxc, fx_palette_rc);
      if (cmd == F("pcy")) FxInstantEvent(fxc, fx_palette_yc);
      if (cmd == F("pcg")) FxInstantEvent(fxc, fx_palette_gc);
      if (cmd == F("pcc")) FxInstantEvent(fxc, fx_palette_cyan);
      if (cmd == F("pcb")) FxInstantEvent(fxc, fx_palette_cb);
      if (cmd == F("pcm")) FxInstantEvent(fxc, fx_palette_cm);
      if (cmd == F("pcw")) FxInstantEvent(fxc, fx_palette_wc);
      
      if (cmd == F("pbd")) FxInstantEvent(fxc, fx_palette_db);
      if (cmd == F("pbr")) FxInstantEvent(fxc, fx_palette_rb);
      if (cmd == F("pby")) FxInstantEvent(fxc, fx_palette_yb);
      if (cmd == F("pbg")) FxInstantEvent(fxc, fx_palette_gb);
      if (cmd == F("pbc")) FxInstantEvent(fxc, fx_palette_cb);
      if (cmd == F("pbb")) FxInstantEvent(fxc, fx_palette_blue);
      if (cmd == F("pbm")) FxInstantEvent(fxc, fx_palette_bm);
      if (cmd == F("pbw")) FxInstantEvent(fxc, fx_palette_wb);
    
      if (cmd == F("pmd")) FxInstantEvent(fxc, fx_palette_dm);
      if (cmd == F("pmr")) FxInstantEvent(fxc, fx_palette_rm);
      if (cmd == F("pmy")) FxInstantEvent(fxc, fx_palette_ym);
      if (cmd == F("pmg")) FxInstantEvent(fxc, fx_palette_gm);
      if (cmd == F("pmc")) FxInstantEvent(fxc, fx_palette_cm);
      if (cmd == F("pmb")) FxInstantEvent(fxc, fx_palette_bm);
      if (cmd == F("pmm")) FxInstantEvent(fxc, fx_palette_magenta);
      if (cmd == F("pmw")) FxInstantEvent(fxc, fx_palette_wm);
    }
    else if (len == 2)
    {
      if (cmd == F("p0"))  FxInstantEvent(fxc, fx_speed_0);
      if (cmd == F("p1"))  FxInstantEvent(fxc, fx_speed_1);
      if (cmd == F("p2"))  FxInstantEvent(fxc, fx_speed_2);
      if (cmd == F("p3"))  FxInstantEvent(fxc, fx_speed_3);
      if (cmd == F("p4"))  FxInstantEvent(fxc, fx_speed_4);
      if (cmd == F("p5"))  FxInstantEvent(fxc, fx_speed_5);
      if (cmd == F("p6"))  FxInstantEvent(fxc, fx_speed_6);
      if (cmd == F("p7"))  FxInstantEvent(fxc, fx_speed_7);
      if (cmd == F("p8"))  FxInstantEvent(fxc, fx_speed_8);
    }
    return;
  }
  else if (cmd[0] == '!') //transitions
  {
    if (cmd == F("!f"))   FxInstantEvent(fxc, fx_transition_fast);
    if (cmd == F("!tf"))  FxInstantEvent(fxc, fx_transition_timed_fade);
    if (cmd == F("!twp")) FxInstantEvent(fxc, fx_transition_timed_wipe_pos);
    if (cmd == F("!twn")) FxInstantEvent(fxc, fx_transition_timed_wipe_neg);
    if (cmd == F("!two")) FxInstantEvent(fxc, fx_transition_timed_wipe_outin);
    if (cmd == F("!twi")) FxInstantEvent(fxc, fx_transition_timed_wipe_inout);
    if (cmd == F("!twr")) FxInstantEvent(fxc, fx_transition_timed_wipe_random);
    return;
  }
  else if (cmd[0] == '_')
  {
    if (cmd == F("_red"))      FxInstantEvent(fxc, fx_palette_red);
    if (cmd == F("_yellow"))   FxInstantEvent(fxc, fx_palette_yellow);
    if (cmd == F("_green"))    FxInstantEvent(fxc, fx_palette_green);
    if (cmd == F("_cyan"))     FxInstantEvent(fxc, fx_palette_cyan);
    if (cmd == F("_blue"))     FxInstantEvent(fxc, fx_palette_blue);
    if (cmd == F("_magenta"))  FxInstantEvent(fxc, fx_palette_magenta);
    if (cmd == F("_white"))    FxInstantEvent(fxc, fx_palette_white);
    if (cmd == F("_dark"))     FxInstantEvent(fxc, fx_palette_dark);

    if (cmd == F("_red2"))     FxInstantEvent(fxc, fx_palette_dr);
    if (cmd == F("_yellow2"))  FxInstantEvent(fxc, fx_palette_dy);
    if (cmd == F("_green2"))   FxInstantEvent(fxc, fx_palette_dg);
    if (cmd == F("_cyan2"))    FxInstantEvent(fxc, fx_palette_dc);
    if (cmd == F("_blue2"))    FxInstantEvent(fxc, fx_palette_db);
    if (cmd == F("_magenta2")) FxInstantEvent(fxc, fx_palette_dm);
    
    if (cmd == F("_lava"))     FxInstantEvent(fxc, fx_palette_lava);
    if (cmd == F("_cloud"))    FxInstantEvent(fxc, fx_palette_cloud);
    if (cmd == F("_ocean"))    FxInstantEvent(fxc, fx_palette_ocean);
    if (cmd == F("_forest"))   FxInstantEvent(fxc, fx_palette_forest);
    if (cmd == F("_rainbow"))  FxInstantEvent(fxc, fx_palette_rainbow);
    if (cmd == F("_rainbow2")) FxInstantEvent(fxc, fx_palette_rainbowstripe);
    if (cmd == F("_party"))    FxInstantEvent(fxc, fx_palette_party);
    if (cmd == F("_heat"))     FxInstantEvent(fxc, fx_palette_heat);
    return;
  }

  if (cmd == F("t01")) fxc.Toggle(1);
  if (cmd == F("t02")) fxc.Toggle(2);
  if (cmd == F("t03")) fxc.Toggle(3);
  if (cmd == F("t04")) fxc.Toggle(4);
  if (cmd == F("t05")) fxc.Toggle(5);
  if (cmd == F("t06")) fxc.Toggle(6);
  if (cmd == F("t07")) fxc.Toggle(7);

  if (cmd == F("x01")) fxc.Excite(1);
  if (cmd == F("x02")) fxc.Excite(2);
  if (cmd == F("x03")) fxc.Excite(3);
  if (cmd == F("x04")) fxc.Excite(4);
  if (cmd == F("x05")) fxc.Excite(5);
  if (cmd == F("x06")) fxc.Excite(6);
  
  if (cmd == F("@all"))   FxInstantEvent(fxc, fx_strip_all);
  if (cmd == F("@inv"))   FxInstantEvent(fxc, fx_strip_inv);
  if (cmd == F("@torso")) fxc.stripMask = DRAGON_TORSO_LEFT|DRAGON_TORSO_RIGHT;
  if (cmd == F("@wings")) fxc.stripMask = DRAGON_WING_LEFT|DRAGON_WING_RIGHT;
  if (cmd == F("@head"))  fxc.stripMask = DRAGON_HEAD;
  if (cmd == F("@tail"))  fxc.stripMask = DRAGON_TAIL;

  if (cmd == F("test"))  fxc.fxState = FxState_Default;
  if (cmd == F("test"))  fxc.fxState = FxState_TestPattern;
  if (cmd == F("mtest")) fxc.fxState = FxState_MultiTestPattern;
  if (cmd == F("fx"))    fxc.fxState = FxState_SideFX;
    
  if (cmd == F("kk"))
  {
    fxc.fxState = FxState_SideFX;
    FxInstantEvent(fxc, fx_palette_dark);
    FxInstantEvent(fxc, fx_speed_rst);
    FxInstantEvent(fxc, fx_speed_0);
    FxInstantEvent(fxc, fx_strip_all);
    FxInstantEvent(fxc, fx_transition_fast);
  }    
}

/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include <Arduino.h>
#include "DevNeo.h"
#include "FxEvent.h"
#include "FxPalette.h"

void FxEventPrint(int event)
{
  if (event >= fx_stripmask_0 && event <= fx_stripmask_255)
  {
    Serial.print(F("stripMask"));
    Serial.print((unsigned int)event-(unsigned int)fx_stripmask_0);
  }
    
  switch(event)
  {
    case fx_sidefx_full_reset: Serial.print(F("Full SideFX Reset"));break;
    case fx_palanim_speed_0: Serial.print(F("x0"));break;
    case fx_palanim_speed_1: Serial.print(F("x1"));break;
    case fx_palanim_speed_2: Serial.print(F("x2"));break;
    case fx_palanim_speed_3: Serial.print(F("x3"));break;
    case fx_palanim_speed_4: Serial.print(F("x4"));break;
    case fx_palanim_speed_5: Serial.print(F("x5"));break;
    case fx_palanim_speed_6: Serial.print(F("x6"));break;
    case fx_palanim_speed_7: Serial.print(F("x7"));break;
    case fx_palanim_speed_8: Serial.print(F("x8"));break;
    case fx_palanim_speed_9: Serial.print(F("x9"));break;
    case fx_palanim_speed_10: Serial.print(F("x10"));break;
    case fx_palanim_speed_11: Serial.print(F("x11"));break;
    case fx_palanim_speed_12: Serial.print(F("x12"));break;
    case fx_palanim_speed_13: Serial.print(F("x13"));break;
    case fx_palanim_speed_14: Serial.print(F("x14"));break;
    case fx_palanim_speed_15: Serial.print(F("x15"));break;
    case fx_palanim_speed_16: Serial.print(F("x16"));break;
    case fx_palanim_speed_17: Serial.print(F("x17"));break;
    case fx_palanim_speed_18: Serial.print(F("x18"));break;
    case fx_palanim_speed_32: Serial.print(F("x32"));break;

    case fx_state_default:   Serial.print(F("StateDefault"));break;
    case fx_state_test:      Serial.print(F("StateTest"));break;
    case fx_state_multitest: Serial.print(F("StateMultiTest"));break;
    case fx_state_sidefx:    Serial.print(F("StateSideFX"));break;

    case fx_brightness_half:   Serial.print(F("bright half"));break;
    case fx_brightness_normal: Serial.print(F("bright normal"));break;
    case fx_brightness_max:    Serial.print(F("bright max"));break;

    case fx_palanim_speed_pos: Serial.print(F("palanim speed pos"));break;
    case fx_palanim_speed_neg: Serial.print(F("palanim speed neg"));break;
    case fx_palanim_speed_inc: Serial.print(F("palanim speed inc"));break;
    case fx_palanim_speed_dec: Serial.print(F("palanim speed dec"));break;
    case fx_palanim_speed_rst: Serial.print(F("palanim speed rst"));break;
    case fx_palanim_location_rst: Serial.print(F("palanim location rst"));break;

    case fx_palette_type_smoothed: Serial.print(F("pal-smoothed"));break;
    case fx_palette_type_literal: Serial.print(F("pal-literal"));break;
    case fx_palette_type_literal2: Serial.print(F("pal-literal2"));break;
    case fx_palette_type_literal3: Serial.print(F("pal-literal3"));break;
    case fx_palette_type_literal4: Serial.print(F("pal-literal4"));break;
    
    case fx_transition_fast: Serial.print(F("t-fast"));break;
    case fx_transition_timed_fade:Serial.print(F("t-timed-fade"));break;
    case fx_transition_timed_wipe_pos:Serial.print(F("t-timed-wipe-pos"));break;
    case fx_transition_timed_wipe_neg:Serial.print(F("t-timed-wipe-neg"));break;
    case fx_transition_timed_wipe_outin:Serial.print(F("t-timed-wipe-outin"));break;
    case fx_transition_timed_wipe_inout:Serial.print(F("t-timed-wipe-inout"));break;
    case fx_transition_timed_wipe_random:Serial.print(F("t-timed-wipe-rnd"));break;
    case fx_transition_timed_fade_sin:Serial.print(F("t-timed-wipe-sine"));break;    
    case fx_transition_timed_fade_cos:Serial.print(F("t-timed-wipe-sine"));break;    
    
    case fx_palette_lead:    Serial.print(F("lead"));break;    
    case fx_palette_follow:  Serial.print(F("follow"));break;       

    case fx_palette_lava: Serial.print(F("lava"));break;
    case fx_palette_cloud: Serial.print(F("cloud"));break;
    case fx_palette_ocean: Serial.print(F("ocean"));break;
    case fx_palette_forest: Serial.print(F("forest"));break;
    case fx_palette_rainbow: Serial.print(F("rainbow"));break;
    case fx_palette_rainbowstripe: Serial.print(F("rainbowstripe"));break;
    case fx_palette_party: Serial.print(F("party"));break;
    case fx_palette_heat: Serial.print(F("heat"));break;

    case fx_palette_dark:    Serial.print(F("dark"));break;
    case fx_palette_white:   Serial.print(F("white"));break;
    case fx_palette_red:     Serial.print(F("red"));break;
    case fx_palette_yellow:  Serial.print(F("yellow"));break;
    case fx_palette_green:   Serial.print(F("green"));break;
    case fx_palette_cyan:    Serial.print(F("cyan"));break;
    case fx_palette_blue:    Serial.print(F("blue"));break;
    case fx_palette_magenta: Serial.print(F("magenta"));break;
    case fx_palette_orange:  Serial.print(F("orange"));break;
    case fx_palette_half:    Serial.print(F("half"));break;
    case fx_palette_lowhalf: Serial.print(F("lowhalf"));break;
    
    case fx_palette_pulse_dark:    Serial.print(F("pulse-dark"));break;
    case fx_palette_pulse_white:   Serial.print(F("pulse-white"));break;
    case fx_palette_pulse_red:     Serial.print(F("pulse-red"));break;
    case fx_palette_pulse_yellow:  Serial.print(F("pulse-yellow"));break;
    case fx_palette_pulse_green:   Serial.print(F("pulse-green"));break;
    case fx_palette_pulse_cyan:    Serial.print(F("pulse-cyan"));break;
    case fx_palette_pulse_blue:    Serial.print(F("pulse-blue"));break;
    case fx_palette_pulse_magenta: Serial.print(F("pulse-magenta"));break;
    case fx_palette_pulse_orange:  Serial.print(F("pulse-orange"));break;
    case fx_palette_pulse_half:  Serial.print(F("pulse-half"));break;
    case fx_palette_pulse_lowhalf:  Serial.print(F("pulse-lowhalf"));break;

    case fx_palette_pulse2_dark:    Serial.print(F("pulse2-dark"));break;
    case fx_palette_pulse2_white:   Serial.print(F("pulse2-white"));break;
    case fx_palette_pulse2_red:     Serial.print(F("pulse2-red"));break;
    case fx_palette_pulse2_yellow:  Serial.print(F("pulse2-yellow"));break;
    case fx_palette_pulse2_green:   Serial.print(F("pulse2-green"));break;
    case fx_palette_pulse2_cyan:    Serial.print(F("pulse2-cyan"));break;
    case fx_palette_pulse2_blue:    Serial.print(F("pulse2-blue"));break;
    case fx_palette_pulse2_magenta: Serial.print(F("pulse2-magenta"));break;
    case fx_palette_pulse2_orange:  Serial.print(F("pulse2-orange"));break;
    case fx_palette_pulse2_half:  Serial.print(F("pulse2-half"));break;
    case fx_palette_pulse2_lowhalf:  Serial.print(F("pulse2-lowhalf"));break;

    case fx_palette_pulse3_dark:    Serial.print(F("pulse3-dark"));break;
    case fx_palette_pulse3_white:   Serial.print(F("pulse3-white"));break;
    case fx_palette_pulse3_red:     Serial.print(F("pulse3-red"));break;
    case fx_palette_pulse3_yellow:  Serial.print(F("pulse3-yellow"));break;
    case fx_palette_pulse3_green:   Serial.print(F("pulse3-green"));break;
    case fx_palette_pulse3_cyan:    Serial.print(F("pulse3-cyan"));break;
    case fx_palette_pulse3_blue:    Serial.print(F("pulse3-blue"));break;
    case fx_palette_pulse3_magenta: Serial.print(F("pulse3-magenta"));break;
    case fx_palette_pulse3_orange:  Serial.print(F("pulse3-orange"));break;
    case fx_palette_pulse3_half:    Serial.print(F("pulse3-half"));break;
    case fx_palette_pulse3_lowhalf: Serial.print(F("pulse3-lowhalf"));break;    

    case fx_palette_pulse4_dark:    Serial.print(F("pulse4-dark"));break;
    case fx_palette_pulse4_white:   Serial.print(F("pulse4-white"));break;
    case fx_palette_pulse4_red:     Serial.print(F("pulse4-red"));break;
    case fx_palette_pulse4_yellow:  Serial.print(F("pulse4-yellow"));break;
    case fx_palette_pulse4_green:   Serial.print(F("pulse4-green"));break;
    case fx_palette_pulse4_cyan:    Serial.print(F("pulse4-cyan"));break;
    case fx_palette_pulse4_blue:    Serial.print(F("pulse4-blue"));break;
    case fx_palette_pulse4_magenta: Serial.print(F("pulse4-magenta"));break;
    case fx_palette_pulse4_orange:  Serial.print(F("pulse4-orange"));break;
    case fx_palette_pulse4_half:    Serial.print(F("pulse4-half"));break;
    case fx_palette_pulse4_lowhalf: Serial.print(F("pulse4-lowhalf"));break;   
    case fx_palette_dw: Serial.print(F("dark-white"));break;
    case fx_palette_dr: Serial.print(F("dark-red"));break;
    case fx_palette_dy: Serial.print(F("dark-yellow"));break;
    case fx_palette_dg: Serial.print(F("dark-green"));break;
    case fx_palette_dc: Serial.print(F("dark-cyan"));break;
    case fx_palette_db: Serial.print(F("dark-blue"));break;
    case fx_palette_dm: Serial.print(F("dark-magenta"));break;
    case fx_palette_wr: Serial.print(F("white-red"));break;
    case fx_palette_wy: Serial.print(F("white-yellow"));break;
    case fx_palette_wg: Serial.print(F("white-green"));break;
    case fx_palette_wc: Serial.print(F("white-cyan"));break;
    case fx_palette_wb: Serial.print(F("white-blue"));break;
    case fx_palette_wm: Serial.print(F("white-magenta"));break;
    case fx_palette_ry: Serial.print(F("red-yellow"));break;
    case fx_palette_rg: Serial.print(F("red-green"));break;
    case fx_palette_rc: Serial.print(F("red-cyan"));break;
    case fx_palette_rb: Serial.print(F("red-blue"));break;
    case fx_palette_rm: Serial.print(F("red-magenta"));break;
    case fx_palette_yg: Serial.print(F("yellow-green"));break;
    case fx_palette_yc: Serial.print(F("yellow-cyan"));break;
    case fx_palette_yb: Serial.print(F("yellow-blue"));break;
    case fx_palette_ym: Serial.print(F("yellow-magenta"));break;
    case fx_palette_gc: Serial.print(F("green-cyan"));break;
    case fx_palette_gb: Serial.print(F("green-blue"));break;
    case fx_palette_gm: Serial.print(F("green-magenta"));break;
    case fx_palette_cb: Serial.print(F("cyan-blue"));break;
    case fx_palette_cm: Serial.print(F("cyan-magenta"));break;
    case fx_palette_bm: Serial.print(F("blue-magenta"));break;

    case fx_palette_wry:Serial.print(F("wry"));break;
    case fx_palette_wrg:Serial.print(F("wrg"));break;
    case fx_palette_wrc:Serial.print(F("wrc"));break;
    case fx_palette_wrb:Serial.print(F("wrb"));break;
    case fx_palette_wrm:Serial.print(F("wrm"));break;
    case fx_palette_wyg:Serial.print(F("wyg"));break;
    case fx_palette_wyc:Serial.print(F("wyc"));break;
    case fx_palette_wyb:Serial.print(F("wyb"));break;
    case fx_palette_wym:Serial.print(F("wym"));break;
    case fx_palette_wgc:Serial.print(F("wgc"));break;
    case fx_palette_wgb:Serial.print(F("wgb"));break;
    case fx_palette_wgm:Serial.print(F("wgm"));break;
    case fx_palette_wcb:Serial.print(F("wcb"));break;
    case fx_palette_wcm:Serial.print(F("wcm"));break;
    case fx_palette_wbm:Serial.print(F("wbm"));break;

    case fx_palette_dry:Serial.print(F("dry"));break;
    case fx_palette_drg:Serial.print(F("drg"));break;
    case fx_palette_drc:Serial.print(F("drc"));break;
    case fx_palette_drb:Serial.print(F("drb"));break;
    case fx_palette_drm:Serial.print(F("drm"));break;
    case fx_palette_dyg:Serial.print(F("dyg"));break;
    case fx_palette_dyc:Serial.print(F("dyc"));break;
    case fx_palette_dyb:Serial.print(F("dyb"));break;
    case fx_palette_dym:Serial.print(F("dym"));break;
    case fx_palette_dgc:Serial.print(F("dgc"));break;
    case fx_palette_dgb:Serial.print(F("dgb"));break;
    case fx_palette_dgm:Serial.print(F("dgm"));break;
    case fx_palette_dcb:Serial.print(F("dcb"));break;
    case fx_palette_dcm:Serial.print(F("dcm"));break;
    case fx_palette_dbm:Serial.print(F("dbm"));break;

    case fx_palette_rgb:Serial.print(F("rgb"));break;
    case fx_palette_rbm:Serial.print(F("rbm"));break;
    case fx_palette_cmy:Serial.print(F("cmy"));break;
    case fx_palette_cbm:Serial.print(F("cbm"));break;
    
    case fx_strip_all:  Serial.print(F("strip-all"));break;
    case fx_strip_none: Serial.print(F("strip-none"));break;
    case fx_strip_odds: Serial.print(F("strip-odds"));break;
    case fx_strip_evens:Serial.print(F("strip-evens"));break;
    case fx_strip0:     Serial.print(F("strip0"));break;
    case fx_strip1:     Serial.print(F("strip1"));break;
    case fx_strip2:     Serial.print(F("strip2"));break;
    case fx_strip3:     Serial.print(F("strip3"));break;
    case fx_strip4:     Serial.print(F("strip4"));break;
    case fx_strip5:     Serial.print(F("strip5"));break;
    case fx_strip6:     Serial.print(F("strip6"));break;
    case fx_strip7:     Serial.print(F("strip7"));break;

    case fx_particles_off: Serial.print(F("particles_off"));break;
    case fx_particles_pos: Serial.print(F("particles_pos"));break;
    case fx_particles_neg: Serial.print(F("particles_neg"));break;
    case fx_particles_rnd: Serial.print(F("particles_rnd"));break;
    case fx_particles_length_1: Serial.print(F("particles_len1"));break;
    case fx_particles_length_2: Serial.print(F("particles_len2"));break;
    case fx_particles_length_3: Serial.print(F("particles_len3"));break;
    case fx_particles_length_4: Serial.print(F("particles_len4"));break;
    case fx_particles_color_dark:   Serial.print(F("particles_color_dark"));break;
    case fx_particles_color_white:  Serial.print(F("particles_color_white"));break;
    case fx_particles_color_red:    Serial.print(F("particles_color_wred"));break;
    case fx_particles_color_yellow: Serial.print(F("particles_color_yellow"));break;
    case fx_particles_color_green : Serial.print(F("particles_color_green"));break;
    case fx_particles_color_cyan:   Serial.print(F("particles_color_cyan"));break;
    case fx_particles_color_blue:   Serial.print(F("particles_color_blue"));break;
    case fx_particles_color_magenta:Serial.print(F("particles_color_magenta"));break;
    case fx_particles_color_orange: Serial.print(F("particles_color_orange"));break;
    case fx_particles_color_half:   Serial.print(F("particles_color_half"));break;
    case fx_particles_color_lowhalf:Serial.print(F("particles_color_lowhalf"));break;
      
    case fx_nothing:Serial.print(F("nothing"));break;
  }
  Serial.println();
}

void FxEvent(FxController &fxc, int event)
{
  bool updatePal = false;

  if (event >= fx_stripmask_0 && event <= fx_stripmask_255)
  {
    fxc.stripMask = (unsigned int)event-(unsigned int)fx_stripmask_0;
    return;
  }
  FxEventPrint(event);

  switch (event)
  {
    case fx_strip_all:   fxc.stripMask = (unsigned int)(LEDS_0|LEDS_1|LEDS_2|LEDS_3|LEDS_4|LEDS_5|LEDS_6|LEDS_7); fxc.SetParticlesLoc(0);break;
    case fx_strip_odds:  fxc.stripMask = (unsigned int)(LEDS_1|LEDS_3|LEDS_5|LEDS_7); break;
    case fx_strip_evens: fxc.stripMask = (unsigned int)(LEDS_0|LEDS_2|LEDS_4|LEDS_6); break;
    case fx_strip_none:  fxc.stripMask = (unsigned int)0; break;
    case fx_strip_inv:   fxc.stripMask = (unsigned int)~(fxc.stripMask); break;
    
    case fx_palanim_speed_0:
    case fx_palanim_speed_1:
    case fx_palanim_speed_2:
    case fx_palanim_speed_3:
    case fx_palanim_speed_4:
    case fx_palanim_speed_5:
    case fx_palanim_speed_6:
    case fx_palanim_speed_7:
    case fx_palanim_speed_8:
    case fx_palanim_speed_9:
    case fx_palanim_speed_10:
    case fx_palanim_speed_11:
    case fx_palanim_speed_12:
    case fx_palanim_speed_13:
    case fx_palanim_speed_14:
    case fx_palanim_speed_15:
    case fx_palanim_speed_16:
    case fx_palanim_speed_17:
    case fx_palanim_speed_18:
      fxc.SetPaletteSpeed(event-fx_palanim_speed_0);
      break;
    case fx_palanim_speed_32:
      fxc.SetPaletteSpeed(32);
      break;

    case fx_state_default:  fxc.fxState = FxState_Default; break;
    case fx_state_test:     fxc.fxState = FxState_TestPattern; break;
    case fx_state_multitest:fxc.fxState = FxState_MultiTestPattern; break;
    case fx_state_sidefx:   fxc.fxState = FxState_SideFX; break;

    case fx_brightness_half:
      fxc.brightness = BRIGHTNESS/2;
      for (int strip=0;strip<NUM_STRIPS;strip++)
        neopixelSetBrightness(strip,BRIGHTNESS/2);
      break;
    case fx_brightness_normal:
      fxc.brightness = BRIGHTNESS;
      for (int strip=0;strip<NUM_STRIPS;strip++)
        neopixelSetBrightness(strip,BRIGHTNESS);
      break;
    case fx_brightness_max:
      fxc.brightness = BRIGHTNESS_LIMIT;
      for (int strip=0;strip<NUM_STRIPS;strip++)
        neopixelSetBrightness(strip,BRIGHTNESS_LIMIT);
      break;

    case fx_sidefx_full_reset:
    {    
      fxc.fxState = FxState_SideFX;
      fxc.stripMask = (unsigned int)(LEDS_0|LEDS_1|LEDS_2|LEDS_3|LEDS_4|LEDS_5|LEDS_6|LEDS_7); fxc.SetParticlesLoc(0);break;
      fxc.KillFX();
      fxc.ResetPaletteSpeed();
      fxc.SetPaletteSpeed(fx_palanim_speed_0);
      fxc.SetTransitionType(Transition_Instant);
      fxc.transitionMux = 0.0f;
      PaletteCreateSingleColor(fxc, DARK);
      FxUpdatePalette(fxc);
      break;    
    }

    case fx_palanim_speed_pos:    fxc.SetPaletteDirection(1);break;
    case fx_palanim_speed_neg:    fxc.SetPaletteDirection(-1);break;
    case fx_palanim_speed_inc:    fxc.ChangePaletteSpeed(1);break;
    case fx_palanim_speed_dec:    fxc.ChangePaletteSpeed(-1);break;
    case fx_palanim_speed_rst:    fxc.ResetPaletteSpeed();break;
    case fx_palanim_location_rst: fxc.ResetPaletteLocation();break;
      
    case fx_transition_fast:              fxc.SetTransitionType(Transition_Instant);break;
    case fx_transition_timed_fade:        fxc.SetTransitionType(Transition_TimedFade);break;
    case fx_transition_timed_wipe_pos:    fxc.SetTransitionType(Transition_TimedWipePos);break;
    case fx_transition_timed_wipe_neg:    fxc.SetTransitionType(Transition_TimedWipeNeg);break;
    case fx_transition_timed_wipe_outin:  fxc.SetTransitionType(Transition_TimedWipeOutIn);break;
    case fx_transition_timed_wipe_inout:  fxc.SetTransitionType(Transition_TimedWipeInOut);break;
    case fx_transition_timed_wipe_random: fxc.SetTransitionType(Transition_TimedWipeRandom);break;
    case fx_transition_timed_fade_sin:    fxc.SetTransitionType(Transition_TimedFadeSin);break;
    case fx_transition_timed_fade_cos:    fxc.SetTransitionType(Transition_TimedFadeCos);break;

    case fx_palette_lead:   PaletteCreateSingleColor(fxc, BLUE);break;
    case fx_palette_follow: PaletteCreateSingleColor(fxc, RED);break;
    
    case fx_palette_dark:   PaletteCreateSingleColor(fxc, DARK);break;
    case fx_palette_white:  PaletteCreateSingleColor(fxc, WHITE);break;
    case fx_palette_red:    PaletteCreateSingleColor(fxc, RED);break;
    case fx_palette_yellow: PaletteCreateSingleColor(fxc, YELLOW);break;
    case fx_palette_green:  PaletteCreateSingleColor(fxc, GREEN);break;
    case fx_palette_cyan:   PaletteCreateSingleColor(fxc, CYAN);break;
    case fx_palette_blue:   PaletteCreateSingleColor(fxc, BLUE);break;
    case fx_palette_magenta:PaletteCreateSingleColor(fxc, MAGENTA);break;
    case fx_palette_orange: PaletteCreateSingleColor(fxc, ORANGE);break;
    case fx_palette_half:   PaletteCreateSingleColor(fxc, HALF);break;
    case fx_palette_lowhalf:PaletteCreateSingleColor(fxc, LOWHALF);break;

    case fx_palette_pulse_dark:   PaletteCreateSinglePulseBand(fxc, DARK);break;
    case fx_palette_pulse_white:  PaletteCreateSinglePulseBand(fxc, WHITE);break;
    case fx_palette_pulse_red:    PaletteCreateSinglePulseBand(fxc, RED);break;
    case fx_palette_pulse_yellow: PaletteCreateSinglePulseBand(fxc, YELLOW);break;
    case fx_palette_pulse_green:  PaletteCreateSinglePulseBand(fxc, GREEN);break;
    case fx_palette_pulse_cyan:   PaletteCreateSinglePulseBand(fxc, CYAN);break;
    case fx_palette_pulse_blue:   PaletteCreateSinglePulseBand(fxc, BLUE);break;
    case fx_palette_pulse_magenta:PaletteCreateSinglePulseBand(fxc, MAGENTA);break;
    case fx_palette_pulse_orange: PaletteCreateSinglePulseBand(fxc, ORANGE);break;
    case fx_palette_pulse_half:   PaletteCreateSinglePulseBand(fxc, HALF);break;
    case fx_palette_pulse_lowhalf:PaletteCreateSinglePulseBand(fxc, LOWHALF);break;

    case fx_palette_pulse2_dark:  PaletteCreateDoublePulseBand(fxc, DARK);break;
    case fx_palette_pulse2_white: PaletteCreateDoublePulseBand(fxc, WHITE);break;
    case fx_palette_pulse2_red:   PaletteCreateDoublePulseBand(fxc, RED);break;
    case fx_palette_pulse2_yellow:PaletteCreateDoublePulseBand(fxc, YELLOW);break;
    case fx_palette_pulse2_green: PaletteCreateDoublePulseBand(fxc, GREEN);break;
    case fx_palette_pulse2_cyan:  PaletteCreateDoublePulseBand(fxc, CYAN);break;
    case fx_palette_pulse2_blue:  PaletteCreateDoublePulseBand(fxc, BLUE);break;
    case fx_palette_pulse2_magenta:PaletteCreateDoublePulseBand(fxc, MAGENTA);break;
    case fx_palette_pulse2_orange:PaletteCreateDoublePulseBand(fxc, ORANGE);break;
    case fx_palette_pulse2_half:PaletteCreateDoublePulseBand(fxc, HALF);break;
    case fx_palette_pulse2_lowhalf:PaletteCreateDoublePulseBand(fxc, LOWHALF);break;    

    case fx_palette_pulse3_dark:  PaletteCreateTriplePulseBand(fxc, DARK);break;
    case fx_palette_pulse3_white: PaletteCreateTriplePulseBand(fxc, WHITE);break;
    case fx_palette_pulse3_red:   PaletteCreateTriplePulseBand(fxc, RED);break;
    case fx_palette_pulse3_yellow:PaletteCreateTriplePulseBand(fxc, YELLOW);break;
    case fx_palette_pulse3_green: PaletteCreateTriplePulseBand(fxc, GREEN);break;
    case fx_palette_pulse3_cyan:  PaletteCreateTriplePulseBand(fxc, CYAN);break;
    case fx_palette_pulse3_blue:  PaletteCreateTriplePulseBand(fxc, BLUE);break;
    case fx_palette_pulse3_magenta:PaletteCreateTriplePulseBand(fxc, MAGENTA);break;
    case fx_palette_pulse3_orange:  PaletteCreateTriplePulseBand(fxc, ORANGE);break;
    case fx_palette_pulse3_half:    PaletteCreateTriplePulseBand(fxc, HALF);break;
    case fx_palette_pulse3_lowhalf: PaletteCreateTriplePulseBand(fxc, LOWHALF);break;    

    case fx_palette_pulse4_dark:PaletteCreateQuadPulseBand(fxc, DARK);break;
    case fx_palette_pulse4_white:PaletteCreateQuadPulseBand(fxc, WHITE);break;
    case fx_palette_pulse4_red:PaletteCreateQuadPulseBand(fxc, RED);break;
    case fx_palette_pulse4_yellow:PaletteCreateQuadPulseBand(fxc, YELLOW);break;
    case fx_palette_pulse4_green:PaletteCreateQuadPulseBand(fxc, GREEN);break;
    case fx_palette_pulse4_cyan:PaletteCreateQuadPulseBand(fxc, CYAN);break;
    case fx_palette_pulse4_blue:PaletteCreateQuadPulseBand(fxc, BLUE);break;
    case fx_palette_pulse4_magenta:PaletteCreateQuadPulseBand(fxc, MAGENTA);break;
    case fx_palette_pulse4_orange:PaletteCreateQuadPulseBand(fxc, ORANGE);break;
    case fx_palette_pulse4_half:PaletteCreateQuadPulseBand(fxc, HALF);break;
    case fx_palette_pulse4_lowhalf:PaletteCreateQuadPulseBand(fxc, LOWHALF);break;    

    case fx_palette_dr: PaletteCreateDoubleColor(fxc, DARK, RED); break;
    case fx_palette_dy: PaletteCreateDoubleColor(fxc, DARK, YELLOW); break;
    case fx_palette_dg: PaletteCreateDoubleColor(fxc, DARK, GREEN); break;
    case fx_palette_dc: PaletteCreateDoubleColor(fxc, DARK, CYAN); break;
    case fx_palette_db: PaletteCreateDoubleColor(fxc, DARK, BLUE); break;
    case fx_palette_dm: PaletteCreateDoubleColor(fxc, DARK, MAGENTA); break;
    case fx_palette_wr: PaletteCreateDoubleColor(fxc, WHITE, RED); break;
    case fx_palette_wy: PaletteCreateDoubleColor(fxc, WHITE, YELLOW); break;
    case fx_palette_wg: PaletteCreateDoubleColor(fxc, WHITE, GREEN); break;
    case fx_palette_wc: PaletteCreateDoubleColor(fxc, WHITE, CYAN); break;
    case fx_palette_wb: PaletteCreateDoubleColor(fxc, WHITE, BLUE); break;
    case fx_palette_wm: PaletteCreateDoubleColor(fxc, WHITE, MAGENTA); break;
    case fx_palette_ry: PaletteCreateDoubleColor(fxc, RED, YELLOW); break;
    case fx_palette_rg: PaletteCreateDoubleColor(fxc, RED, GREEN); break;
    case fx_palette_rc: PaletteCreateDoubleColor(fxc, RED, CYAN); break;
    case fx_palette_rb: PaletteCreateDoubleColor(fxc, RED, BLUE); break;
    case fx_palette_rm: PaletteCreateDoubleColor(fxc, RED, MAGENTA); break;
    case fx_palette_yg: PaletteCreateDoubleColor(fxc, YELLOW, GREEN); break;
    case fx_palette_yc: PaletteCreateDoubleColor(fxc, YELLOW, CYAN); break;
    case fx_palette_yb: PaletteCreateDoubleColor(fxc, YELLOW, BLUE); break;
    case fx_palette_ym: PaletteCreateDoubleColor(fxc, YELLOW, MAGENTA); break;
    case fx_palette_gc: PaletteCreateDoubleColor(fxc, GREEN, CYAN); break;
    case fx_palette_gb: PaletteCreateDoubleColor(fxc, GREEN, BLUE); break;
    case fx_palette_gm: PaletteCreateDoubleColor(fxc, GREEN, MAGENTA); break;
    case fx_palette_cb: PaletteCreateDoubleColor(fxc, CYAN, BLUE); break;
    case fx_palette_cm: PaletteCreateDoubleColor(fxc, CYAN, MAGENTA); break;
    case fx_palette_bm: PaletteCreateDoubleColor(fxc, BLUE, MAGENTA); break;

    case fx_palette_wry: PaletteCreateQuadColor(fxc, WHITE, RED,    WHITE, YELLOW); break;
    case fx_palette_wrg: PaletteCreateQuadColor(fxc, WHITE, RED,    WHITE, GREEN); break;
    case fx_palette_wrc: PaletteCreateQuadColor(fxc, WHITE, RED,    WHITE, CYAN); break;
    case fx_palette_wrb: PaletteCreateQuadColor(fxc, WHITE, RED,    WHITE, BLUE); break;
    case fx_palette_wrm: PaletteCreateQuadColor(fxc, WHITE, RED,    WHITE, MAGENTA); break;
    case fx_palette_wyg: PaletteCreateQuadColor(fxc, WHITE, YELLOW, WHITE, GREEN); break;
    case fx_palette_wyc: PaletteCreateQuadColor(fxc, WHITE, YELLOW, WHITE, CYAN); break;
    case fx_palette_wyb: PaletteCreateQuadColor(fxc, WHITE, YELLOW, WHITE, BLUE); break;
    case fx_palette_wym: PaletteCreateQuadColor(fxc, WHITE, YELLOW, WHITE, MAGENTA); break;
    case fx_palette_wgc: PaletteCreateQuadColor(fxc, WHITE, GREEN,  WHITE, CYAN); break;
    case fx_palette_wgb: PaletteCreateQuadColor(fxc, WHITE, GREEN,  WHITE, BLUE); break;
    case fx_palette_wgm: PaletteCreateQuadColor(fxc, WHITE, GREEN,  WHITE, MAGENTA); break;
    case fx_palette_wcb: PaletteCreateQuadColor(fxc, WHITE, CYAN,   WHITE, BLUE); break;
    case fx_palette_wcm: PaletteCreateQuadColor(fxc, WHITE, CYAN,   WHITE, MAGENTA); break;
    case fx_palette_wbm: PaletteCreateQuadColor(fxc, WHITE, BLUE,   WHITE, MAGENTA); break;

    case fx_palette_dry: PaletteCreateQuadColor(fxc, DARK, RED,    DARK, YELLOW); break;
    case fx_palette_drg: PaletteCreateQuadColor(fxc, DARK, RED,    DARK, GREEN); break;
    case fx_palette_drc: PaletteCreateQuadColor(fxc, DARK, RED,    DARK, CYAN); break;
    case fx_palette_drb: PaletteCreateQuadColor(fxc, DARK, RED,    DARK, BLUE); break;
    case fx_palette_drm: PaletteCreateQuadColor(fxc, DARK, RED,    DARK, MAGENTA); break;
    case fx_palette_dyg: PaletteCreateQuadColor(fxc, DARK, YELLOW, DARK, GREEN); break;
    case fx_palette_dyc: PaletteCreateQuadColor(fxc, DARK, YELLOW, DARK, CYAN); break;
    case fx_palette_dyb: PaletteCreateQuadColor(fxc, DARK, YELLOW, DARK, BLUE); break;
    case fx_palette_dym: PaletteCreateQuadColor(fxc, DARK, YELLOW, DARK, MAGENTA); break;
    case fx_palette_dgc: PaletteCreateQuadColor(fxc, DARK, GREEN,  DARK, CYAN); break;
    case fx_palette_dgb: PaletteCreateQuadColor(fxc, DARK, GREEN,  DARK, BLUE); break;
    case fx_palette_dgm: PaletteCreateQuadColor(fxc, DARK, GREEN,  DARK, MAGENTA); break;
    case fx_palette_dcb: PaletteCreateQuadColor(fxc, DARK, CYAN,   DARK, BLUE); break;
    case fx_palette_dcm: PaletteCreateQuadColor(fxc, DARK, CYAN,   DARK, MAGENTA); break;
    case fx_palette_dbm: PaletteCreateQuadColor(fxc, DARK, BLUE,   DARK, MAGENTA); break;

    case fx_palette_rbm: PaletteCreateQuadColor(fxc, RED, MAGENTA, BLUE,    DARK); break;
    case fx_palette_rgb: PaletteCreateQuadColor(fxc, RED, GREEN, BLUE,      DARK); break;
    case fx_palette_cmy: PaletteCreateQuadColor(fxc, CYAN, MAGENTA, YELLOW, DARK); break;
    case fx_palette_cbm: PaletteCreateQuadColor(fxc, CYAN, BLUE, MAGENTA,   DARK); break;

    case fx_palette_lava:PaletteCreate16Color(fxc,
        WEBRGB::Black,WEBRGB::Maroon,WEBRGB::Black,WEBRGB::Maroon,    
        WEBRGB::DarkRed,WEBRGB::Maroon,WEBRGB::DarkRed,WEBRGB::DarkRed,    
        WEBRGB::DarkRed,WEBRGB::DarkRed,WEBRGB::Red,WEBRGB::Orange,    
        WEBRGB::White,WEBRGB::Orange,WEBRGB::Red,WEBRGB::DarkRed);
      break;
    case fx_palette_cloud:PaletteCreate16Color(fxc,  
      WEBRGB::Blue,WEBRGB::DarkBlue,WEBRGB::DarkBlue,WEBRGB::DarkBlue,
      WEBRGB::DarkBlue,WEBRGB::DarkBlue,WEBRGB::DarkBlue,WEBRGB::DarkBlue,
      WEBRGB::Blue,WEBRGB::DarkBlue,WEBRGB::SkyBlue,WEBRGB::SkyBlue,
      WEBRGB::LightBlue,WEBRGB::White,WEBRGB::LightBlue,WEBRGB::SkyBlue);
      break;
    case fx_palette_ocean:PaletteCreate16Color(fxc,    
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
    case fx_palette_forest:PaletteCreate16Color(fxc,    
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
    case fx_palette_rainbow:PaletteCreate16Color(fxc, 
      0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00,
      0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
      0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
      0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B);break;
    case fx_palette_rainbowstripe:PaletteCreate16Color(fxc,    
      0xFF0000, 0x000000, 0xAB5500, 0x000000,
      0xABAB00, 0x000000, 0x00FF00, 0x000000,
      0x00AB55, 0x000000, 0x0000FF, 0x000000,
      0x5500AB, 0x000000, 0xAB0055, 0x000000);break;
    case fx_palette_party:PaletteCreate16Color(fxc,   
      0x5500AB, 0x84007C, 0xB5004B, 0xE5001B,
      0xE81700, 0xB84700, 0xAB7700, 0xABAB00,
      0xAB5500, 0xDD2200, 0xF2000E, 0xC2003E,
      0x8F0071, 0x5F00A1, 0x2F00D0, 0x0007F9);break;
    case fx_palette_heat:PaletteCreate16Color(fxc,
      0x000000, 0x330000, 0x660000, 0x990000, 
      0xCC0000, 0xFF0000, 0xFF3300, 0xFF6600, 
      0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33, 
      0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF);break;
    case fx_palette_seahawks:PaletteCreateQuadColor(fxc, 
      0x00,0x7F,0x00,
      0x00,0x00,0xFF,
      0x1f,0x1f,0x1F,
      0x00,0x00,0x7F);break;

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

    case fx_palette_type_smoothed:
      fxc.SetPaletteType(FxPaletteType::Smoothed);      
      break;
      
    case fx_palette_type_literal:  fxc.SetPaletteType(FxPaletteType::Literal); break;
    case fx_palette_type_literal2: fxc.SetPaletteType(FxPaletteType::Literal2); break;
    case fx_palette_type_literal3: fxc.SetPaletteType(FxPaletteType::Literal3); break;
    case fx_palette_type_literal4: fxc.SetPaletteType(FxPaletteType::Literal4); break;

    case fx_strip0: fxc.stripMask = LEDS_0; break;
    case fx_strip1: fxc.stripMask = LEDS_1; break;
    case fx_strip2: fxc.stripMask = LEDS_2; break;
    case fx_strip3: fxc.stripMask = LEDS_3; break;
    case fx_strip4: fxc.stripMask = LEDS_4; break;
    case fx_strip5: fxc.stripMask = LEDS_5; break;
    case fx_strip6: fxc.stripMask = LEDS_6; break;
    case fx_strip7: fxc.stripMask = LEDS_7; break;
 }
}
  

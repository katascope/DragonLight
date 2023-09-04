/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef FX_EVENT_DEF
#define FX_EVENT_DEF
#include <Arduino.h>

struct FxController;

//List of possible Fx events
enum FxEvent
{
  fx_palanim_speed_0  = 0,
  fx_palanim_speed_1  = 1,
  fx_palanim_speed_2  = 2,
  fx_palanim_speed_3  = 3,
  fx_palanim_speed_4  = 4,
  fx_palanim_speed_5  = 5,
  fx_palanim_speed_6  = 6,
  fx_palanim_speed_7  = 7,
  fx_palanim_speed_8  = 8,
  fx_palanim_speed_9  = 9,
  fx_palanim_speed_10 = 10,
  fx_palanim_speed_11 = 11,
  fx_palanim_speed_12 = 12,
  fx_palanim_speed_13 = 13,
  fx_palanim_speed_14 = 14,
  fx_palanim_speed_15 = 15,
  fx_palanim_speed_16 = 16,
  fx_palanim_speed_17 = 17,
  fx_palanim_speed_18 = 18,
  fx_palanim_speed_32 = 19,

  fx_palanim_speed_pos,
  fx_palanim_speed_neg,
  fx_palanim_speed_inc,
  fx_palanim_speed_dec,
  fx_palanim_speed_rst,
  fx_palanim_location_rst,

  fx_state_default,
  fx_state_test,
  fx_state_multitest,
  fx_state_sidefx,

  fx_brightness_half,
  fx_brightness_normal,
  fx_brightness_max,

  fx_transition_fast,
  fx_transition_timed_fade,
  fx_transition_timed_wipe_pos,
  fx_transition_timed_wipe_neg,
  fx_transition_timed_wipe_outin,
  fx_transition_timed_wipe_inout,
  fx_transition_timed_wipe_random,

  fx_transition_timed_fade_sin,
  fx_transition_timed_fade_cos,

  fx_sidefx_full_reset,
  
  fx_palette_lead,
  fx_palette_follow,

  fx_palette_lava,
  fx_palette_cloud,
  fx_palette_ocean,
  fx_palette_forest,
  fx_palette_rainbow,
  fx_palette_rainbowstripe,
  fx_palette_party,
  fx_palette_heat,
  fx_palette_seahawks,

  fx_palette_dark,
  fx_palette_white,
  fx_palette_red,
  fx_palette_yellow,
  fx_palette_green,
  fx_palette_cyan,
  fx_palette_blue,
  fx_palette_magenta,
  fx_palette_orange,
  fx_palette_half,
  fx_palette_lowhalf,

  fx_palette_pulse_dark,
  fx_palette_pulse_white,
  fx_palette_pulse_red,
  fx_palette_pulse_yellow,
  fx_palette_pulse_green,
  fx_palette_pulse_cyan,
  fx_palette_pulse_blue,
  fx_palette_pulse_magenta,
  fx_palette_pulse_orange,
  fx_palette_pulse_half,
  fx_palette_pulse_lowhalf,

  fx_palette_pulse2_dark,
  fx_palette_pulse2_white,
  fx_palette_pulse2_red,
  fx_palette_pulse2_yellow,
  fx_palette_pulse2_green,
  fx_palette_pulse2_cyan,
  fx_palette_pulse2_blue,
  fx_palette_pulse2_magenta,
  fx_palette_pulse2_orange,
  fx_palette_pulse2_half,
  fx_palette_pulse2_lowhalf,  

  fx_palette_pulse3_dark,
  fx_palette_pulse3_white,
  fx_palette_pulse3_red,
  fx_palette_pulse3_yellow,
  fx_palette_pulse3_green,
  fx_palette_pulse3_cyan,
  fx_palette_pulse3_blue,
  fx_palette_pulse3_magenta,
  fx_palette_pulse3_orange,
  fx_palette_pulse3_half,
  fx_palette_pulse3_lowhalf,  

  fx_palette_pulse4_dark,
  fx_palette_pulse4_white,
  fx_palette_pulse4_red,
  fx_palette_pulse4_yellow,
  fx_palette_pulse4_green,
  fx_palette_pulse4_cyan,
  fx_palette_pulse4_blue,
  fx_palette_pulse4_magenta,
  fx_palette_pulse4_orange,
  fx_palette_pulse4_half,
  fx_palette_pulse4_lowhalf,    
  
  fx_palette_dw,
  fx_palette_dr,
  fx_palette_dy,
  fx_palette_dg,
  fx_palette_dc,
  fx_palette_db,
  fx_palette_dm,
  fx_palette_wr,
  fx_palette_wy,
  fx_palette_wg,
  fx_palette_wc,
  fx_palette_wb,
  fx_palette_wm,
  fx_palette_ry,
  fx_palette_rg,
  fx_palette_rc,
  fx_palette_rb,
  fx_palette_rm,
  fx_palette_yg,
  fx_palette_yc,
  fx_palette_yb,
  fx_palette_ym,
  fx_palette_gc,
  fx_palette_gb,
  fx_palette_gm,
  fx_palette_cb,
  fx_palette_cm,
  fx_palette_bm,

  fx_palette_wry,
  fx_palette_wrg,
  fx_palette_wrc,
  fx_palette_wrb,
  fx_palette_wrm,
  fx_palette_wyg,
  fx_palette_wyc,
  fx_palette_wyb,
  fx_palette_wym,
  fx_palette_wgc,
  fx_palette_wgb,
  fx_palette_wgm,
  fx_palette_wcb,
  fx_palette_wcm,
  fx_palette_wbm,

  fx_palette_dry,
  fx_palette_drg,
  fx_palette_drc,
  fx_palette_drb,
  fx_palette_drm,
  fx_palette_dyg,
  fx_palette_dyc,
  fx_palette_dyb,
  fx_palette_dym,
  fx_palette_dgc,
  fx_palette_dgb,
  fx_palette_dgm,
  fx_palette_dcb,
  fx_palette_dcm,
  fx_palette_dbm,

  fx_palette_rbm,
  fx_palette_rgb,
  fx_palette_cmy,
  fx_palette_cbm,

  fx_particles_off,
  fx_particles_pos,
  fx_particles_neg,
  fx_particles_rnd,
  fx_particles_length_1,
  fx_particles_length_2,
  fx_particles_length_3,
  fx_particles_length_4,

  fx_particles_color_dark,
  fx_particles_color_white,
  fx_particles_color_red,
  fx_particles_color_yellow,
  fx_particles_color_green,
  fx_particles_color_cyan,
  fx_particles_color_blue,
  fx_particles_color_magenta,
  fx_particles_color_orange,
  fx_particles_color_half,
  fx_particles_color_lowhalf,    

  fx_palette_type_smoothed,
  fx_palette_type_literal,
  fx_palette_type_literal2,
  fx_palette_type_literal3,
  fx_palette_type_literal4,
    
  fx_strip_all,  //Target all strips
  fx_strip_none, //Target no strips, why?
  fx_strip_evens, //Target even strips
  fx_strip_odds, //Target odd strips
  fx_strip_inv,  //Target 'opposing' strips
  
  fx_stripmask_0, fx_strip = fx_stripmask_0,//make a syntactic sugar for it
  fx_stripmask_1,fx_stripmask_2,fx_stripmask_3,fx_stripmask_4,fx_stripmask_5,fx_stripmask_6,fx_stripmask_7,fx_stripmask_8,  
  fx_stripmask_9,fx_stripmask_10,fx_stripmask_11,fx_stripmask_12,fx_stripmask_13,fx_stripmask_14,fx_stripmask_15,fx_stripmask_16,
  fx_stripmask_17,fx_stripmask_18,fx_stripmask_19,fx_stripmask_20,fx_stripmask_21,fx_stripmask_22,fx_stripmask_23,fx_stripmask_24,
  fx_stripmask_25,fx_stripmask_26,fx_stripmask_27,fx_stripmask_28,fx_stripmask_29,fx_stripmask_30,fx_stripmask_31,fx_stripmask_32,
  fx_stripmask_33,fx_stripmask_34,fx_stripmask_35,fx_stripmask_36,fx_stripmask_37,fx_stripmask_38,fx_stripmask_39,fx_stripmask_40,
  fx_stripmask_41,fx_stripmask_42,fx_stripmask_43,fx_stripmask_44,fx_stripmask_45,fx_stripmask_46,fx_stripmask_47,fx_stripmask_48,
  fx_stripmask_49,fx_stripmask_50,fx_stripmask_51,fx_stripmask_52,fx_stripmask_53,fx_stripmask_54,fx_stripmask_55,fx_stripmask_56,
  fx_stripmask_57,fx_stripmask_58,fx_stripmask_59,fx_stripmask_60,fx_stripmask_61,fx_stripmask_62,fx_stripmask_63,fx_stripmask_64,
  fx_stripmask_65,fx_stripmask_66,fx_stripmask_67,fx_stripmask_68,fx_stripmask_69,fx_stripmask_70,fx_stripmask_71,fx_stripmask_72,
  fx_stripmask_73,fx_stripmask_74,fx_stripmask_75,fx_stripmask_76,fx_stripmask_77,fx_stripmask_78,fx_stripmask_79,fx_stripmask_80,
  fx_stripmask_81,fx_stripmask_82,fx_stripmask_83,fx_stripmask_84,fx_stripmask_85,fx_stripmask_86,fx_stripmask_87,fx_stripmask_88,
  fx_stripmask_89,fx_stripmask_90,fx_stripmask_91,fx_stripmask_92,fx_stripmask_93,fx_stripmask_94,fx_stripmask_95,fx_stripmask_96,
  fx_stripmask_97,fx_stripmask_98,fx_stripmask_99,fx_stripmask_100,fx_stripmask_101,fx_stripmask_102,fx_stripmask_103,fx_stripmask_104,
  fx_stripmask_105,fx_stripmask_106,fx_stripmask_107,fx_stripmask_108,fx_stripmask_109,fx_stripmask_110,fx_stripmask_111,fx_stripmask_112,
  fx_stripmask_113,fx_stripmask_114,fx_stripmask_115,fx_stripmask_116,fx_stripmask_117,fx_stripmask_118,fx_stripmask_119,fx_stripmask_120,
  fx_stripmask_121,fx_stripmask_122,fx_stripmask_123,fx_stripmask_124,fx_stripmask_125,fx_stripmask_126,fx_stripmask_127,fx_stripmask_128,
  fx_stripmask_129,fx_stripmask_130,fx_stripmask_131,fx_stripmask_132,fx_stripmask_133,fx_stripmask_134,fx_stripmask_135,fx_stripmask_136,
  fx_stripmask_137,fx_stripmask_138,fx_stripmask_139,fx_stripmask_140,fx_stripmask_141,fx_stripmask_142,fx_stripmask_143,fx_stripmask_144,
  fx_stripmask_145,fx_stripmask_146,fx_stripmask_147,fx_stripmask_148,fx_stripmask_149,fx_stripmask_150,fx_stripmask_151,fx_stripmask_152,
  fx_stripmask_153,fx_stripmask_154,fx_stripmask_155,fx_stripmask_156,fx_stripmask_157,fx_stripmask_158,fx_stripmask_159,fx_stripmask_160,
  fx_stripmask_161,fx_stripmask_162,fx_stripmask_163,fx_stripmask_164,fx_stripmask_165,fx_stripmask_166,fx_stripmask_167,fx_stripmask_168,
  fx_stripmask_169,fx_stripmask_170,fx_stripmask_171,fx_stripmask_172,fx_stripmask_173,fx_stripmask_174,fx_stripmask_175,fx_stripmask_176,
  fx_stripmask_177,fx_stripmask_178,fx_stripmask_179,fx_stripmask_180,fx_stripmask_181,fx_stripmask_182,fx_stripmask_183,fx_stripmask_184,
  fx_stripmask_185,fx_stripmask_186,fx_stripmask_187,fx_stripmask_188,fx_stripmask_189,fx_stripmask_190,fx_stripmask_191,fx_stripmask_192,
  fx_stripmask_193,fx_stripmask_194,fx_stripmask_195,fx_stripmask_196,fx_stripmask_197,fx_stripmask_198,fx_stripmask_199,fx_stripmask_200,
  fx_stripmask_201,fx_stripmask_202,fx_stripmask_203,fx_stripmask_204,fx_stripmask_205,fx_stripmask_206,fx_stripmask_207,fx_stripmask_208,
  fx_stripmask_209,fx_stripmask_210,fx_stripmask_211,fx_stripmask_212,fx_stripmask_213,fx_stripmask_214,fx_stripmask_215,fx_stripmask_216,
  fx_stripmask_217,fx_stripmask_218,fx_stripmask_219,fx_stripmask_220,fx_stripmask_221,fx_stripmask_222,fx_stripmask_223,fx_stripmask_224,
  fx_stripmask_225,fx_stripmask_226,fx_stripmask_227,fx_stripmask_228,fx_stripmask_229,fx_stripmask_230,fx_stripmask_231,fx_stripmask_232,
  fx_stripmask_233,fx_stripmask_234,fx_stripmask_235,fx_stripmask_236,fx_stripmask_237,fx_stripmask_238,fx_stripmask_239,fx_stripmask_240,
  fx_stripmask_241,fx_stripmask_242,fx_stripmask_243,fx_stripmask_244,fx_stripmask_245,fx_stripmask_246,fx_stripmask_247,fx_stripmask_248,
  fx_stripmask_249,fx_stripmask_250,fx_stripmask_251,fx_stripmask_252,fx_stripmask_253,fx_stripmask_254,fx_stripmask_255,//Same as fx_MULTI_FINISH

  fx_strip0 = fx_stripmask_1,    
  fx_strip1 = fx_stripmask_2,    
  fx_strip2 = fx_stripmask_4,    
  fx_strip3 = fx_stripmask_8,    
  fx_strip4 = fx_stripmask_16,    
  fx_strip5 = fx_stripmask_32,    
  fx_strip6 = fx_stripmask_64,    
  fx_strip7 = fx_stripmask_128,   

  fx_nothing = 0xFFFF
};

void FxEventPrint(int event);
void FxEvent(FxController &fxc,int event);

#endif

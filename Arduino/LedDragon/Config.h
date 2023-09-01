/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef CONFIG_DEF
#define CONFIG_DEF

#define LEDDRAGON 0
#define LEAD      0 //Generally LightSuit*A silver
#define FOLLOW    0 //Generally LightSuit*B gold
#define FOXBOT    0 //Generally LightSuit*B gold
#define MREE      0
#define PREE      0
#define JACKET    0
#define LAMP      1
#define TESTER    0
#define MTESTER   0

#if SUIT_JACKET
static const char *DeviceName = "LightSuitAngelTest";
#elif LEDDRAGON
static const char *DeviceName = "LedDragon";
#elif LAMP
static const char *DeviceName = "LedLamp";
#elif LEAD
static const char *DeviceName = "LightSuitAngelLead";
#elif FOLLOW
static const char *DeviceName = "LightSuitAngelB";
#elif MREE
static const char *DeviceName = "Mree";
#elif PREE
static const char *DeviceName = "Pree";
#elif MTESTER
static const char *DeviceName = "MultiTester";
#else
static const char *DeviceName = "UnkDevice";
#endif


#define STARTUP_STATE       FxState_Default    //FxState_Default;//FxState_TestPattern;//FxState_PlayingTrack;//
#define UPDATE_DELAY        30 //100 //Milliseconds to delay between palette updates, set to 100 for iot, 30 for ble
#define NUM_STRIPS          8
#define BRIGHTNESS          50  //Default brightness
#define BRIGHTNESS_LIMIT    100 //Maximum allowed brightness, //90 possible but runs too hot
#define ENABLE_SAFETY       0
#define ENABLE_TRACK_CHECK  1
#define ENABLE_BLE          1
#define ENABLE_NEOPIXEL     1
#define ENABLE_MULTISTRIP   1   //Enable multiple LED strips
#define LED_PIN             3
#define DEBUG_STATUS        0
#define DEBUG_SERIAL        1
#define SERIAL_BAUD_RATE    115200
#define DRAGON_HEAD         LEDS_2|LEDS_3
#define DRAGON_TORSO_LEFT   LEDS_0
#define DRAGON_TORSO_RIGHT  LEDS_1
#define DRAGON_WING_LEFT    LEDS_4
#define DRAGON_WING_RIGHT   LEDS_5
#define DRAGON_TAIL         LEDS_6|LEDS_7

#if MTESTER
#define NUM_LEDS_0          199
#define NUM_LEDS_1          199
#define NUM_LEDS_2          199
#define NUM_LEDS_3          199
#define NUM_LEDS_4          199
#define NUM_LEDS_5          199
#define NUM_LEDS_6          199
#define NUM_LEDS_7          199
#undef LED_PIN
#define LED_PIN             3
#endif

#if LEDDRAGON
#undef STARTUP_STATE 
#define STARTUP_STATE       FxState_SideFX
#define NUM_LEDS_0          199
#define NUM_LEDS_1          199
#define NUM_LEDS_2          199
#define NUM_LEDS_3          199
#define NUM_LEDS_4          199
#define NUM_LEDS_5          199
#define NUM_LEDS_6          199
#define NUM_LEDS_7          199
#endif

#if LEAD
#define NUM_LEDS_0          53
#define NUM_LEDS_1          53
#define NUM_LEDS_2          76
#define NUM_LEDS_3          76
#define NUM_LEDS_4          48 //arms
#define NUM_LEDS_5          48
#define NUM_LEDS_6          61 //legs
#define NUM_LEDS_7          61
#endif

#if TESTER
#define NUM_LEDS_0          60
#define NUM_LEDS_1          10
#define NUM_LEDS_2          10
#define NUM_LEDS_3          10
#define NUM_LEDS_4          10 //arms
#define NUM_LEDS_5          10
#define NUM_LEDS_6          10 //legs
#define NUM_LEDS_7          10
#undef LED_PIN
#define LED_PIN             3
#undef ENABLE_BLE
#define ENABLE_BLE          0
#undef STARTUP_STATE
#define STARTUP_STATE       FxState_TestPattern
#undef NUM_STRIPS
#define NUM_STRIPS          8
#undef ENABLE_MULTISTRIP
#define ENABLE_MULTISTRIP   0
#endif

#if FOLLOW
#define NUM_LEDS_0          49
#define NUM_LEDS_1          49
#define NUM_LEDS_2          71
#define NUM_LEDS_3          71
#define NUM_LEDS_4          40 //arms
#define NUM_LEDS_5          40
#define NUM_LEDS_6          53 //legs
#define NUM_LEDS_7          53
#endif

#if SUIT_JACKET
#define NUM_LEDS_0          46
#define NUM_LEDS_1          46
#define NUM_LEDS_2          36
#define NUM_LEDS_3          36
#define NUM_LEDS_4          38 //arms
#define NUM_LEDS_5          38
#define NUM_LEDS_6          35 //legs
#define NUM_LEDS_7          35
#endif

#if FOXBOT
#define NUM_LEDS_0          32
#define NUM_LEDS_1          32
#define NUM_LEDS_2          36
#define NUM_LEDS_3          36
#define NUM_LEDS_4          38 //arms
#define NUM_LEDS_5          38
#define NUM_LEDS_6          35 //legs
#define NUM_LEDS_7          35
#endif



#if LAMP
#define NUM_LEDS_0          86
#define NUM_LEDS_1          86
#define NUM_LEDS_2          86
#define NUM_LEDS_3          86
#define NUM_LEDS_4          86
#define NUM_LEDS_5          86
#define NUM_LEDS_6          86
#define NUM_LEDS_7          86
#undef STARTUP_STATE
#define STARTUP_STATE       FxState_SideFX
#undef BRIGHTNESS
#define BRIGHTNESS          50  //Default brightness
#undef BRIGHTNESS_LIMIT 
#define BRIGHTNESS_LIMIT    100 //Maximum allowed brightness, //90 possible but runs too hot
#undef NUM_STRIPS
#define NUM_STRIPS          8
#undef ENABLE_MULTISTRIP
#define ENABLE_MULTISTRIP   1
#endif

#if ENABLE_MEMORYUSAGE
#include <MemoryUsage.h>
#endif
#endif

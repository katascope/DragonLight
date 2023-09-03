/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "FxPalette.h"
#include "FxController.h"
#include "Cmd.h"
#include "State.h"
#include "DevBle.h"
#include "DevNeo.h"

static FxController fxController;

static unsigned long lastTimeDisplay = 0;
static unsigned long lastTimePoll = 0;
static unsigned long lockedFPS = 60;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.setTimeout(50);
  Serial.print(DeviceName);
  Serial.print(F("Serial init: "));
  Serial.println(SERIAL_BAUD_RATE);

#if ENABLE_MEMORYUSAGE
  Serial.println(F("MemoryUsage"));
  MEMORY_PRINT_STACKSIZE
  MEMORY_PRINT_HEAPSIZE
#endif  

  fxController.fxState = STARTUP_STATE;

#if ENABLE_NEOPIXEL
  Serial.println(F("Delaying 3 seconds for LEDs."));  delay( 3000 ); // power-up safety delay
  neopixelSetup();
  Serial.print(F("NeoPixel LEDS on pin"));
  Serial.println(LED_PIN);
  
  Serial.print(F("NeoPixel Brightness = { "));
  FxEvent(fxController, fx_palette_dark);  
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {
    fxController.strip[strip]->brightness = BRIGHTNESS;
    neopixelSetBrightness(strip,fxController.strip[strip]->brightness);
    Serial.print(fxController.strip[strip]->brightness);
    Serial.print(F(" "));
  }
  Serial.println(F(" }"));
#else
  Serial.println(F("No NeoPixel init"));
#endif

#if ENABLE_BLE
  Serial.println(F("BLE init"));
  bleSetup();
#else
  Serial.println(F("No BLE init"));
#endif

  Serial.println(F("Setup complete."));
}

void loop()
{
  while (Serial.available())
  {
      String str = Serial.readString();
      if (str.length()<=3) SimpleUserCommandInput(fxController, (int)str[0]);
      else ComplexUserCommandInput(fxController, str);
  }

#if ENABLE_BLE
  blePoll(fxController);
#endif

  bool needsUpdate = false;

  if (millis()-lastTimePoll > (1000/lockedFPS))
  {
    needsUpdate = true;
    State_Poll(fxController);   
    lastTimePoll = millis(); 
  }
  
  if (fxController.transitionMux < 1.0f)
  {
    fxController.SetTransitionType(Transition_TimedFade);
    fxController.transitionMux += 0.1f;
    if (fxController.transitionMux > 1.0f)
      fxController.transitionMux = 1.0f;
    Do_Transition(fxController);
  } 

  if (needsUpdate)
  {
    unsigned long t =  millis();
    if (t - fxController.lastTimeLedUpdate > UPDATE_DELAY)//delay to let bluetooth get data(fastled issue)
    {
      FxUpdatePalette(fxController);      
      fxController.lastTimeLedUpdate = t;
    }
  }

#if DEBUG_STATUS
  //Display status on timed basis
  unsigned long currentTime = millis();
  if (currentTime - lastTimeDisplay > DEBUG_STATUS_RATE)//delay to let bluetooth get data
  {      
    fxController.PrintStatus();
    lastTimeDisplay = currentTime;
  }
#endif
}

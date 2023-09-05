/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include "FxPalette.h"
#include "FxController.h"
#include "DevBle.h"
#include "SideFX.h"

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#if ENABLE_BLE
#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
#include "FxPalette.h"
#include "Cmd.h"
const int BLE_LED_PIN = LED_BUILTIN;
const int RSSI_LED_PIN = 25;//LED_PWR;
BLEService mainService( BLE_UUID_SERVICE );
BLEUnsignedLongCharacteristic authenticateCharacteristic( BLE_UUID_CHARACTERISTIC_AUTHENTICATE, BLEWrite);
BLEUnsignedLongCharacteristic statusCharacteristic( BLE_UUID_CHARACTERISTIC_STATUS, BLEWrite | BLERead | BLENotify );
BLECharCharacteristic commandCharacteristic( BLE_UUID_CHARACTERISTIC_COMMAND, BLERead | BLEWrite  );
BLECharCharacteristic soundCharacteristic( BLE_UUID_CHARACTERISTIC_SOUND,  BLERead | BLEWrite | BLEWriteWithoutResponse );
BLECharCharacteristic paletteCharacteristic( BLE_UUID_CHARACTERISTIC_PALETTE, BLERead | BLEWrite  );
BLECharCharacteristic fxToggleCharacteristic( BLE_UUID_CHARACTERISTIC_FXTOGGLE, BLERead | BLEWrite  );
BLECharCharacteristic fxToggleOnCharacteristic( BLE_UUID_CHARACTERISTIC_FXTOGGLEON, BLERead | BLEWrite  );
BLECharCharacteristic fxToggleOffCharacteristic( BLE_UUID_CHARACTERISTIC_FXTOGGLEOFF, BLERead | BLEWrite  );
BLECharCharacteristic fxExciteCharacteristic( BLE_UUID_CHARACTERISTIC_FXEXCITE, BLERead | BLEWrite  );
BLECharCharacteristic fxResetCharacteristic( BLE_UUID_CHARACTERISTIC_FXRESET, BLERead | BLEWrite  );
BLECharCharacteristic fxSideFXPresetCharacteristic( BLE_UUID_CHARACTERISTIC_FXPRESET, BLERead | BLEWrite  );

//BLEDevice * central = NULL;
void blePeripheralConnectHandler(BLEDevice c) {
  //central = &c;
#if DEBUG_BLE  
  // central connected event handler
  Serial.print(F("Connected event, central: "));
  Serial.println(c.address());
#endif 
}
void blePeripheralDisconnectHandler(BLEDevice c) {
  //central = NULL;
#if DEBUG_BLE  
  // central disconnected event handler
  Serial.print(F("Disconnected event, central: "));
  Serial.println(c.address());
#endif  
}
bool bleSetup()
{
  pinMode( BLE_LED_PIN, OUTPUT );
  pinMode( RSSI_LED_PIN, OUTPUT );
  
  if ( !BLE.begin() )
  {
    return false;
  }

  // set advertised local name and service UUID:
  BLE.setDeviceName( DeviceName );
  BLE.setLocalName( DeviceName );
  BLE.setAdvertisedService( mainService );

  // BLE add characteristics
  mainService.addCharacteristic( authenticateCharacteristic );
  mainService.addCharacteristic( commandCharacteristic );
  mainService.addCharacteristic( soundCharacteristic );
  mainService.addCharacteristic( paletteCharacteristic );
  mainService.addCharacteristic( fxToggleCharacteristic );
  mainService.addCharacteristic( fxToggleOnCharacteristic );
  mainService.addCharacteristic( fxToggleOffCharacteristic );
  mainService.addCharacteristic( fxExciteCharacteristic );
  mainService.addCharacteristic( fxResetCharacteristic );
  mainService.addCharacteristic( fxSideFXPresetCharacteristic );
 
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  
  // add service
  BLE.addService( mainService );

  // set the initial value for the characeristics:
  commandCharacteristic.writeValue( 0 );
  statusCharacteristic.writeValue(0);
  soundCharacteristic.writeValue(127);
  paletteCharacteristic.writeValue(0);
  fxToggleCharacteristic.writeValue(0);
  fxToggleOnCharacteristic.writeValue(0);
  fxToggleOffCharacteristic.writeValue(0);
  fxExciteCharacteristic.writeValue(0);
  fxResetCharacteristic.writeValue(0);
  fxSideFXPresetCharacteristic.writeValue(0);
  // start advertising
  BLE.advertise();

  digitalWrite( BLE_LED_PIN, HIGH );    
  return true;
}


void blePoll(FxController &fxc)
{
  //static unsigned long counter = 0;
  //static long previousMillis = 0;

  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

#if DEBUG_BLE  
//  Serial.println(F("Waiting for connection."));
#endif  

  if ( central )
  {
#if DEBUG_BLE  
//    Serial.print( F("Connected to central: " ));
//    Serial.print( central.address() );
#endif
    if ( central.connected() )
    {
      //if( authenticateCharacteristic.value() == 3838) //authenticated
      {
        if (commandCharacteristic.written() )
        {   
          Serial.print( F("BLE cmd: " ));
          int v = commandCharacteristic.value();
          Serial.print( (char)v);
          SimpleUserCommandInput(fxc, v);
          Serial.println();
        }
        if (soundCharacteristic.written() )
        {
          fxc.vol = (float)soundCharacteristic.value() / (float)255;
        }
        if (paletteCharacteristic.written() )
        {
          int palChoice = paletteCharacteristic.value();
          Serial.print(F("PalChoice"));
          Serial.println(palChoice);
          PaletteCreateById(fxc,palChoice);
        }
        if (fxToggleCharacteristic.written() )
        {
          int channel = fxToggleCharacteristic.value();
          Serial.print(F("Toggle "));
          Serial.println(channel);
          fxc.Toggle(channel);
        }
        if (fxToggleOnCharacteristic.written() )
        {
          int channel = fxToggleOnCharacteristic.value();
          Serial.print(F("ToggleOn "));
          Serial.println(channel);
          fxc.ToggleOn(channel);
        }
        if (fxToggleOffCharacteristic.written() )
        {
          int channel = fxToggleOffCharacteristic.value();
          Serial.print(F("ToggleOff "));
          Serial.println(channel);
          fxc.ToggleOff(channel);
        }
        if (fxExciteCharacteristic.written() )
        {
          int channel = fxExciteCharacteristic.value();
          Serial.print(F("Excite "));
          Serial.println(channel);
          fxc.Excite(channel);
        }
        if (fxResetCharacteristic.written() )
        {
          int channel = fxResetCharacteristic.value();
          Serial.print(F("Reset "));
          Serial.println(channel);
          fxc.Reset(channel);
        }
        if (fxSideFXPresetCharacteristic.written() )
        {
          int preset = fxSideFXPresetCharacteristic.value();
          Serial.print(F("Preset "));
          Serial.println(preset);
          SideFXActivatePreset(fxc, preset);
        }
      }

    } // while connected

#if DEBUG_BLE  
    //Serial.print( F( "Disconnected from central: " ) );
//    Serial.println( central->address() );
#endif    
  } // if central
} // loop
#endif

/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef DEVICE_BLE_DEF
#define DEVICE_BLE_DEF

//////////////// BLE Section /////////////////////
#if ENABLE_BLE
#include "Fx.h"
// BLE UUIDs
#define BLE_UUID_SERVICE                      "02FE4875-5056-48B5-AD15-36E30665D9B4"
#define BLE_UUID_CHARACTERISTIC_AUTHENTICATE  "4C75BB42-5365-458D-A3EA-2B91339646B7"
#define BLE_UUID_CHARACTERISTIC_STATUS        "D01C9106-91BD-4998-9554-85264D33ACB2"
#define BLE_UUID_CHARACTERISTIC_COMMAND       "220154BF-1DCE-4F03-85F0-7BA905D2D6B0"
#define BLE_UUID_CHARACTERISTIC_PLAY          "3B140EF5-0A72-4891-AD38-83B5A2595622"
#define BLE_UUID_CHARACTERISTIC_SOUND         "04E92E16-47AF-11EE-BE56-0242AC120002"
#define BLE_UUID_CHARACTERISTIC_PALETTE       "7a0cca4f-9124-4831-923c-6ffdd6042ff2"
#define BLE_UUID_CHARACTERISTIC_FXTOGGLE      "fbdd5000-4b48-4a1a-9e40-98e84fd69245"
#define BLE_UUID_CHARACTERISTIC_FXTOGGLEON    "9677b246-7355-4130-a770-769cd4732677"
#define BLE_UUID_CHARACTERISTIC_FXTOGGLEOFF   "cb9b8b39-96a8-498d-8420-4c9fc558894d"
#define BLE_UUID_CHARACTERISTIC_FXEXCITE      "5549a237-ede8-4b5e-abb0-b233cebe0e52"
#define BLE_UUID_CHARACTERISTIC_FXRESET       "a8907f1f-09ea-4caf-8f73-3acfad5ace43"

bool bleSetup();
void blePoll(FxController &fxc);
#endif

#endif

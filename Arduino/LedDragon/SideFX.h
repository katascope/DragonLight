/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef FX_SIDEFX_DEF
#define FX_SIDEFX_DEF
struct FxController;

class FxChannel
{
public:  
  bool on = false;
  int state = 0;
};

#define NUM_FX_CHANNELS 100

class FxChannelSystem
{
public:
  FxChannel channels[NUM_FX_CHANNELS];
  void KillFX()
  {
    for (int channel=0;channel<NUM_FX_CHANNELS;channel++)
    {
      channels[channel].on = false;
      channels[channel].state = 0;
    }    
  }
  void Toggle(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    channels[channel].on = !channels[channel].on;      
  }
  void ToggleOn(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    channels[channel].on = true;
  }
  void ToggleOff(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    channels[channel].on = false;
  }
  void Excite(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    if (channel == 2)
    {
     channels[channel].state++;
     if (channels[channel].state > 2)
      channels[channel].state = 0;
    }
    if (channel == 4)    
    {
     channels[channel].state++;
     if (channels[channel].state > 2)
      channels[channel].state = 0;
    }
    if (channel == 5)
    {
     channels[channel].state++;
     if (channels[channel].state > 1)
      channels[channel].state = 0;
    }
    if (channel == 7)    
    {
     channels[channel].state++;
     if (channels[channel].state > 1)
      channels[channel].state = 0;
    }
  }
  void Reset(int channel)
  {
    if (channel >= NUM_FX_CHANNELS)  return;
    channels[channel].state = 0;
  }
};

void SideFXPollState(FxController &fxc);
void SideFXPrintState(FxController &fxc);
void SideFXActivatePreset(FxController &fxc, int trackId);

#endif

/*Copyright 6/14/2022 github/katascope
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "FxStrip.h"
#include "FxEvent.h"
#include "FxPalette.h"

FxStrip::FxStrip(int nl)
{
  numleds = nl;
  palette = (uint32_t *)malloc(sizeof(uint32_t) * numleds);
  nextPalette = (uint32_t *)malloc(sizeof(uint32_t) * numleds);
  initialPalette = (uint32_t *)malloc(sizeof(uint32_t) * numleds);
//  sideFXPalette = (uint32_t *)malloc(sizeof(uint32_t) * numleds);
  sequence = (int *)malloc(sizeof(sequence) * numleds);
  for (int i = 0; i < numleds; i++)
  {
    palette[i] = LEDRGB(0, 0, 0);
    nextPalette[i] = LEDRGB(0, 0, 0);
    initialPalette[i] = LEDRGB(0, 0, 0);
    //sideFXPalette[i] = LEDRGB(0, 0, 0);
    sequence[i] = -1;
  }
}

void sequence_linear(unsigned int arr[], int n)
{
  for (unsigned int i=0;i < n;i++)
    arr[i] = n;
}

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void sequence_random(int arr[], int n)
{
  //srand(time(NULL));
  for (int i = n - 1; i > 0; i--)
  {
    int j = rand() % (i + 1);
    swap(&arr[i], &arr[j]);
  }
}

void FxStrip::SetTransitionType(FxTransitionType t)
{
  transitionType = t;
  if (t == Transition_TimedWipePos || t == Transition_TimedWipeNeg
    || t == Transition_TimedWipeOutIn || t == Transition_TimedWipeInOut
    || t == Transition_TimedFadeSin || t == Transition_TimedFadeCos) 
    {
      paletteIndex = 0;
    }
  if (t == fx_transition_timed_wipe_neg)
    paletteIndex = 15;
    
  if (t == Transition_TimedWipeRandom)
  {
    paletteIndex = 0;
    //sequence_linear(fxc.strip[strip]->sequence, fxc.strip[strip]->numleds);
    sequence_random(sequence, numleds);
  }    
}

void FxStrip::SetPaletteType(FxPaletteType pt)
{
  paletteType = pt;
}
void FxStrip::SetParticlesLoc(int loc)
{
  for (int i = 0; i < NUM_PARTICLES; i++)
    particles[i].loc = loc;
}
void FxStrip::SetParticlesRunning(bool state)
{
  for (int i = 0; i < NUM_PARTICLES; i++)
    particles[i].on = state;
}
void FxStrip::SetParticlesLength(int len)
{
  for (int i = 0; i < NUM_PARTICLES; i++)
    particles[i].len = len;
}
void FxStrip::SetParticlesColor(uint32_t rgb)
{
  for (int i = 0; i < NUM_PARTICLES; i++)
    particles[i].rgb = rgb;
}
void FxStrip::SetParticlesDirection(int dir)//-1 or 1
{
  RandomizeParticles();
  for (int i = 0; i < NUM_PARTICLES; i++)
    particles[i].loc = dir * particles[i].loc;
}
void FxStrip::SetParticleMode(FxParticleMode mode)
{
  for (int i = 0; i < NUM_PARTICLES; i++)
    particles[i].mode = mode;
  RandomizeParticles();
}
void FxStrip::RandomizeParticles()
{
  for (int i = 0; i < NUM_PARTICLES; i++)
  {
    particles[i].loc = rand() % (numleds - 1);
    if (particles[i].mode == FX_PARTICLEMODE_RND)
      particles[i].loc = rand() % 10;
  }
}

void FxStrip::PrintTransitionName()
{
  switch (transitionType)
  {
    case Transition_Instant:         Serial.print(F("Fast")); return;
    case Transition_TimedFade:       Serial.print(F("Fade")); return;
    case Transition_TimedWipePos:    Serial.print(F("Wip+")); return;
    case Transition_TimedWipeNeg:    Serial.print(F("Wip-")); return;
    case Transition_TimedWipeOutIn:  Serial.print(F("WipOI")); return;
    case Transition_TimedWipeInOut:  Serial.print(F("WipIO")); return;
    case Transition_TimedWipeRandom: Serial.print(F("WipRnd")); return;
    case Transition_TimedFadeSin:    Serial.print(F("FadeSin")); return;
    case Transition_TimedFadeCos:    Serial.print(F("FadeCos")); return;
    default: Serial.print(F("Unknown"));
  }
}

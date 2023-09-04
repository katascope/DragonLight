#include "FxController.h"
#include "FxPalette.h"
#include "FxEvent.h"

FxController::FxController()
{
  for (int i=0;i<NUM_STRIPS;i++)
  {
    strip[i] = NULL;
    switch (i)
    {
      case 0: strip[i] = new FxStrip(NUM_LEDS_0); break;
#if ENABLE_MULTISTRIP
      case 1: strip[i] = new FxStrip(NUM_LEDS_1); break;
      case 2: strip[i] = new FxStrip(NUM_LEDS_2); break;
      case 3: strip[i] = new FxStrip(NUM_LEDS_3); break;
      case 4: strip[i] = new FxStrip(NUM_LEDS_4); break;
      case 5: strip[i] = new FxStrip(NUM_LEDS_5); break;
      case 6: strip[i] = new FxStrip(NUM_LEDS_6); break;
      case 7: strip[i] = new FxStrip(NUM_LEDS_7); break;
#endif
    }
  }
}
void FxController::Toggle(int channel)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->fxSystem.Toggle(channel);
}
void FxController::ToggleOn(int channel)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->fxSystem.ToggleOn(channel);
}
void FxController::ToggleOff(int channel)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->fxSystem.ToggleOff(channel);
}
void FxController::Excite(int channel)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->fxSystem.Excite(channel);
}
void FxController::Reset(int channel)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->fxSystem.Reset(channel);
}
void FxController::KillFX()
{
  for (int s=0;s<NUM_STRIPS;s++)
  {
    strip[s]->fxSystem.KillFX();
    for (int i=0;i<strip[s]->numleds;i++)
    {
      strip[s]->palette[i] = LEDRGB(0,0,0);
      strip[s]->nextPalette[i] = LEDRGB(0,0,0);
      strip[s]->initialPalette[i] = LEDRGB(0,0,0);
      strip[s]->sequence[i] = -1;
    }
  }
}
void FxController::SetPaletteType(FxPaletteType paletteType)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->SetPaletteType(paletteType);
}
void FxController::SetTransitionType(FxTransitionType transitionType)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->SetTransitionType(transitionType);
}
void FxController::SetParticlesLoc(int loc)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))
      strip[s]->SetParticlesLoc(loc);
}
void FxController::SetParticlesRunning(bool isRunning)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->SetParticlesRunning(isRunning);
}  
void FxController::SetParticlesColor(uint32_t rgb)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->SetParticlesColor(rgb);
}  
void FxController::SetParticlesDirection(int dir)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->SetParticlesDirection(dir);
}  
void FxController::SetParticlesLength(int len)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->SetParticlesLength(len);
}
void FxController::SetParticleMode(FxParticleMode mode)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s))   
      strip[s]->SetParticleMode(mode);
}
void FxController::ResetPaletteLocation()
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s)) 
      strip[s]->paletteIndex = 0;
}
void FxController::ResetPaletteSpeed()
{
  for (int s=0;s<NUM_STRIPS;s++)
  {
    if (stripMask & (1<<s)) 
    {
      strip[s]->paletteSpeed = 0;
      strip[s]->paletteIndex = 0;
      strip[s]->paletteDirection = 1;
    }
  }
}
void FxController::SetPaletteSpeed(int v)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s)) 
      strip[s]->paletteSpeed = v;
}
void FxController::ChangePaletteSpeed(int ps)
{
  for (int s=0;s<NUM_STRIPS;s++)
    if (stripMask & (1<<s)) 
    {
      strip[s]->paletteSpeed += ps;
      if (strip[s]->paletteSpeed < 0)
       strip[s]->paletteSpeed = 0;
    }
}

void FxController::SetPaletteDirection(int c)
{
  for (int s=0;s<NUM_STRIPS;s++)
  {
    if (stripMask & (1<<s)) 
    {
      strip[s]->paletteDirection = c;
      if (strip[s]->paletteSpeed >= 18)
        strip[s]->paletteSpeed = 18;
      if (strip[s]->paletteSpeed < 0)
        strip[s]->paletteSpeed = 0;
    }
  }
}

void FxController::PrintStateName()
{
  switch (fxState)
  {
    case FxState_Default:           Serial.print(F("Norm")); break;
    case FxState_TestPattern:       Serial.print(F("Test")); break;
    case FxState_MultiTestPattern:  Serial.print(F("MultiTest")); break;
    case FxState_SideFX:            Serial.print(F("SideFX")); break;
    default: Serial.print(F("Unk"));
  }
}

void FxController::PrintStatus()
{
  Serial.print(F("v="));
  Serial.print(vol);
  Serial.print(F("[state="));
  PrintStateName();
  Serial.print(F(",strip&="));
  Serial.print(stripMask);
  Serial.print(F(",mux="));
  Serial.print(transitionMux);

#if ENABLE_NEOPIXEL  
  for (int s=0;s<NUM_STRIPS;s++)
  {
    Serial.print(F("["));
    strip[s]->PrintTransitionName();
    Serial.print(F(",b="));
    Serial.print(strip[s]->brightness);
    Serial.print(F(",ps="));
    Serial.print(strip[s]->paletteSpeed);
    Serial.print(F(",pd="));
    Serial.print(strip[s]->paletteDirection);
    Serial.print(F(",pi="));
    Serial.print(strip[s]->paletteIndex);
    Serial.print(F("] "));
  }
#endif    
  Serial.print(F(")]"));
  Serial.println();
}

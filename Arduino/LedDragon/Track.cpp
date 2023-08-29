/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#include "Config.h"
#include <Arduino.h>
#include <avr/pgmspace.h> 
#include "Fx.h"
#include "Track.h"
#include "FxCore.h"
#include "Time.h"

int songTrackContext = 0;
void SetSongTrackContext(int id) { songTrackContext = id; }
int GetNumSongTracks()
{
  switch (songTrackContext)
  {
    case 0: return sizeof(SongTrackRGB) / (sizeof(unsigned long) * 2); 
    case 1: return sizeof(SongTrackCMY) / (sizeof(unsigned long) * 2); 
    case 2: return sizeof(SongTrackWD) / (sizeof(unsigned long) * 2); 
    case 3: return sizeof(SongTrackRYGCBM) / (sizeof(unsigned long) * 2); 
  }
}
unsigned long SongTrack_read(int i, int o)
{
  switch (songTrackContext)
  {
    case 0: return pgm_read_dword(&(SongTrackRGB[i*2+o])); 
    case 1: return pgm_read_dword(&(SongTrackCMY[i*2+o])); 
    case 2: return pgm_read_dword(&(SongTrackWD[i*2+o])); 
    case 3: return pgm_read_dword(&(SongTrackRYGCBM[i*2+o])); 
  }
}
unsigned long SongTrack_timecode(int i) { return SongTrack_read(i,0); }
unsigned long SongTrack_event(int i)    { return SongTrack_read(i,1); }

/*
int GetNumSongTracks()                  { return sizeof(SongTrackRGB) / (sizeof(unsigned long) * 2); }
unsigned long SongTrack_timecode(int i) { return pgm_read_dword(&(SongTrackRGB[i*2+0])); } 
unsigned long SongTrack_event(int i)    { return pgm_read_dword(&(SongTrackRGB[i*2+1])); }
*/
unsigned long GetFinalTimeCodeEntry() { return SongTrack_timecode(GetNumSongTracks()-1); }
int GetNextTimeCodeMatch(int currentMatch) { 
  unsigned long tc = SongTrack_timecode(currentMatch); 
  for (int i=0;i<GetNumSongTracks();i++) if (SongTrack_timecode(i) > tc) return i; return 0; 
}
int GetCurrentTimeCodeMatch(unsigned long timecode) { int match = 0; for (int i=0;i<GetNumSongTracks();i++) { if (SongTrack_timecode(i) <= timecode) match = i; } return match; }
int GetPreviousTimeCodeMatch(unsigned long timecode) { int match = 0; for (int i=0;i<GetNumSongTracks();i++) { if (SongTrack_timecode(i) < timecode) match = i; } return match; }

void FxTrackSay(unsigned long timecode, unsigned long matchedTimecode,unsigned long nextMatchedTimecode)
{
    float tc = (float)matchedTimecode;// / (float)1000.0f;
    Serial.print(tc);
    Serial.print(F(" :"));
    for (int i=0;i<GetNumSongTracks();i++)
    {
      if (SongTrack_timecode(i) == matchedTimecode)
      {
        Serial.print(F(" "));
        PrintFxEventName(SongTrack_event(i));
      }
    }
    Serial.print(F(", next("));
    Serial.print(nextMatchedTimecode);
    Serial.print(F(")= "));
    for (int i=0;i<GetNumSongTracks();i++)
    {
      if (SongTrack_timecode(i) == nextMatchedTimecode)
      {
          Serial.print(F(" "));
          Serial.print(SongTrack_event(i));
          PrintFxEventName(SongTrack_event(i));
      }
    }
    float timeUntil = (float)(nextMatchedTimecode - (float)timecode) / 1000.0f;
    Serial.print(F(" in "));
    Serial.print(timeUntil);
    Serial.print(F("s"));
    Serial.println();
}


void trackStart(FxController &fxc,unsigned long tc, unsigned long tcOffset, FxTrackEndAction trackEndAction)
{
  fxc.fxState = FxState_PlayingTrack;

  for (int strip=0;strip<NUM_STRIPS;strip++)
  {    
    fxc.strip[strip]->paletteSpeed = 0;
    fxc.strip[strip]->paletteDirection = 1;
    fxc.strip[strip]->transitionType = Transition_Instant;
    fxc.strip[strip]->paletteUpdateType = FxPaletteUpdateType::Once;    
  }
  fxc.transitionMux = 0;
  fxc.fxTrackEndAction = trackEndAction;
  setTimecodeLastMatched(tc);
  setTimecodeTimeOffset((unsigned long)(millis() - (signed long)TRACK_START_DELAY));
  Serial.print(F("Playing Track"));
  Serial.print(F(", Time Offset = "));
  Serial.println(getTimecodeTimeOffset());
}

void trackStop(FxController &fxc)
{
  fxc.fxState = FxState_Default;
  for (int strip=0;strip<NUM_STRIPS;strip++)
  {    
    fxc.strip[strip]->paletteUpdateType = FxPaletteUpdateType::None; 
    fxc.strip[strip]->SetParticlesRunning(false);
  }
  fxc.stripMask = 0xFF;
  //Serial.println(F("Stopping Track"));
}

bool trackHasLinearTime()
{
  int lasttc = 0;
  for (int i=0;i<GetNumSongTracks();i++) 
  {
    unsigned long tc = SongTrack_timecode(i);
    //unsigned long evt = SongTrack_time(i);
    if (tc < lasttc)
       return false;
  }
  return true;
}

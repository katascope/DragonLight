#include <Arduino.h>
#include "Time.h"

static TimeCode timeController;

void setTimecodeLastMatched(unsigned long match)
{
//  Serial.print(F("Set match="));
//  Serial.println(match);
  timeController.lastMatchedTimecode = match;
}

unsigned long getTimecodeLastMatched()
{
  return timeController.lastMatchedTimecode;
}

signed long getTimecodeTimeOffset()
{
  return timeController.timeOffset;
}

signed long getTimecodeSongOffset()
{
  return timeController.songOffset;
}

signed long setTimecodeTimeOffset(signed long tc)
{
  timeController.timeOffset = tc;  
}

signed long setTimecodeSongOffset(signed long tc)
{
  timeController.songOffset = tc;  
}

unsigned long GetTime() { return (unsigned long)((signed long)millis() - (signed long)getTimecodeTimeOffset() + getTimecodeSongOffset()); }

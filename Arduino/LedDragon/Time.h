#ifndef TIMECODE_DEF
#define TIMECODE_DEF

//////// Time code section ////////
struct TimeCode
{
  signed long timeOffset = 0;
  signed long songOffset = 0;
  unsigned long lastMatchedTimecode = 0;
};
signed long getTimecodeTimeOffset();
signed long getTimecodeSongOffset();
signed long setTimecodeTimeOffset(signed long tc);
signed long setTimecodeSongOffset(signed long tc);
void setTimecodeLastMatched(unsigned long match);
unsigned long getTimecodeLastMatched();
unsigned long GetTime();
//////// Time code section ////////

#endif

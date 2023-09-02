/*Copyright 6/14/2022 github/katascope
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/
#ifndef SIDEFX_TRACK_DEF
#define SIDEFX_TRACK_DEF

#include <avr/pgmspace.h>

const word SideFxTrackCM[] PROGMEM =
{
0,7, 0,7, 0,7, 0,7, 0,7, 0,7, 0,7, 0,7,
125,125,125,125,125,125,125,125,
0,1,0, 0,4,0,
1,1,0, 1,4,0,
2,1,0, 2,4,0,
3,1,0, 3,4,0,
4,1,0, 4,4,0,
5,1,0, 5,4,0,
6,1,0, 6,4,0,
7,1,0, 7,4,0
};

const word SideFxTrackUSA[] PROGMEM =
{
0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 
9,9,9,9,9,9,9,9,
0,1,0, 0,3,0, 0,4,0, 0,5,0, 0,7,0,
1,1,0, 1,3,0, 1,4,0, 1,5,0, 1,7,0,
2,1,0, 2,3,0, 2,4,0, 2,5,0, 2,7,0,
3,1,0, 3,3,0, 3,4,0, 3,5,0, 3,7,0,
4,1,0, 4,3,0, 4,4,0, 4,5,0, 4,7,0,
5,1,0, 5,3,0, 5,4,0, 5,5,0, 5,7,0,
6,1,0, 6,3,0, 6,4,0, 6,5,0, 6,7,0,
7,1,0, 7,3,0, 7,4,0, 7,5,0, 7,7,0
};

const word SideFxTrackFirePit[] PROGMEM =
{
0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 
208,208,208,208,208,208,208,208,
0,1,0, 0,2,1, 0,3,0,
1,1,0, 1,2,1, 1,3,0,
2,1,0, 2,2,1, 2,3,0,
3,1,0, 3,2,1, 3,3,0,
4,1,0, 4,2,1, 4,3,0,
5,1,0, 5,2,1, 5,3,0,
6,1,0, 6,2,1, 6,3,0,
7,1,0, 7,2,1, 7,3,0
};

const word SideFxTrackPurpleParty[] PROGMEM =
{
0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 
21,21,21,21,21,21,21,21,
0,1,0, 0,3,0, 0,4,0, 0,5,0, 0,7,0,
1,1,0, 1,3,0, 1,4,0, 1,5,0, 1,7,0,
2,1,0, 2,3,0, 2,4,0, 2,5,0, 2,7,0,
3,1,0, 3,3,0, 3,4,0, 3,5,0, 3,7,0,
4,1,0, 4,3,0, 4,4,0, 4,5,0, 4,7,0,
5,1,0, 5,3,0, 5,4,0, 5,5,0, 5,7,0,
6,1,0, 6,3,0, 6,4,0, 6,5,0, 6,7,0,
7,1,0, 7,3,0, 7,4,0, 7,5,0, 7,7,0
};

const word SideFxTrackRainbow[] PROGMEM =
{
0,3, 0,3, 0,3, 0,3, 0,3, 0,3, 0,3, 0,3, 
205,205,205,205,205,205,205,205,
0,1,0, 0,5,0,
1,1,0, 1,5,0,
2,1,0, 2,5,0,
3,1,0, 3,5,0,
4,1,0, 4,5,0,
5,1,0, 5,5,0,
6,1,0, 6,5,0,
7,1,0, 7,5,0
};

const word SideFxTrackHeat[] PROGMEM =
{
0,2, 0,2, 0,2, 0,2, 0,2, 0,2, 0,2, 0,2, 
201,201,201,201,201,201,201,201,
0,1,0, 0,3,0, 0,4,1, 0,5,1,
1,1,0, 1,3,0, 1,4,1, 1,5,1,
2,1,0, 2,3,0, 2,4,1, 2,5,1,
3,1,0, 3,3,0, 3,4,1, 3,5,1,
4,1,0, 4,3,0, 4,4,1, 4,5,1,
5,1,0, 5,3,0, 5,4,1, 5,5,1,
6,1,0, 6,3,0, 6,4,1, 6,5,1,
7,1,0, 7,3,0, 7,4,1, 7,5,1
};


#endif

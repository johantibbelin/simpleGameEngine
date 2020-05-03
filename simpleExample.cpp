#include "simpleGameEngine.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>

#if defined(__ATARI_ST__)

#endif

int main()
{
  simpleGameEngine ge = simpleGameEngine(0,5,320,188);
  ge.setPixelSize(16);
  ge.drawPixel(5,5,1);
  ge.drawPixel(6,6,14);
int x,y;
  while (1) {
  for (x=0;x<20;x++)
    {
      for (y=1;y<12;y++) 
	{
	  ge.drawPixel(x,y,rand() % 16);
	}
    }
}
  return 0;
}

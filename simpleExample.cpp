#include "simpleGameEngine.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
//#include <chrono>

#if defined(__ATARI_ST__)

#endif

int main()
{
  simpleGameEngine ge = simpleGameEngine(0,5,320,188);
  ge.setPixelSize(16);
  ge.drawPixel(5,5,1);
  ge.drawPixel(6,6,14);
  short framecount=0;
  short nLastFPS=0;
  int x,y;
  // std::chrono::time_point<std::chrono::system_clock> m_tp1,m_tp2; 
  //m_tp1 = m_tp2 = std::chrono::system_clock::now();
  //float fFrameTimer=0;
  while (1) {
    /* // Handle Timing
    m_tp2 = std::chrono::system_clock::now();
    std::chrono::duration<int> elapsedTime = m_tp2 - m_tp1;
    m_tp1 = m_tp2;
    int fElapsedTime = elapsedTime.count();
    */
  for (x=0;x<20;x++)
    {
      for (y=1;y<12;y++) 
	{
	  ge.drawPixel(x,y,rand() % 16);
	}
    }
  /* fFrameTimer += fElapsedTime;
  framecount++;
  		if (fFrameTimer >= 1)
		{
			nLastFPS = framecount;
			fFrameTimer -= 1;
			const char* s = "simpleGameEngine - FPS: " + framecount;
			printf("%d",framecount);
			int shadr = (int)s >> 16;
			int sladr = ((int)s & 0xFFFF);
			wind_set(ge.hand,2,shadr,sladr,0,0);

			framecount = 0;
			}*/

  
}
  return 0;
}

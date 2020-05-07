#include "simpleGameEngine.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
//#include <chrono>

#if defined(__ATARI_ST__)

#endif

int main()
{
  simpleGameEngine ge = simpleGameEngine(0,5,320,187);
  ge.setPixelSize(16);
  ge.drawPixel(5,5,1);
  ge.drawPixel(6,6,14);
  //  short framecount=0;
  //  short nLastFPS=0;
  int x,y;
  // std::chrono::time_point<std::chrono::system_clock> m_tp1,m_tp2; 
  //m_tp1 = m_tp2 = std::chrono::system_clock::now();
  //float fFrameTimer=0;
  bool run=true;  
  int fcount=0;
while (run) {
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

  fcount++;
  if (fcount > 50)
    run = false;
}
 ge.clearBuffer();
 ge.drawLine(20,20,300,20,2);
 ge.drawLine(20,40,300,40,3);
 ge.drawLine(20,60,300,60,4);
 ge.drawLine(20,20,20,160,2);
 ge.drawLine(300,20,300,160,2);
 ge.drawRectangle(60, 60, 120, 120, 2);
 ge.fillRectangle(80, 80, 100, 100, 4);
 ge.drawLine(0,0,150,150,2);
 ge.drawLine(0,0,319,160,3);
 ge.drawLine(0,0,100,140,4);
 ge.drawLine(100,100,80,178,5);
 ge.setPixelSize(1);
 ge.drawCircle(100,100,25,2);
 ge.fillCircle(150, 100, 25, 4);
 ge.drawTriangle(150, 150, 100, 100, 75, 75, 2);
 ge.clearBuffer();
 ge.drawLine(100,100,230,125,3);
 ge.drawLine(80,80,147,137,4);
 ge.clearBuffer();
 ge.drawTriangle(100, 100, 150,100, 150,150,6);
 //ge.drawLine(100,100,150,100,5);
 //ge.drawLine(150,100,150,150,5);
 //ge.drawLine(100,100,150,150,5);
while (1) {}
  return 0;
}

// SimpleGameEngine impl.
#include "simpleGameEngine.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <osbind.h>
#include <gem.h>



simpleGameEngine::simpleGameEngine() 
{

}
simpleGameEngine::simpleGameEngine(short x,short y,short w,short h) 
{
  screen_mem = (short*)Physbase();
  hand = wind_create(3,x,y,w,h);
  const char* s = "simpleGameEngine";
  int shadr = (int)s >> 16;
  int sladr = ((int)s & 0xFFFF);
  wind_set(hand,2,shadr,sladr,0,0);
  wind_open(hand,x,y,w,h);
   clearBuffer();
}

simpleGameEngine::~simpleGameEngine()
{

}
int drawPixel(short x,short y,short color)
{

}                                                                                        
void simpleGameEngine::clearBuffer()
{
  short sb = x % 16;
  start_mask = 0xffffffff >> sb;
  short eb = 16 -((x+w) % 16);
  end_mask = 0xffffffff << eb;
  window_mem = screen_mem + 80 * ay;
  short * screen_ptr = window_mem;
  int ix,iy;
  for (iy = ay; iy < (ay+h);iy++)
    {
      int sw = 1;
      int ew = ((x+w) / 16) -4;
      int count = 0;
      for (ix = x / 16; ix < ((x+w) / 16); ix++)
	{
	  if (sw == 1) { 
	    *(screen_ptr) = *screen_ptr & start_mask;
	  screen_ptr++;
	  count++;
	  if (count==4)
	    sw=0;
	  }
	  else if (ix >= ew) {
	    *(screen_ptr) = *screen_ptr & end_mask;
	    screen_ptr++;
	  } 
	  else
	    *(screen_ptr++) = 0; 
	}   
    }
}
int simpleGameEngine::drawRectangle()
{

}
int simpleGameEngine::drawRectangleFilled()
{

}



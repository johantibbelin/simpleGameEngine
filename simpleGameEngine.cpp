// SimpleGameEngine impl.
#include "simpleGameEngine.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>
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
  
  window_mem = screen_mem +(y+11)*40+(x/16)*4; 
  clearBuffer();
}

simpleGameEngine::~simpleGameEngine()
{

}
int drawPixel()
{

}                                                                                        
void simpleGameEngine::clearBuffer()
{
  short* tmem = window_mem;
  for (int i = 0; i<(h);i++)
    {
      for (int j = 0; j<(w/16)+1; j++)
	{
	  *(tmem)++=0;
	}
    }
}
int simpleGameEngine::drawRectangle()
{

}
int simpleGameEngine::drawRectangleFilled()
{

}



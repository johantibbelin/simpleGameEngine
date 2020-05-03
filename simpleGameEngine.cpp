// SimpleGameEngine impl.
#include "simpleGameEngine.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <osbind.h>
#include <gem.h>

void simpleGameEngine::printBin(int num)
{
  while (num) {
     if (num & 1)
       printf("1");
     else
       printf("0");
     num >>= 1;
}

}

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
  ax=x+1;
  ay=y+11;
  aw=w-2;
  ah=h-12;
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
  short sb = ax % 16;
  uint16_t start_mask = (0xffff >> sb);
  short eb = 16 -((ax+aw) % 16);
  uint16_t end_mask = (0xffff << eb);
  start_mask = ~start_mask;
  end_mask = ~end_mask;
  window_mem = screen_mem + 80 * ay;
  short * screen_ptr = window_mem;
  int ix,iy;
  if (_DEBUG_) {
    printf("clearBuffer numbers:\n");
    printf("sb: %d eb: %d \n",sb,eb);
    printf("start_mask: %d",start_mask);
    printf(" end_mask: %d\n",end_mask);
 }
  // while (1) {}
  for (iy = ay; iy < (ay+ah);iy++)
    {

      for (ix = ax / 16; ix < ((ax+aw) / 16) *4+4 ; ix++)
	{
	  if(ix < 4)
	    *(screen_ptr++) &= start_mask;
	  else if (ix > ((ax +aw /16) *4 -4))
	    *screen_ptr = *screen_ptr++ & end_mask;
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



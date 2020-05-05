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
  clear(2); // clear with bg color
  graf_growbox(0,0,5,5,x,y,w,h);
  wind_open(hand,x,y,w,h);
  _window = true;
  graf_mouse(0,0);
  ax=x+1;
  ay=y+11;
  aw=w-2;
  ah=h-12;
   clearBuffer();
}

simpleGameEngine::~simpleGameEngine()
{

}
void simpleGameEngine::drawPixel(short x,short y,short color)
{
  uint16_t bplane0,bplane1,bplane2,bplane3;
    short c = color;
    if (pixel_size_x == 16) {
      short bp0 = c & 0x01;
      short bp1 = (c & 0x02) >> 1;
      short bp2 = (c & 0x04) >> 2;
      short bp3 = (c & 0x08 ) >> 3;
      if (bp0) 
	bplane0 = 0xffff;
      else
	bplane0 = 0x0000;
      if (bp1) 
	bplane1 = 0xffff;
      else
	bplane1 = 0x0000;
      if (bp2) 
	bplane2 = 0xffff;
      else
	bplane2 = 0x0000;
      if (bp3) 
	bplane3 = 0xffff;
      else
	bplane3 = 0x0000;
      short *screen_ptr = screen_mem;
      screen_ptr+=16*80*y+x*4;
      for (int i=0; i < pixel_size_y;i++) {
	if (x == 0) {
	  *(screen_ptr) &= start_mask;
	  bplane0 &= ~start_mask;
	  *(screen_ptr++) |= bplane0;
	  *(screen_ptr) &= start_mask;
	  bplane1 &= ~start_mask;
	  *(screen_ptr++) |= bplane1;
	  *(screen_ptr) &= start_mask;
	  bplane2 &= ~start_mask;
	  *(screen_ptr++) |= bplane2;
	  *(screen_ptr) &= start_mask;
	  bplane3 &= ~start_mask;
	  *(screen_ptr++) |= bplane3;
	} else if (x == 19) {
	  *(screen_ptr) &= end_mask;
	  bplane0 &= ~end_mask;
	  *(screen_ptr++) |= bplane0;
	  *(screen_ptr) &= end_mask;
	  bplane1 &= ~end_mask;
	  *(screen_ptr++) |= bplane1;
	  *(screen_ptr) &= end_mask;
	  bplane2 &= ~end_mask;
	  *(screen_ptr++) |= bplane2;
	  *(screen_ptr) &= end_mask;
	  bplane3 &= ~end_mask;
	  *(screen_ptr++) |= bplane3;
	} else {
	  *(screen_ptr++) = bplane0;
	  *(screen_ptr++) = bplane1;
	  *(screen_ptr++) = bplane2;
	  *(screen_ptr++) = bplane3;
	}
	screen_ptr+=76;
      }
    }       
}   

                                                                                        
void simpleGameEngine::clearBuffer()
{
  short sb = ax % 16;
  uint16_t start_mask = (0xffff >> sb);
  short eb = 16 -((ax+aw) % 16);
  uint16_t end_mask = (0xffff << eb);
  start_mask = ~start_mask;
  end_mask = ~end_mask;
  this->start_mask = start_mask;
  this->end_mask = end_mask;
  window_mem = screen_mem + 80 * ay;
  short * screen_ptr = window_mem;
  int ix,iy;
  
  for (int y=0;y < ah-11; y++) {
    for (x=0; x < 20;x++) {
      if (x == 0) {
	*(screen_ptr++) &= start_mask;
	*(screen_ptr++) &= start_mask;
	*(screen_ptr++) &= start_mask;
	*(screen_ptr++) &= start_mask;
      } else if (x == 19 ){
	*(screen_ptr++) &= end_mask;	
	*(screen_ptr++) &= end_mask;	
	*(screen_ptr++) &= end_mask;	
	*(screen_ptr++) &= end_mask;	
      } else {
	*(screen_ptr++) = 0;
	*(screen_ptr++) = 0;
	*(screen_ptr++) = 0;
	*(screen_ptr++) = 0;	
      }
    }
  } 
}
void simpleGameEngine::drawLine(short x1,short y1,short x2, short y2, short c)
{
  //Horisontal
  if (y1 == y2)
    {
      int sb = x1 % 16;
      int eb = 16 - (x2 % 16);
      short start_mask = 0xffff >> sb;
      short end_mask = 0xffff << eb;
      int bp0 = c & 0x01;
      int bp1 = (c & 0x02) >> 1;
      int bp2 = (c & 0x04) >> 2;
      int bp3 = (c & 0x08) >> 3;
      if (bp0) 
	bp0 = 0xffff;
      else
	bp0 = 0x0000;
      if (bp1) 
	bp1 = 0xffff;
      else
	bp1 = 0x0000;
      if (bp2) 
	bp2 = 0xffff;
      else
	bp2 = 0x0000;
      if (bp3) 
	bp3 = 0xffff;
      else
	bp3 = 0x0000;
      short bp0_start = start_mask & bp0;
      short bp1_start = start_mask & bp1;
      short bp2_start = start_mask & bp2;
      short bp3_start = start_mask & bp3;
      short bp0_end = end_mask & bp0;
      short bp1_end = end_mask & bp1;
      short bp2_end = end_mask & bp2;
      short bp3_end = end_mask & bp3;
      short *screen_ptr = screen_mem;
      screen_ptr += 80 * y1 + (x1 / 16)*4;
      for (int i=0; i < ( (x2 / 16) - (x1 / 16) )+1; i++)
	{
	  if (i==0) {
	    *(screen_ptr) &= ~start_mask;
	    *(screen_ptr++) |= bp0_start;
	    *(screen_ptr) &= ~start_mask;
	    *(screen_ptr++) |= bp1_start;
	    *(screen_ptr) &= ~start_mask;
	    *(screen_ptr++) |= bp2_start;
	    *(screen_ptr) &= ~start_mask;
	    *(screen_ptr++) |= bp3_start;
	  } else if (i== (x2/16-x1/16)) {
	    *(screen_ptr) &= ~end_mask;
	    *(screen_ptr++) |= bp0_end;
	    *(screen_ptr) &= ~end_mask;
	    *(screen_ptr++) |= bp1_end;
	    *(screen_ptr) &= ~end_mask;
	    *(screen_ptr++) |= bp2_end;
	    *(screen_ptr) &= ~end_mask;
	    *(screen_ptr++) |= bp3_end;
	  } else {
	    *(screen_ptr++) = bp0;
	    *(screen_ptr++) = bp1;
	    *(screen_ptr++) = bp2;
	    *(screen_ptr++) = bp3;

	  }
	}
    } else if (x1 == x2) { //Vertical line
    short bit = x1 % 16;
    short data = 0x8000 >> bit;
    
    short mask = ~data;

      int bp0 = c & 0x01;
      int bp1 = (c & 0x02) >> 1;
      int bp2 = (c & 0x04) >> 2;
      int bp3 = (c & 0x08) >> 3;
      if (bp0) 
	bp0 = data;
      else
	bp0 = 0x0000;
      if (bp1) 
	bp1 = data;
      else
	bp1 = 0x0000;
      if (bp2) 
	bp2 = data;
      else
	bp2 = 0x0000;
      if (bp3) 
	bp3 = data;
      else
	bp3 = 0x0000;
      short* screen_ptr = screen_mem;
      screen_ptr += y1*80 + (x1/16)*4;
      for (int i=0; i< y2+1; i++) {
	*(screen_ptr)   &= mask;
	*(screen_ptr++) |= bp0;
	*(screen_ptr)   &= mask;
	*(screen_ptr++) |= bp1;
	*(screen_ptr)   &= mask;
	*(screen_ptr++) |= bp2;
	*(screen_ptr)   &= mask;
	*(screen_ptr++) |= bp3;
	screen_ptr += 76;
      }
  }
}

void simpleGameEngine::drawRectangle()
{

}
int simpleGameEngine::drawRectangleFilled(short x1,short y1,short x2,short y2, short c)
{
  for (int i=0;i < y2-y1;i++) {
    drawLine(x1,y1+i,x2,y1+i,c);
  }
}
void simpleGameEngine::setPixelSize(short size)
{
  pixel_size_x = pixel_size_y = size;
}
void simpleGameEngine::clear(short c)
{
  short bplane0,bplane1,bplane2,bplane3;
  short bp0 = c & 0x01;
  short bp1 = (c & 0x02) >> 1;
  short bp2 = (c & 0x04) >> 2;
  short bp3 = (c & 0x08 ) >> 3;
  if (bp0) 
    bplane0 = 0xffff;
  else
    bplane0 = 0x0000;
  if (bp1) 
    bplane1 = 0xffff;
  else
    bplane1 = 0x0000;
  if (bp2) 
    bplane2 = 0xffff;
  else
    bplane2 = 0x0000;
  if (bp3) 
    bplane3 = 0xffff;
  else
    bplane3 = 0x0000;
  if (!_window) {
  short *screen_ptr = screen_mem;
  for (int i=0; i < 4000; i++) {
    *(screen_ptr++) = bplane0;
    *(screen_ptr++) = bplane1;
    *(screen_ptr++) = bplane2;
    *(screen_ptr++) = bplane3;
  }
  } else {
    // clear window
  }
}



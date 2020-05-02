#ifndef _SIMPLEGAMEENGINE_H_
#define _SIMPLEGAMEENGINE_H_

// includes
#if defined(__cplusplus)
#include <cstdio>
#include <cstdlib>
#include <cstdint>

#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#endif

#if defined(__ATARI_ST__)
#include <osbind.h>
#include <gem.h>
#endif
//c linkage functions
#if defined(__cplusplus)
extern "C" {
#endif
int  vbl();
int  hbl();
void  vbl_hbl_init();
void  vbl_hbl_restore();
#if defined(__cplusplus)
}
#endif
class simpleGameEngine 
{
   private:
short *screen_mem;
short *window_mem;
short *window_buffer;
short *screen_buffer;
int hand;
// window cordinates 
short x,y,w,h;

struct st_draw_buffer {
  uint16_t bplane0;
  uint16_t bplane1;
  uint16_t bplane2;
  uint16_t bplane3;
};
st_draw_buffer draw_buffer;
#if defined(__cplusplus)
   public:
 simpleGameEngine(short x,short y,short w,short h); 
simpleGameEngine();
~simpleGameEngine();
#endif
void drawPixel();
void clearBuffer();
int drawRectangle();
int drawRectangleFilled();
#if defined(__cplusplus)
};
#endif

#endif // end _SIMPLEGAMEENGINE_H_

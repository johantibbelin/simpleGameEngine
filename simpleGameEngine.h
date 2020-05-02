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

//c linkage functions
#if defined(__cplusplus)
extern "C" {
#endif
  vbl();
  hbl();
  vbl_hbl_init();
  vbl_hbl_restore();
#if defined(__cplusplus)
}

class simpleGameEngine 
{
   private:
#endif
short *screen_mem;
short *window_mem;
short *window_buffer;
short *screen_buffer;
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
simpleGameEngine();
~simpleGameEngine();
#endif
drawPixel();
clearBuffer();
drawRectangle();
drawRectangleFilled();
#if defined(__cplusplus)
};
#endif

#endif // end _SIMPLEGAMEENGINE_H_

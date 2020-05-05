#ifndef _SIMPLEGAMEENGINE_H_
#define _SIMPLEGAMEENGINE_H_

//macros
#ifdef _DEBUG_
#define _DEBUG_ 1
#else
#define _DEBUG_ 0
#endif

// includes
#if defined(__cplusplus)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
  // If double buffer is used 
  short *window_buffer;
  short *screen_buffer;
  //int hand;
  // Pixel size
  short pixel_size_x=1,pixel_size_y=1;
  // window cordinates 
  short x,y,w,h;
  // Window work area cordinates
  short ax,ay,aw,ah;
 // Line start and end mask
  uint16_t start_mask,end_mask; 
  

  struct st_draw_buffer {
  uint16_t bplane0;
  uint16_t bplane1;
  uint16_t bplane2;
  uint16_t bplane3;
};
  bool _window = false;
  void printBin(int num);
 
  st_draw_buffer draw_buffer;
#if defined(__cplusplus)
 public:
  short vblcount;
  int hand;
 simpleGameEngine(short x,short y,short w,short h); 
 simpleGameEngine();
 ~simpleGameEngine();
#endif
 void drawPixel(short x,short y,short color);
 void clearBuffer();
void drawRectangle(short x1,short y1,short x2,short y2, short c);
 int drawRectangleFilled(short x1,short y1,short x2,short y2,short c);
 void setPixelSize(short size);
 void clear(short c);
 void drawLine(short x1,short y1,short x2, short y2, short c);

#if defined(__cplusplus)
};
#endif

#endif // end _SIMPLEGAMEENGINE_H_

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
#include <math.h>

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
  int iScreenSizeX;
  int iScreenSizeY;
  int iPixelSizeX;
  int iPixelSizeY;
  int  iWindowSizeX;
  int  iWindowSizeY;

  bool bFullScreen;
  bool bEnableVsync;
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
  bool bAtomActive; 
  // stuff
  /*olc::vi2d  vScreenSize = { 0, 0 };
  olc::vf2d  vInvScreenSize = { 0,0 };
  olc::vi2d  vPixelSize = { 1, 1 };
  olc::vi2d  vWindowSize;
  bool	bFullScreen = full_screen;
  bool	bEnableVSYNC = vsync;
  olc::vf2d	vPixel = 2.0f / vScreenSize;
*/

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
 int Construct(int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, bool full_screen, bool vsync);
#endif
 void Start();
 void EngineThread();
 void CoreUpdate();
 void drawPixel(short x,short y,short color);
 void clearBuffer();
void drawRectangle(short x1,short y1,short x2,short y2, short c);
void fillRectangle(short x1,short y1,short x2,short y2,short c);
 void drawTriangle(short x1, short y1, short x2, short y2, short x3, short y3, short c);
 void fillTriangle(short x1,short y1,short x2,short y2,short x3,short y3,short c);
 void setPixelSize(short size);
 void clear(short c);
 void drawLine(short x1,short y1,short x2, short y2, short c);
 void drawCircle(short x,short y,short radius,short c);
 void fillCircle(short x, short y, short radius, short c);
 int OnUserUpdate(int time);
#if defined(__cplusplus)
};
#endif

	// O------------------------------------------------------------------------------O
	// | olc::vX2d - A generic 2D vector type                                         |
	// O------------------------------------------------------------------------------O
#if !defined(OLC_IGNORE_VEC2D)
namespace olc {
	template <class T>
	struct v2d_generic
	{
		T x = 0;
		T y = 0;
		inline v2d_generic() : x(0), y(0)                        {                                                            }
		inline v2d_generic(T _x, T _y) : x(_x), y(_y)            {                                                            }
		inline v2d_generic(const v2d_generic& v) : x(v.x), y(v.y){                                                            }
		inline T mag()                                           { return std::sqrt(x * x + y * y);                           }
		inline T mag2()					                         { return x * x + y * y;                                      }
		inline v2d_generic  norm()                               { T r = 1 / mag(); return v2d_generic(x*r, y*r);             }
		inline v2d_generic  perp()                               { return v2d_generic(-y, x);                                 }
		inline T dot(const v2d_generic& rhs)                     { return this->x * rhs.x + this->y * rhs.y;                  }
		inline T cross(const v2d_generic& rhs)                   { return this->x * rhs.y - this->y * rhs.x;                  }
		inline v2d_generic  operator +  (const v2d_generic& rhs) const { return v2d_generic(this->x + rhs.x, this->y + rhs.y);}
		inline v2d_generic  operator -  (const v2d_generic& rhs) const { return v2d_generic(this->x - rhs.x, this->y - rhs.y);}
		inline v2d_generic  operator *  (const T& rhs)           const { return v2d_generic(this->x * rhs, this->y * rhs);    }
		inline v2d_generic  operator *  (const v2d_generic& rhs) const { return v2d_generic(this->x * rhs.x, this->y * rhs.y);}
		inline v2d_generic  operator /  (const T& rhs)           const { return v2d_generic(this->x / rhs, this->y / rhs);    }
		inline v2d_generic  operator /  (const v2d_generic& rhs) const { return v2d_generic(this->x / rhs.x, this->y / rhs.y);}
		inline v2d_generic& operator += (const v2d_generic& rhs) { this->x += rhs.x; this->y += rhs.y; return *this;          }
		inline v2d_generic& operator -= (const v2d_generic& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this;          }
		inline v2d_generic& operator *= (const T& rhs)           { this->x *= rhs; this->y *= rhs; return *this;              }
		inline v2d_generic& operator /= (const T& rhs)           { this->x /= rhs; this->y /= rhs; return *this;              }
		inline operator v2d_generic<int32_t>() const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
		inline operator v2d_generic<float>() const { return { static_cast<float>(this->x), static_cast<float>(this->y) };     }
		inline operator v2d_generic<double>() const { return { static_cast<double>(this->x), static_cast<double>(this->y) };  }
	};

	// Note: joshinils has some good suggestions here, but they are complicated to implement at this moment, 
	// however they will appear in a future version of PGE
	template<class T> inline v2d_generic<T> operator * (const float& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs * (float)rhs.x), (T)(lhs * (float)rhs.y)); }
	template<class T> inline v2d_generic<T> operator * (const double& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs * (double)rhs.x), (T)(lhs * (double)rhs.y)); }
	template<class T> inline v2d_generic<T> operator * (const int& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs * (int)rhs.x), (T)(lhs * (int)rhs.y)); }
	template<class T> inline v2d_generic<T> operator / (const float& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs / (float)rhs.x), (T)(lhs / (float)rhs.y)); }
	template<class T> inline v2d_generic<T> operator / (const double& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs / (double)rhs.x), (T)(lhs / (double)rhs.y)); }
	template<class T> inline v2d_generic<T> operator / (const int& lhs, const v2d_generic<T>& rhs)
	{ return v2d_generic<T>((T)(lhs / (int)rhs.x), (T)(lhs / (int)rhs.y)); }

	typedef v2d_generic<int32_t> vi2d;
	typedef v2d_generic<uint32_t> vu2d;
	typedef v2d_generic<float> vf2d;
	typedef v2d_generic<double> vd2d;
}
#endif
#endif // end _SIMPLEGAMEENGINE_H_

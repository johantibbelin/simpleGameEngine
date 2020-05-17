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
#include <string>
#include <iostream>
#include <vector>
#include <map>

#define UNUSED(x) (void)(x)

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
namespace olc
{
	class PixelGameEngine;
		
	// Pixel Game Engine Advanced Configuration
	constexpr uint8_t  nMouseButtons = 5;
	constexpr uint8_t  nDefaultAlpha = 0xFF;
	constexpr uint32_t nDefaultPixel = (nDefaultAlpha << 24);
	enum rcode { FAIL = 0, OK = 1, NO_FILE = -1 };

	struct ResourceBuffer : public std::streambuf
	{
		ResourceBuffer(std::ifstream &ifs, uint32_t offset, uint32_t size);
		std::vector<char> vMemory;
	};	

	// O------------------------------------------------------------------------------O
	// | olc::Pixel - Represents a 32-Bit RGBA colour                                 |
	// O------------------------------------------------------------------------------O
	struct Pixel
	{
		union
		{
			uint32_t n = nDefaultPixel;
			struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; } ;
		};

		enum Mode { NORMAL, MASK, ALPHA, CUSTOM };

		Pixel();
		Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = nDefaultAlpha);
		Pixel(uint32_t p);
		bool operator==(const Pixel& p) const;
		bool operator!=(const Pixel& p) const;
	};

	Pixel PixelF(float red, float green, float blue, float alpha = 1.0f);



	// O------------------------------------------------------------------------------O
	// | USEFUL CONSTANTS                                                             |
	// O------------------------------------------------------------------------------O
	static const Pixel
	GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
	RED(255, 0, 0),      DARK_RED(128, 0, 0),      VERY_DARK_RED(64, 0, 0),
	YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
	GREEN(0, 255, 0),    DARK_GREEN(0, 128, 0),    VERY_DARK_GREEN(0, 64, 0),
	CYAN(0, 255, 255),   DARK_CYAN(0, 128, 128),   VERY_DARK_CYAN(0, 64, 64),
	BLUE(0, 0, 255),     DARK_BLUE(0, 0, 128),     VERY_DARK_BLUE(0, 0, 64),
	MAGENTA(255, 0, 255),DARK_MAGENTA(128, 0, 128),VERY_DARK_MAGENTA(64, 0, 64),
	WHITE(255, 255, 255),BLACK(0, 0, 0),           BLANK(0, 0, 0, 0);

	enum Key
	{
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
		NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL, PERIOD
	};

	class Sprite
	{
	public:
		Sprite();
		Sprite(const std::string& sImageFile);
		Sprite(int32_t w, int32_t h);
		~Sprite();

	public:
		olc::rcode LoadFromFile(const std::string& sImageFile, char *pack = nullptr);
		olc::rcode LoadFromFile(const std::string& sImageFile);

		olc::rcode LoadFromPGESprFile(const std::string& sImageFile, char *pack = nullptr);

		olc::rcode SaveToPGESprFile(const std::string& sImageFile);

	public:
		int32_t width = 0;
		int32_t height = 0;
		enum Mode { NORMAL, PERIODIC };
		enum Flip { NONE = 0, HORIZ = 1, VERT = 2 };

	public:
		void SetSampleMode(olc::Sprite::Mode mode = olc::Sprite::Mode::NORMAL);
		Pixel GetPixel(int32_t x, int32_t y);
		bool  SetPixel(int32_t x, int32_t y, Pixel p);
		//		Pixel GetPixel(const olc::vi2d& a);
		//	bool  SetPixel(const olc::vi2d& a, Pixel p);
		Pixel Sample(float x, float y);
		Pixel SampleBL(float u, float v);
		Pixel* GetData();
		Pixel *pColData = nullptr;
		Mode modeSample = Mode::NORMAL;
		uint16_t *bpData;
		uint16_t palette[16];
		uint16_t endMask=0;
	};

#if !defined(OLC_IGNORE_VEC2D)
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
#endif



	// O------------------------------------------------------------------------------O
	// | olc::HWButton - Represents the state of a hardware button (mouse/key/joy)    |
	// O------------------------------------------------------------------------------O
	struct HWButton
	{
		bool bPressed = false;	// Set once during the frame the event occurs
		bool bReleased = false;	// Set once during the frame the event occurs
		bool bHeld = false;		// Set true for all frames between pressed and released events
	};

	/*	class ResourcePack : public std::streambuf
	{
	public:
		ResourcePack();
		~ResourcePack();
		bool AddFile(const std::string& sFile);
		bool LoadPack(const std::string& sFile, const std::string& sKey);
		bool SavePack(const std::string& sFile, const std::string& sKey);
		ResourceBuffer GetFileBuffer(const std::string& sFile);
		bool Loaded();
	private:
		struct sResourceFile { uint32_t nSize; uint32_t nOffset; };
		std::map<std::string, sResourceFile> mapFiles;

		std::ifstream baseFile;

		std::vector<char> scramble(const std::vector<char>& data, const std::string& key);
		std::string makeposix(const std::string& path);
		};*/

}

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
 void DrawSprite(int32_t x, int32_t y, olc::Sprite *sprite, uint32_t scale, uint8_t flip);
 int OnUserUpdate(int time);
#if defined(__cplusplus)
};
#endif
#endif // end _SIMPLEGAMEENGINE_H_

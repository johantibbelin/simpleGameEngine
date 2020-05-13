// blurg
// SimpleGameEngine impl.
#include "simpleGameEngine.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <osbind.h>
#include <gem.h>
#include <utility>
#include <string>
#include <png.h>
#include <iostream>
#include <vector>
#include <map>

// Sprite impl
namespace olc {
	Sprite::Sprite()
	{ pColData = nullptr; width = 0; height = 0; }


  Sprite::Sprite(const std::string& sImageFile)
	{ LoadFromFile(sImageFile); }



	Sprite::Sprite(int32_t w, int32_t h)
	{
		if(pColData) delete[] pColData;
		width = w;		height = h;
		pColData = new Pixel[width * height];
		for (int32_t i = 0; i < width*height; i++)
			pColData[i] = Pixel();
	}

	Sprite::~Sprite()
	{ if (pColData) delete[] pColData; }


	olc::rcode Sprite::LoadFromPGESprFile(const std::string& sImageFile, char *pack)
	{
	  	if (pColData) delete[] pColData;

		return olc::FAIL;
	}

	void Sprite::SetSampleMode(olc::Sprite::Mode mode)
	{ modeSample = mode; }

  /*	Pixel Sprite::GetPixel(const olc::vi2d& a)
	{ return GetPixel(a.x, a.y); }

	bool Sprite::SetPixel(const olc::vi2d& a, Pixel p)
	{ return SetPixel(a.x, a.y, p); }
  */
	Pixel Sprite::GetPixel(int32_t x, int32_t y)
	{
		if (modeSample == olc::Sprite::Mode::NORMAL)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
				return pColData[y*width + x];
			else
				return Pixel(0, 0, 0, 0);
		}
		else
		{
			return pColData[abs(y%height)*width + abs(x%width)];
			}
	}

	bool Sprite::SetPixel(int32_t x, int32_t y, Pixel p)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			pColData[y*width + x] = p;
			return true;
		}
		else
			return false;
	}

	Pixel Sprite::Sample(float x, float y)
	{
		int32_t sx = std::min((int32_t)((x * (float)width)), width - 1);
		int32_t sy = std::min((int32_t)((y * (float)height)), height - 1);
		return GetPixel(sx, sy);
	}

	Pixel Sprite::SampleBL(float u, float v)
	{
		u = u * width - 0.5f;
		v = v * height - 0.5f;
		int x = (int)floor(u); // cast to int rounds toward zero, not downward
		int y = (int)floor(v); // Thanks @joshinils
		float u_ratio = u - x;
		float v_ratio = v - y;
		float u_opposite = 1 - u_ratio;
		float v_opposite = 1 - v_ratio;

		olc::Pixel p1 = GetPixel(std::max(x, 0), std::max(y, 0));
		olc::Pixel p2 = GetPixel(std::min(x + 1, (int)width - 1), std::max(y, 0));
		olc::Pixel p3 = GetPixel(std::max(x, 0), std::min(y + 1, (int)height - 1));
		olc::Pixel p4 = GetPixel(std::min(x + 1, (int)width - 1), std::min(y + 1, (int)height - 1));

		return olc::Pixel(
			(uint8_t)((p1.r * u_opposite + p2.r * u_ratio) * v_opposite + (p3.r * u_opposite + p4.r * u_ratio) * v_ratio),
			(uint8_t)((p1.g * u_opposite + p2.g * u_ratio) * v_opposite + (p3.g * u_opposite + p4.g * u_ratio) * v_ratio),
			(uint8_t)((p1.b * u_opposite + p2.b * u_ratio) * v_opposite + (p3.b * u_opposite + p4.b * u_ratio) * v_ratio));
	}

	Pixel* Sprite::GetData()
	{ return pColData; }
	// O------------------------------------------------------------------------------O
	// | olc::Pixel IMPLEMENTATION                                                    |
	// O------------------------------------------------------------------------------O
	Pixel::Pixel()
	{ r = 0; g = 0; b = 0; a = nDefaultAlpha; }

	Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{ r = red; g = green; b = blue; a = alpha; }

	Pixel::Pixel(uint32_t p)
	{ n = p; }

	bool Pixel::operator==(const Pixel& p) const
	{ return n == p.n; }

	bool Pixel::operator!=(const Pixel& p) const
	{ return n != p.n; }

	Pixel PixelF(float red, float green, float blue, float alpha)
	{
		return Pixel(uint8_t(red * 255.0f), uint8_t(green * 255.0f), uint8_t(blue * 255.0f), uint8_t(alpha * 255.0f));
	}
  // Sprite Load from file
	olc::rcode Sprite::LoadFromFile(const std::string& sImageFile, char* pack)
	{
		UNUSED(pack);
		////////////////////////////////////////////////////////////////////////////
		// Use libpng, Thanks to Guillaume Cottenceau
		// https://gist.github.com/niw/5963798

		// Also reading png from streams
		// http://www.piko3d.net/tutorials/libpng-tutorial-loading-png-files-from-streams/

		png_structp png;
		png_infop info;

		auto loadPNG = [&]()
		{
			png_read_info(png, info);
			png_byte color_type;
			png_byte bit_depth;
			png_bytep* row_pointers;
			width = png_get_image_width(png, info);
			height = png_get_image_height(png, info);
			color_type = png_get_color_type(png, info);
			bit_depth = png_get_bit_depth(png, info);
			if (bit_depth == 16) png_set_strip_16(png);
			if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
			if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)	png_set_expand_gray_1_2_4_to_8(png);
			if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
			if (color_type == PNG_COLOR_TYPE_RGB ||
				color_type == PNG_COLOR_TYPE_GRAY ||
				color_type == PNG_COLOR_TYPE_PALETTE)
				png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
			if (color_type == PNG_COLOR_TYPE_GRAY ||
				color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
				png_set_gray_to_rgb(png);
			png_read_update_info(png, info);
			row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
			for (int y = 0; y < height; y++) {
				row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
			}
			png_read_image(png, row_pointers);
			////////////////////////////////////////////////////////////////////////////
			// Create sprite array
			pColData = new Pixel[width * height];
			// Iterate through image rows, converting into sprite format
			for (int y = 0; y < height; y++)
			{
				png_bytep row = row_pointers[y];
				for (int x = 0; x < width; x++)
				{
					png_bytep px = &(row[x * 4]);
					SetPixel(x, y, Pixel(px[0], px[1], px[2], px[3]));
				}
			}

			for (int y = 0; y < height; y++) // Thanks maksym33
				free(row_pointers[y]);
			free(row_pointers);
			png_destroy_read_struct(&png, &info, nullptr);			
		};

		png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) goto fail_load;

		info = png_create_info_struct(png);
		if (!info) goto fail_load;

		if (setjmp(png_jmpbuf(png))) goto fail_load;

		if (pack == nullptr)
		{
			FILE* f = fopen(sImageFile.c_str(), "rb");
			if (!f) return olc::NO_FILE;
			png_init_io(png, f);
			loadPNG();
			fclose(f);
		}
		else
		{
		  /*ResourceBuffer rb = pack->GetFileBuffer(sImageFile);
			std::istream is(&rb);
			png_set_read_fn(png, (png_voidp)&is, pngReadStream);
			loadPNG();*/
		}

		return olc::OK;

	fail_load:
		width = 0;
		height = 0;
		pColData = nullptr;
		return olc::FAIL;
	}

 
} // namespace olc

/*
 * SimpleGameEngine Implementation
 */
// helper function
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

int simpleGameEngine::Construct(int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, bool full_screen, bool vsync)
{
    x = y = 0;
    w = iScreenSizeX = screen_w, 
    h = iScreenSizeY =screen_h;
    //vInvScreenSize = { 1.0f / float(screen_w), 1.0f / float(screen_h) };
    pixel_size_x = iPixelSizeX = pixel_w;
    pixel_size_y = iPixelSizeY = pixel_h;
    iWindowSizeX = iScreenSizeX;
    iWindowSizeY = iScreenSizeY;
    bFullScreen = full_screen;
    bEnableVsync = vsync;
    //vPixel = 2.0f / vScreenSize;
    
    //if (vPixelSize.x <= 0 || vPixelSize.y <= 0 || vScreenSize.x <= 0 || vScreenSize.y <= 0)
    //	return olc::FAIL;
    
    // Construct default font sheet
    //olc_ConstructFontSheet();
    return 0;
}

void simpleGameEngine::Start()
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
   // Start the thread
   bAtomActive = true;
   EngineThread();
}
   void simpleGameEngine::EngineThread()
   {
       while (bAtomActive)
	   {
	       while (bAtomActive) { CoreUpdate();  }
	   }
   }

void simpleGameEngine::CoreUpdate()
{
    int time=0;
    //time
    if (!OnUserUpdate(time))
	bAtomActive = false;
//stuff
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
      bplane0 = bp0 ? 0xffff : 0x0;
      bplane1 = bp1 ? 0xffff : 0x0;
      bplane2 = bp2 ? 0xffff : 0x0;
      bplane3 = bp3 ? 0xffff : 0x0;

      short *screen_ptr = (short*) Physbase();
      screen_ptr+=16*80*y+x*4;
      if (_DEBUG_) {
	  printf("%d",&screen_ptr);
	  Crawcin();
      }
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
    } else if (pixel_size_x == 1) {
      short bp0 = c & 0x01;
      short bp1 = (c & 0x02) >> 1;
      short bp2 = (c & 0x04) >> 2;
      short bp3 = (c & 0x08 ) >> 3;
      short bit,d,m;
      bit = x % 16;
      d = 0x8000 >> bit;
      m = ~d;
      bp0 = bp0 ? d : 0x0;
      bp1 = bp1 ? d : 0x0;
      bp2 = bp2 ? d : 0x0;
      bp3 = bp3 ? d : 0x0;

      short *screen_ptr = screen_mem;
      screen_ptr+=80*y+(x/16)*4;
      *(screen_ptr) &= m;
      *(screen_ptr++) |=bp0;
      *(screen_ptr) &= m;
      *(screen_ptr++) |=bp1;
      *(screen_ptr) &= m;
      *(screen_ptr++) |=bp2;
      *(screen_ptr) &= m;
      *(screen_ptr) |=bp3;
    }       
}   
void simpleGameEngine::drawCircle(short x, short y, short radius, short c)
{
  short x0 = 0;
  short y0 = radius;
  short d = 3 - 2 * radius;
  if (!radius) return;
  
  while (y0 >= x0) // only formulate 1/8 of circle
    {
        drawPixel(x + x0, y - y0, c);
	drawPixel(x + y0, y - x0, c);
	drawPixel(x + y0, y + x0, c);
	drawPixel(x + x0, y + y0, c);
	drawPixel(x - x0, y + y0, c);
	drawPixel(x - y0, y + x0, c);
	drawPixel(x - y0, y - x0, c);
	drawPixel(x - x0, y - y0, c);
	if (d < 0) d += 4 * x0++ + 6;
	else d += 4 * (x0++ - y0--) + 10;
    }
}

void simpleGameEngine::fillCircle(short x, short y, short radius, short c)
{
		// Taken from wikipedia
		short x0 = 0;
		short y0 = radius;
		short d = 3 - 2 * radius;
		if (!radius) return;

		auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i <= ex; i++)
				drawPixel(i, ny, c);
		};

		while (y0 >= x0)
		{
			// Modified to draw scan-lines instead of edges
			drawline(x - x0, x + x0, y - y0);
			drawline(x - y0, x + y0, y - x0);
			drawline(x - x0, x + x0, y + y0);
			drawline(x - y0, x + y0, y + x0);
			if (d < 0) d += 4 * x0++ + 6;
			else d += 4 * (x0++ - y0--) + 10;
		}
}

/*	void simpleGameEngine::DrawSprite(int32_t x, int32_t y, Sprite *sprite, uint32_t scale, uint8_t flip)
	{
		if (sprite == nullptr)
			return;

		int32_t fxs = 0, fxm = 1, fx = 0;
		int32_t fys = 0, fym = 1, fy = 0;
		if (flip & olc::Sprite::Flip::HORIZ) { fxs = sprite->width - 1; fxm = -1; }
		if (flip & olc::Sprite::Flip::VERT) { fys = sprite->height - 1; fym = -1; }

		if (scale > 1)
		{
			fx = fxs;
			for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
			{
				fy = fys;
				for (int32_t j = 0; j < sprite->height; j++, fy += fym)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
							Draw(x + (i*scale) + is, y + (j*scale) + js, sprite->GetPixel(fx, fy));
			}
		}
		else
		{
			fx = fxs;
			for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
			{
				fy = fys;
				for (int32_t j = 0; j < sprite->height; j++, fy += fym)
					drawPixel(x + i, y + j, sprite->GetPixel(fx, fy));
			}
		}
		}*/
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
  //  int ix,iy;
  
  for (int y=0;y <= ah; y++) {
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

      bp0 = bp0 ? 0xffff : 0x0;
      bp1 = bp1 ? 0xffff : 0x0;
      bp2 = bp2 ? 0xffff : 0x0;
      bp3 = bp3 ? 0xffff : 0x0;

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
      bp0 = bp0 ? data : 0x0;
      bp1 = bp1 ? data : 0x0;
      bp2 = bp2 ? data : 0x0;
      bp3 = bp3 ? data : 0x0;
   
      short* screen_ptr = screen_mem;
      screen_ptr += y1*80 + (x1/16)*4;
      for (int i=0; i<= y2-y1; i++) {
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
  } else {
      if (x2 < x1) {
	  short tmpx,tmpy;
	  tmpx = x1;
	  tmpy = y1;
	  x1 = x2;
	  y1 = y2;
	  x2 = tmpx;
	  y2 = tmpy;
      }
    int xc; //,yc;
    short d,m;
   int m_new = 2 * (y2 - y1); 
   int slope_error_new = m_new - (x2 - x1); 
   short* screen_ptr;
   for (int x = x1, y = y1; x <= x2; x++) 
   { 
     xc = x % 16;
     d= 0x8000 >> xc;
     m = ~d;
      int bp0 = c & 0x01;
      int bp1 = (c & 0x02) >> 1;
      int bp2 = (c & 0x04) >> 2;
      int bp3 = (c & 0x08) >> 3;

      bp0 = bp0 ? d : 0x0;
      bp1 = bp1 ? d : 0x0;
      bp2 = bp2 ? d : 0x0;
      bp3 = bp3 ? d : 0x0;
 

     screen_ptr = screen_mem + y* 80 + (x/16)*4; 
     *(screen_ptr) &= m;
     *(screen_ptr++) |= bp0;
     *(screen_ptr) &= m;
     *(screen_ptr++) |= bp1;
     *(screen_ptr) &= m;
     *(screen_ptr++) |= bp2;
     *(screen_ptr) &= m;
     *(screen_ptr) |= bp3;
      // Add slope to increment angle formed 
      slope_error_new += m_new; 
  
      // Slope error reached limit, time to 
      // increment y and update slope error. 
      if (slope_error_new >= 0) 
      { 
         y++; 
         slope_error_new  -= 2 * (x2 - x1); 
      } 
   } 
  }
}

void simpleGameEngine::drawRectangle(short x1,short y1,short x2,short y2, short c)
{
  drawLine(x1,y1,x2,y1,c);
  drawLine(x1,y1,x1,y2,c);
  drawLine(x2,y1,x2,y2,c);
  drawLine(x1,y2,x2,y2,c);
}
void simpleGameEngine::fillRectangle(short x1,short y1,short x2,short y2, short c)
{
  short line=y1;
  short tline=y2;
  while (line < tline && line < 199 ) {
     drawLine(x1,line,x2,line,c);
    line++;
  }
}

void simpleGameEngine::drawTriangle(short x1, short y1, short x2, short y2, short x3, short y3, short c)
{
    drawLine(x1,y1,x2,y2,c);
    drawLine(x2,y2,x3,y3,c);
    drawLine(x3,y3,x1,y1,c);
}

void simpleGameEngine::fillTriangle(short x1,short y1,short x2,short y2,short x3,short y3,short c)
{
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) drawPixel(i, ny, c); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1>y2) {std::swap(y1, y2); std::swap(x1, x2); }
		if (y1>y3) {std::swap(y1, y3); std::swap(x1, x3); }
		if (y2>y3) {std::swap(y2, y3); std::swap(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1);
		if (dx1<0) { dx1 = -dx1; signx1 = -1; }	else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1);
		if (dx2<0) { dx2 = -dx2; signx2 = -1; } else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
		if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx>t1x) minx = t1x;
			if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x;
			if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										// Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i<dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:

			if (minx>t1x) minx = t1x;
			if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x;
			if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y>y3) return;
		}
}

void simpleGameEngine::setPixelSize(short size)
{
  pixel_size_x = pixel_size_y = size;
}
void simpleGameEngine::clear(short c)
{

  short bp0 = c & 0x01;
  short bp1 = (c & 0x02) >> 1;
  short bp2 = (c & 0x04) >> 2;
  short bp3 = (c & 0x08 ) >> 3;
  bp0 = bp0 ? 0xffff : 0x0;
  bp1 = bp1 ? 0xffff : 0x0;
  bp2 = bp2 ? 0xffff : 0x0;
  bp3 = bp3 ? 0xffff : 0x0;
  if (!_window) {
  short *screen_ptr = screen_mem;
  for (int i=0; i < 4000; i++) {
    *(screen_ptr++) = bp0;
    *(screen_ptr++) = bp1;
    *(screen_ptr++) = bp2;
    *(screen_ptr++) = bp3;
  }
  } else {
    // clear window
  }
}



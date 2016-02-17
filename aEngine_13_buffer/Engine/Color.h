
#pragma once

#include"prequest.h"

namespace NBengine {

#define RGB24BIT(r, g, b)  ((((r)&0xff)<<16) | (((g)&0xff)<<8) | ((b)&0xff))

	// 255 base
	class Color
	{
		uchar r, g, b, a;
	public:
		Color() { r = g = b = 0, a = 255; }
		Color(uchar r, uchar g, uchar b, uchar a = 255) :r(r), g(g), b(b), a(a) {}
		Color(u32 color, uchar alpha = 255) {
			a = alpha;
			b = (uchar)((color & 0xff0000) >> 16);
			g = (uchar)((color & 0x00ff00) >> 8);
			r = (uchar)(color & 0x0000ff);
		}


		inline u32 toInt() const { return RGB24BIT(r, g, b); }
	};
}
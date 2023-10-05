//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <wtypes.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Global Defines
//------------------------------------------------------------------------------
#define _RGB(r, g, b) ((r) << 16) | ((g) << 8) | ((b) << 0)

#define RGB15to24(rgb)				\
	((((rgb) >> 0) & 31) << 3)		\
	| ((((rgb) >> 5) & 31) << 11)	\
	| (((rgb >> 10) & 31) << 19)

#define _RGB15(r, g, b) WORD(((r) << 10) | ((g) << 5) | ((b) << 0))


//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------
union ARGB
{
	explicit ARGB(DWORD _color = 0xFF000000): color(0) {}
	explicit ARGB(WORD _color): color(0xFF000000 | RGB15to24(_color)) {}
	explicit ARGB(const RGBTRIPLE& triple) : color(_RGB(triple.rgbtRed, triple.rgbtGreen, triple.rgbtBlue) | (255 << 24)) {}
	ARGB(BYTE r, BYTE g, BYTE b, BYTE a = 255): color(_RGB(r, g, b) | (a << 24)) {}

	inline WORD toRGB15() const
	{
		return
			(((red >> 3) & 31) << 10)
			| (((green >> 3) & 31) << 5)
			| (((blue >> 3) & 31) << 0);
	}

	operator RGBTRIPLE()
	{ 
		RGBTRIPLE triple;

		triple.rgbtRed = this->red;
		triple.rgbtGreen = this->green;
		triple.rgbtBlue = this->blue;

		return triple;
	}
	
	inline operator DWORD () const { return color; }
	inline operator WORD () const { return toRGB15(); }
	
	inline operator RGBQUAD() const
	{
		RGBQUAD quad;

		quad.rgbBlue = blue;
		quad.rgbGreen = green;
		quad.rgbRed = red;
		quad.rgbReserved = alpha;

		return quad;
	}

	bool operator ==(const ARGB& other) const
	{ 
		return color == other.color;
	}

	struct
	{
		BYTE blue;
		BYTE green;
		BYTE red;
		BYTE alpha;
	};

	DWORD color;
};

//------------------------------------------------------------------------------
// ARGBFX.h
//------------------------------------------------------------------------------
#ifndef ARGBFX_H
#define ARGBFX_H

#include "ARGB.h"
#include "../Math/Fixed.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// Structure Definition
//------------------------------------------------------------------------------
struct ARGBFX
{
	ARGBFX(): alpha(0), blue(0), green(0), red(0) {}
	ARGBFX(const ARGB& rgb): red(rgb.red), green(rgb.green), blue(rgb.blue), alpha(rgb.alpha) {}
	ARGBFX(Fx32 r, Fx32 g, Fx32 b, Fx32 a = Fx32(255)): alpha(a), blue(b), green(g), red(r) {}
	ARGBFX(const ARGBFX& other): alpha(other.alpha), blue(other.blue), green(other.green), red(other.red) {}

	operator RGBQUAD () const
	{
		RGBQUAD quad;

		quad.rgbRed = Fx32toInt(red);
		quad.rgbGreen = Fx32toInt(green);
		quad.rgbBlue = Fx32toInt(blue);
		quad.rgbReserved = Fx32toInt(alpha);

		return quad;
	}

	operator ARGB () const
	{
		return ARGB(
			Fx32toInt(red),
			Fx32toInt(green),
			Fx32toInt(blue),
			Fx32toInt(alpha));
	}

	operator WORD () const
	{
		return _RGB15(Fx32toInt(red) / 8, Fx32toInt(green) / 8, Fx32toInt(blue) / 8);
	}

	ARGBFX operator - (const ARGBFX& rhs) const
	{
		return ARGBFX(red - rhs.red, green - rhs.green, blue - rhs.blue, alpha - rhs.alpha);
	}

	ARGBFX operator + (const ARGBFX& rhs) const
	{
		return ARGBFX(red + rhs.red, green + rhs.green, blue + rhs.blue, alpha + rhs.alpha);
	}

	ARGBFX operator * (const ARGB& rhs) const
	{
		return ARGBFX(red * rhs.red, green * rhs.green, blue * rhs.blue, alpha * rhs.alpha);
	}

	ARGBFX operator * (const ARGBFX& rhs) const
	{
		return ARGBFX(red * rhs.red, green * rhs.green, blue * rhs.blue, alpha * rhs.alpha);
	}

	ARGBFX operator * (const Fx32 rhs) const
	{
		return ARGBFX(red * rhs, green * rhs, blue * rhs, alpha * rhs);
	}

	ARGBFX operator * (const int rhs) const
	{
		return ARGBFX(red * rhs, green * rhs, blue * rhs, alpha * rhs);
	}

	ARGBFX operator / (const Fx32 rhs) const
	{
		return ARGBFX(red / rhs, green / rhs, blue / rhs, alpha / rhs);
	}

	ARGBFX operator / (const int rhs) const
	{
		return ARGBFX(red / rhs, green / rhs, blue / rhs, alpha / rhs);
	}

	ARGBFX& operator += (const ARGBFX& rhs)
	{
		alpha	+= rhs.alpha;
		blue	+= rhs.blue;
		green	+= rhs.green;
		red		+= rhs.red;

		return *this;
	}

	ARGBFX& operator -= (const ARGBFX& rhs)
	{
		alpha -= rhs.alpha;
		blue -= rhs.blue;
		green -= rhs.green;
		red -= rhs.red;

		return *this;
	}

	ARGBFX& operator *= (const Fx32 rhs)
	{
		alpha *= rhs;
		blue *= rhs;
		green *= rhs;
		red *= rhs;

		return *this;
	}

	Fx32 alpha;
	Fx32 blue;
	Fx32 green;
	Fx32 red;
};


static const ARGBFX kArgbFxWhite = ARGBFX(255, 255, 255, 255);
static const ARGBFX kArgbFxRed = ARGBFX(255, 0, 0, 255);
static const ARGBFX kArgbFxGreen = ARGBFX(0, 255, 0, 255);
static const ARGBFX kArgbFxBlue = ARGBFX(0, 0, 255, 255);

////////////////////////////////////////////////////////////////////////////////
#endif
//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "..\Math\Vector.h"

#include <wtypes.h>
////////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------------------
//	Rectangle
//--------------------------------------------------------------------------
struct Rect: public RECT
{
	Rect();	
	Rect(long l, long t, long r, long b);
	Rect(const Vector2i& tl, const Vector2i& br);
	Rect(const Vector2<long>& tl, const Vector2<long>& br);
	Rect(const Vector2i& tl, int width, int height);
	Rect(const RECT& rc);

	bool operator == (const Rect& rhs) const;
	Rect operator +(const Vector2<LONG>& p) const;
	Rect operator +(const Rect& r) const;

	template<class T>
	Rect operator +(const Vector2<T>& p) const
	{
		return *this + Vector2<LONG>((LONG)p.x, (LONG)p.y);
	}

	bool contains(int x, int y) const;

	template<class T>
	bool contains(const Vector2<T>& p) const
	{ 
		return contains((int)p.x, (int)p.y);
	}

	long width() const;
	long height() const;

	Vector2<LONG> topLeft() const;
	Vector2<LONG> bottomRight() const;
};

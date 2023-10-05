//------------------------------------------------------------------------------
//  Copyright : (c) 2006
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------
#ifndef RECT_H
#define RECT_H

#include <wtypes.h>
////////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------------------
//	Rectangle
//--------------------------------------------------------------------------
struct Rect: public RECT
{
	Rect() { left = right = bottom = top = 0; }
	
	Rect(long l, long t, long r, long b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	
	Rect(const RECT& rc)
	{
		left = rc.left;
		top = rc.top;
		right = rc.right;
		bottom = rc.bottom;
	}

	bool operator == (const Rect& rhs) const
	{
		return
			left == rhs.left
			&& right == rhs.right
			&& top == rhs.top
			&& bottom == rhs.bottom;
	}

	bool contains(int x, int y) const
	{
		return (left <= x && right > x) && (top <= y && bottom > y);
	}

	long width() const	{ return right - left; }
	long height() const	{ return bottom - top; }
};


////////////////////////////////////////////////////////////////////////////////
#endif
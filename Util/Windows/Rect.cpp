//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Rect.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Rect
//------------------------------------------------------------------------------
Rect::Rect()
{
	left = right = bottom = top = 0;
}

Rect::Rect(long l, long t, long r, long b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

Rect::Rect(const Vector2i& tl, const Vector2i& br)
{
	left = tl.x;
	top = tl.y;
	right = br.x;
	bottom = br.y;
}

Rect::Rect(const Vector2<long>& tl, const Vector2<long>& br)
{
	left = tl.x;
	top = tl.y;
	right = br.x;
	bottom = br.y;
}

Rect::Rect(const Vector2i& tl, int width, int height)
{
	left = tl.x;
	top = tl.y;
	right = tl.x + width;
	bottom = tl.y + height;
}

Rect::Rect(const RECT& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
}

bool Rect::operator == (const Rect& rhs) const
{
	return
		left == rhs.left
		&& right == rhs.right
		&& top == rhs.top
		&& bottom == rhs.bottom;
}

Rect Rect::operator +(const Vector2<LONG>& p) const
{
	return Rect(left + p.x, top + p.y, right + p.x, bottom + p.y);
}

Rect Rect::operator +(const Rect& r) const
{
	return Rect(left + r.left, top + r.top, right + r.right, bottom + r.bottom);
}

bool Rect::contains(int x, int y) const
{
	return (left <= x && right > x) && (top <= y && bottom > y);
}

long Rect::width() const
{
	return right - left;
}

long Rect::height() const
{
	return bottom - top;
}

Vector2<LONG> Rect::topLeft() const
{
	return Vector2<LONG>(left, top);
}

Vector2<LONG> Rect::bottomRight() const
{
	return Vector2<LONG>(right, bottom);
}

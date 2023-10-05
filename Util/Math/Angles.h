//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Angles
//------------------------------------------------------------------------------
template<class T>
inline T degToRad(T val)
{
	return val * T(M_PI / 180);
}


template<class T>
inline T radToDeg(T val)
{
	return val * T(180 / M_PI);
}


template<class T>
inline T radianWrap(T val)
{
	while (val >= T(2 * M_PI))
		val -= T(2 * M_PI);

	while (val < T(0))
		val += T(2 * M_PI);

	return val;
}


template<class T>
inline T degWrap(T val)
{
	while (val >= T(360))
		val -= T(360);

	while (val < 0)
		val += T(360);

	return val;
}

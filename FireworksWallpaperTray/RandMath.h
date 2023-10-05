//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	RandMath
//------------------------------------------------------------------------------
template<class T>
inline T mod(T a, T b)
{
	if (b == T(0))
	{
		return T(0);
	}

	return a % b;
}


template<class T>
inline int randRange(T low, T high)
{
	if (low == high)
	{
		return low;
	}

	return low + mod((T)rand(), high - low + 1);
}


inline bool randPercent(int percent)
{
	if (percent <= 0)
	{
		return false;
	}

	if (percent >= 100)
	{
		return true;
	}

	return ((rand() * 100) / RAND_MAX) <= percent;
}

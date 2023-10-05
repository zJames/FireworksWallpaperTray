//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Wrap
//------------------------------------------------------------------------------
template<class T>
T incWrap(T val, const T maxVal)
{
	val += T(1);

	if (val < maxVal)
		return val;
	else
		return T(0);
}


template<class T>
T decWrap(T val, const T maxVal)
{
	if (val > T(0))
		return val - T(1);

	return maxVal - T(1);
}

template<class T>
T wrapTo(T input, T low, T high)
{
	while (input < low)
	{
		input += (high - low);
	}

	while (input > high)
	{
		input -= high - low;
	}

	return input;
}

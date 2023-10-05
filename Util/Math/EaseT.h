//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	EaseT
//------------------------------------------------------------------------------
template<class T, class S>
class EaseT
{
public:
	static T cubicEaseIn(const T from, const T to, S time, const S d = S(1))
	{
		time /= d;

		return to * time * time * time + from;
	}

	static T cubicEaseOut(const T from, const T to, S time, const S d = S(1))
	{
		time /= d;
		time -= 1;

		return to * (time * time * time + 1) + from;
	}

	static T cubicEaseInOut(const T from, const T to, S time, const S d = S(1))
	{
		time /= d / 2;

		if (time < S(1))
		{
			return to / 2 * time * time * time + from;
		}

		time -= 2;

		return to / 2 * (time * time * time + 2) + from;
	}

	static T qudEaseIn(const T from, const T to, S time, const S d = S(1))
	{
		time /= d;

		return to * time * time + from;
	}

	static T quadEaseOut(const T from, const T to, S time, const S d = S(1))
	{
		time /= d;

		return -to * time * (time - 2) + from;
	}

	static T quadEaseInOut(const T from, const T to, S time, const S d = S(1))
	{
		time /= d / 2;
		
		if (time < 1)
		{
			return from / 2 * time * time + from;
		}
		
		time -= 1;
		
		return -to / 2 * (time * (time - 2) - 1) + from;
	}

	static T quarticEaseIn(const T from, const T to, S time, const S d = S(1))
	{
		time /= d;

		return to * time * time * time * time + from;
	}

	static T quarticEaseOut(const T from, const T to, S time, const S d = S(1))
	{
		time /= d;
		time -= 1;

		return -to * (time * time * time * time - 1) + from;
	}

	static T quarticEaseInOut(const T from, const T to, S time, const S d = S(1))
	{
		time /= d / 2;
		
		if (time < 1)
		{
			return to / 2 * time * time * time * time + from;
		}
		
		time -= 2;
		
		return -to / 2 * (time * time * time * time - 2) + from;
	}

	static T quinticEaseIn(const T from, const T to, S time, const S d = S(1))
	{
		time /= d;
		
		return to * time * time * time * time * time + from;
	}

	static T quinticEaseOut(const T from, const T to, S time, const S d = S(1))
	{
		time /= d;
		time -= 1;
		
		return to * (time * time * time * time * time + 1) + from;
	}

	static T quinticEaseInOut(const T from, const T to, S time, const S d = S(1))
	{
		time /= d / 2;
		
		if (time < 1)
		{
			return to / 2 * time * time * time * time * time + from;
		}
		
		time -= 2;
		
		return to / 2 * (time * time * time * time * time + 2) + from;
	}

	static T lerp(const T from, const T to, S time)
	{
		return from + (to - from) * time;
	}
};

//TODO:
//	This goes from 0 to 1
//
//					x^a
// fa(x) =	-------------------
//			  x^a + (1 - x)^a

typedef EaseT<float, float> EaseF;
typedef EaseT<double, double> EaseD;
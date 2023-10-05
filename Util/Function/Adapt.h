//------------------------------------------------------------------------------
//  Copyright : (c) 2010
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Function.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Adapt
//------------------------------------------------------------------------------
template<class R, class T>
R adapt(Function<typename T()>& t)
{
	return R(t());
}

template<class R, class T, class Af>
R adapt(Function<typename T(Af)>& t, Af a)
{
	return R(t(a));
}

template<class R, class T>
Function<typename R()> adaptFunc(Function<typename T()>& t)
{
	return func(&adapt<R, T>, t);
}

template<class R, class T, class Af>
Function<typename R(Af)> adaptFunc(Function<typename T()>& t, Af a)
{
	return func(&adapt<R, T>, t, a);
}

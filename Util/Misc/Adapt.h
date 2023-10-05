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

template<class R, class T>
Function<typename R()> adaptFunc(Function<typename T()>& t)
{
	return func(&adapt<typename R, typename T>, t);
}
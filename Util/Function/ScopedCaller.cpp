//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "ScopedCaller.h"

#include "../Misc/SwapT.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	ScopedCaller
//------------------------------------------------------------------------------
ScopedCaller::ScopedCaller(const FastDelegate<void()>& func)
	:
	mFunc(func)
{
}


ScopedCaller::ScopedCaller(ScopedCaller& other)
{
	swapT(mFunc, other.mFunc);
}


ScopedCaller::~ScopedCaller()
{
	if (!mFunc.empty())
		mFunc();
}


ScopedCaller& ScopedCaller::operator=(ScopedCaller& other)
{
	swapT(mFunc, other.mFunc);

	return *this;
}
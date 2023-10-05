//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "FastDelegate.h"
////////////////////////////////////////////////////////////////////////////////

using namespace fastdelegate; //I know this is bad, but this seems unnecessary

//------------------------------------------------------------------------------
//	ScopedCaller:
//		Calls function delegate when going out of scope.
//------------------------------------------------------------------------------
class ScopedCaller
{
public:
	ScopedCaller(const FastDelegate<void()>& func);
	ScopedCaller(ScopedCaller&);
	~ScopedCaller();

	ScopedCaller& operator=(ScopedCaller& other);

private:
	FastDelegate<void()> mFunc;
};
//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <assert.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Util
//------------------------------------------------------------------------------
#define ARRAY_LENGTH(a)			(sizeof(a) / sizeof(a[0]))
#define SAFE_DELETE(n)			{ delete (n); (n) = NULL; }
#define SAFE_DELETE_ARRAY(n)	{ delete [] (n); (n) = NULL; }
#define SAFE_RELEASE(n)			{ if(n) { (n)->Release(); (n)=NULL; } }

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif // max

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif // min

template<class T, class S>
T* assert_cast(S* ptr)
{
	assert(static_cast<T*>(ptr) == dynamic_cast<T*>(ptr));

	return static_cast<T*>(ptr);
}
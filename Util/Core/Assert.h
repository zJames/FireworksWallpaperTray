//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Util.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Assert
//------------------------------------------------------------------------------
#define ctAssert(e) \
	do { \
		switch(0) \
		{ \
		case 0: \
		case e: \
			; \
		} \
	} while(0)

// Utility for conveniently putting array length compile-time asserts next to
// arrays that are outside functions.
#define AssertArrayLength(array, length) \
	static inline void AssertArrayLength_##array() \
	{ \
		ctAssert(ARRAY_LENGTH(array) == length); \
	}

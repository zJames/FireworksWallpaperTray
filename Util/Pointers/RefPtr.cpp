//------------------------------------------------------------------------------
//  Copyright : (c) 2006
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "RefPtr.h"
#include <map>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Variables
//------------------------------------------------------------------------------
static std::map<void*, int> s_ptrMap;


void RefData::addRef(void* ptr)
{
	if (ptr)
		s_ptrMap[ptr] += 1;
}


bool RefData::decRef(void* ptr)
{
	if (ptr)
		s_ptrMap[ptr] -= 1;

	return s_ptrMap[ptr] == 0;
}
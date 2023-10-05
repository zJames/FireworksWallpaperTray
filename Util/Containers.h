//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Containers
//------------------------------------------------------------------------------
#include "Containers/BitSet.h"
#include "Containers/BlockAllocator.h"
#include "Containers/BlockArray.h"
#include "Containers/Contains.h"
#include "Containers/Maybe.h"
#include "Containers/ObjBuff.h"


template<class C, class T>
bool removeFirst(C& container, T& object)
{
	for (typename C::Iterator i = container.begin(); i != container.end(); ++i)
	{
		if (*i == object)
		{
			container.erase(i);
			return true;
		}
	}

	return false;
}
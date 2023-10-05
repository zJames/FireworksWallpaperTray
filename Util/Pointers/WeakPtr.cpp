//------------------------------------------------------------------------------
//  Copyright : (c) 2007
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "WeakPtr.h"
////////////////////////////////////////////////////////////////////////////////

void WeakPtrBase::weakLink(const Weakable* w)
{
	if(w)
	{
		weakObject = w;
		nextPtr = w->weakPtrList;
		w->weakPtrList = this;
	}
	else
	{
		weakObject = 0;
		nextPtr = 0;
	}
}


void WeakPtrBase::weakUnlink()
{
	if(weakObject)
	{
		WeakPtrBase **pprev = &weakObject->weakPtrList;

		while (*pprev != this)
			pprev = &(*pprev)->nextPtr;

		*pprev = nextPtr;
	}
}


void Weakable::clearWeakPtrList()
{
	while(weakPtrList)
	{
		weakPtrList->weakSet(0);
	}
}
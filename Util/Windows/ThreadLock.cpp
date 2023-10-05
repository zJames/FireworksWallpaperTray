//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "ThreadLock.h"

#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Lock
//------------------------------------------------------------------------------
Lock::Lock(Ptr<volatile LONG64> threadLock)
	:
	mThreadLock(threadLock)
{
	if (mThreadLock.get() != NULL)
	{
		InterlockedIncrement64(mThreadLock.get());
	}
}


Lock::~Lock()
{
	if (mThreadLock.get() != NULL)
	{
		InterlockedDecrement64(mThreadLock.get());
	}
}


Lock& Lock::operator =(const Lock& other)
{
	if (mThreadLock.get() != NULL)
	{
		InterlockedDecrement64(mThreadLock.get());
	}

	mThreadLock = other.mThreadLock;

	if (mThreadLock.get() != NULL)
	{
		InterlockedIncrement64(mThreadLock.get());
	}

	return *this;
}


//------------------------------------------------------------------------------
//	ThreadLock
//------------------------------------------------------------------------------
ThreadLock::ThreadLock()
	:
	mLocked(0)
{
}


Lock ThreadLock::getLock()
{
	while (isLocked())
	{
		Sleep(2);
	}

	return Lock(&mLocked);
}


bool ThreadLock::isLocked() const
{
	return mLocked != 0;
}

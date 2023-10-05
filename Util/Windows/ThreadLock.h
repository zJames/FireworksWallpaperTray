//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Pointers/Ptr.h"

#include <wtypes.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	ThreadLock
//------------------------------------------------------------------------------
class Lock
{
public:
	~Lock();

	Lock& operator = (const Lock& other);

private:
	friend class ThreadLock;

	Lock(Ptr<volatile LONG64>);

	Ptr<volatile LONG64> mThreadLock;
};


class ThreadLock
{
public:
	ThreadLock();

	Lock getLock();

	bool isLocked() const;

private:
	volatile LONG64 mLocked;
};
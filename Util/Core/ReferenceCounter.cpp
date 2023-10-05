//------------------------------------------------------------------------------
//  Copyright : (c) 2018
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "ReferenceCounter.h"

#include "../Containers/BlockAllocator.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Data
//------------------------------------------------------------------------------
static BlockAllocator<int> sReferenceAllocator;


//------------------------------------------------------------------------------
//	ReferenceCounter
//------------------------------------------------------------------------------
ReferenceCounter::ReferenceCounter()
{
}

ReferenceCounter::ReferenceCounter(const ReferenceCounter& other)
{
	*this = other;
}

ReferenceCounter::~ReferenceCounter()
{
	release();
}

void ReferenceCounter::create()
{
	release();

	mCount = sReferenceAllocator.construct(1);
}

Maybe<int> ReferenceCounter::release()
{
	if (empty())
	{
		return nothing;
	}

	const int oldVal = --(*mCount);

	if (oldVal == 0)
	{
		sReferenceAllocator.free(mCount.get());
	}

	mCount = NULL;

	return oldVal;
}

ReferenceCounter& ReferenceCounter::operator=(const ReferenceCounter& other)
{
	release();

	if (!other.empty())
	{
		mCount = other.mCount;

		++(*mCount);
	}

	return *this;
}

Maybe<int> ReferenceCounter::current() const
{
	if (mCount.get())
	{
		return *mCount;
	}

	return nothing;
}

bool ReferenceCounter::empty() const
{
	return (mCount.get() == NULL);
}

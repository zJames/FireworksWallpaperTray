//------------------------------------------------------------------------------
//  Copyright : (c) 2018
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "..\Containers\Maybe.h"
#include "..\Pointers\Ptr.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	ReferenceCounter
//------------------------------------------------------------------------------
class ReferenceCounter
{
public:
	ReferenceCounter();
	ReferenceCounter(const ReferenceCounter& other);
	~ReferenceCounter();

	void create();
	Maybe<int> release();

	ReferenceCounter& operator =(const ReferenceCounter& other);

	Maybe<int> current() const;
	bool empty() const;

private:
	Ptr<int> mCount;
};

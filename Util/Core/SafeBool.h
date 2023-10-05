//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Core/Assert.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	SafeBool
//------------------------------------------------------------------------------
class SafeBoolBase
{
public:
	void safeBoolDummyMethod() const;

protected:
	typedef void (SafeBoolBase::*BaseMethodPtr)() const;

	~SafeBoolBase()
	{
	}
};


template<typename T>
class SafeBool : public SafeBoolBase
{
public:
	inline operator BaseMethodPtr() const
	{
		const bool truthValue =
			(static_cast<const T*>(this))->safeBoolValue();

		return
			truthValue
			? &SafeBoolBase::safeBoolDummyMethod
			: 0;
	}

protected:
	~SafeBool()
	{
	}
};


template<class LvT, class RvT>
bool operator==(const SafeBool<LvT>&, const SafeBool<RvT>&)
{
	ctAssert(false);
	return false;
}


// Same as operator==.
template<class LvT, class RvT>
bool operator!=(const SafeBool<LvT>&, const SafeBool<RvT>&)
{
	ctAssert(false);
	return false;
}

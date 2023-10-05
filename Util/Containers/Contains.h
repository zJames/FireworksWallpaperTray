//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Contains
//------------------------------------------------------------------------------
template<class T, class S>
bool contains(const S& collection, const T& value)
{
	for (typename S::const_iterator iter = collection.cbegin(); iter != collection.cend(); ++iter)
	{
		if (*iter == value)
		{
			return true;
		}
	}

	return false;
}


//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Config_Chain.h"

#include "Config_Base.h"
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	//------------------------------------------------------------------------------
	//	Chain
	//------------------------------------------------------------------------------
	Chain::Chain(BaseList& baseList)
	{
		mBases.swap(baseList);
	}

	Chain::Chain(BasePtr base)
	{
		mBases.push_back(base);
	}

	Chain::~Chain() {}

	const char* Chain::getRawOption(const char* name) const
	{
		for (BaseList::const_iterator iter = mBases.cbegin(); iter != mBases.cend(); ++iter)
		{
			const char* value = (*iter)->getRawOption(name);

			if (value != NULL)
			{
				return value;
			}
		}

		return NULL;
	}
} //end namespace Config
//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Config_Defaults.h"
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	//------------------------------------------------------------------------------
	//	Defaults
	//------------------------------------------------------------------------------
	Defaults::Defaults(const Default* defaults, size_t numDefaults)
	{
		convert(defaults, numDefaults);
	}

	Defaults::Defaults(const StringMap& stringMap)
		:
		mStrings(stringMap)
	{
	}

	const char* Defaults::getRawOption(const char* name) const
	{
		RefString refName(name);
		StringMap::const_iterator obj = mStrings.find(refName);

		if (obj == mStrings.end())
		{
			return NULL;
		}

		return obj->second;
	}

	void Defaults::convert(const Default* defaults, size_t numDefaults)
	{
		for (size_t i = 0; i < numDefaults; ++i)
		{
			mStrings[defaults[i].option] = defaults[i].value;
		}
	}
}
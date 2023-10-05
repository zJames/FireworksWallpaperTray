//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Config_Option.h"
#include "../Text/RefString.h"

#include <string>
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	//------------------------------------------------------------------------------
	//	IConfig
	//------------------------------------------------------------------------------
	class IConfig
	{
	public:
		IConfig() { seperator = ","; }
		virtual ~IConfig() {}

		virtual const char* getRawOption(const char*) const = 0;

		template<class T>
		T get(const Option<T>& option)
		{
			return option.convert(getRawOption(option.name()), seperator);
		}

		RefString seperator;
	};
} //end namespace Config

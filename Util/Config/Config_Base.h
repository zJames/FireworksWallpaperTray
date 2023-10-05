//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Config_IOption.h"
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	//------------------------------------------------------------------------------
	//	ConfigBase
	//------------------------------------------------------------------------------
	class Base
	{
	public:
		virtual ~Base();

		virtual const char* getRawOption(const char* name) const = 0;
		virtual const char* getRawOption(const IOption& option) const;
	};
} //end namespace Config

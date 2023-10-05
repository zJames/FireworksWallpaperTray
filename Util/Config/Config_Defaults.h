//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Config_Base.h"
#include "../Text/RefString.h"

#include <map>
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	struct Default
	{
		RefString option;
		RefString value;
	};

	//------------------------------------------------------------------------------
	//	Defaults
	//------------------------------------------------------------------------------
	class Defaults : public Base
	{
	public:
		typedef std::map<RefString, RefString> StringMap;

		Defaults(const Default* defaults, size_t numDefaults);
		Defaults(const StringMap& stringMap);

		const char* getRawOption(const char* name) const;

	private:
		void convert(const Default* defaults, size_t numDefualts);

		StringMap mStrings;
	};
}

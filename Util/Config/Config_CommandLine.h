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
	//------------------------------------------------------------------------------
	//	CommandLine
	//------------------------------------------------------------------------------
	class CommandLine : public Base
	{
	public:
		typedef std::map<RefString, RefString> StringMap;

		CommandLine(const char* argv[], int argc, const char* _prefix = "/", const char* _seperator = ":");
		CommandLine(const char* cmdLine, const char* _prefix = "/", const char* _seperator = ":");

		const char* getRawOption(const char* name) const;

		const RefString prefix;
		const RefString seperator;

	private:
		StringMap mStrings;
	};
} //end namespace Config

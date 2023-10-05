//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Text/RefString.h"
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	//------------------------------------------------------------------------------
	//	IOption
	//------------------------------------------------------------------------------
	class IOption
	{
	public:
		IOption(const RefString& name);

		const char* name() const;

	private:
		const RefString mName;
	};
} //end namespace Config

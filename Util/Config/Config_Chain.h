//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Config_IConfig.h"
#include "Config_Option.h"
#include "../Pointers/SharedPtr.h"

#include <list>
#include <map>
#include <string>
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	class Base;

	//------------------------------------------------------------------------------
	//	Chain
	//------------------------------------------------------------------------------
	class Chain : public IConfig
	{
	public:
		typedef SharedPtr<Base> BasePtr;
		typedef std::list<BasePtr> BaseList;

		Chain(BaseList& baseList);
		Chain(BasePtr base);
		virtual ~Chain();

		const char* getRawOption(const char*) const;

	private:
		BaseList mBases;
	};
}

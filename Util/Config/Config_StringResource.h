//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Config_Base.h"
#include "../Text/RefString.h"

#include <map>
#include <wtypes.h>
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	struct ResourceMap
	{
		RefString name;
		UINT id;
	};

	//------------------------------------------------------------------------------
	//	StringResource
	//------------------------------------------------------------------------------
	class StringResource : public Base
	{
	public:
		typedef std::map<RefString, UINT> StringMap;

		static const size_t kNumBuffers = 8;
		static const size_t kBufferSize = 2048;

		StringResource(HINSTANCE hInst, const ResourceMap* resources, size_t numResources);
		StringResource(HINSTANCE hInst, const StringMap& stringMap);

		const char* getRawOption(const char* name) const;

	private:
		void incIndex() const;

		StringMap mStrings;
		HINSTANCE mInstance;
		mutable size_t mBufferIdx;
		mutable char mBuffers[kNumBuffers][kBufferSize];
	};
}
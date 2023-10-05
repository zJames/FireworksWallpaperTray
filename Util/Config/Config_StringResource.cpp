//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Config_StringResource.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Config_StringResource
//------------------------------------------------------------------------------
namespace Config
{
	StringResource::StringResource(HINSTANCE hInst, const ResourceMap* resources, size_t numResources)
		:
		mInstance(hInst),
		mBufferIdx(0)
	{
		for (size_t i = 0; i < numResources; ++i)
		{
			mStrings[resources[i].name] = resources[i].id;
		}
	}

	StringResource::StringResource(HINSTANCE hInst, const StringMap& stringMap)
		:
		mStrings(stringMap),
		mInstance(hInst),
		mBufferIdx(0)
	{
	}

	const char* StringResource::getRawOption(const char* name) const
	{
		RefString refName(name);
		StringMap::const_iterator iter = mStrings.find(refName);

		if (iter == mStrings.end())
		{
			return NULL;
		}

		incIndex();

		LoadStringA(mInstance, iter->second, mBuffers[mBufferIdx], kBufferSize);

		return mBuffers[mBufferIdx];
	}

	void StringResource::incIndex() const
	{
		if (++mBufferIdx >= kNumBuffers)
		{
			mBufferIdx = 0;
		}
	}
}
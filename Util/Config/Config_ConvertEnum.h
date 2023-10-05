//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Config_IConverter.h"
#include "../Misc/Convert.h"
#include "../Pointers/Ptr.h"

#include <vector>
////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	//------------------------------------------------------------------------------
	//	ConvertEnum
	//------------------------------------------------------------------------------
	template<class T>
	class ConvertEnum : public IConverter<T>
	{
	public:
		ConvertEnum(Ptr<const StringMap> stringMap, size_t numStringMaps)
			:
			mStringMap(stringMap),
			mNumStringMaps(numStringMaps)
		{
		}

		T convert(const char* input, const char* seperator) const
		{
			return T(convertToInt(input, mStringMap.get(), mNumStringMaps));
		}

	private:
		const Ptr<const StringMap> mStringMap;
		const size_t mNumStringMaps;
	};
} //end namespace Config
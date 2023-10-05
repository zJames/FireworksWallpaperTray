//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Config_IConverter.h"
#include "../Misc/Convert.h"
#include "../Text/RefString.h"

#include <string>
#include <vector>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Config_ConvertT
//------------------------------------------------------------------------------
namespace Config
{
	template<class T>
	class ConvertT : public IConverter<T>
	{
	public:
		T convert(const char* input, const char* seperator) const
		{
			if (input == NULL)
			{
				return T();
			}

			return T(input);
		}
	};

	template<>
	inline std::list<RefString> ConvertT<std::list<RefString> >::convert(const char* input, const char* seperator) const
	{
		return convertToRefStringList(input, seperator);
	}

	template<>
	inline std::vector<RefString> ConvertT<std::vector<RefString> >::convert(const char* input, const char* seperator) const
	{
		return convertToRefStringArray(input, seperator);
	}

	template<>
	inline std::list<std::string> ConvertT<std::list<std::string> >::convert(const char* input, const char* seperator) const
	{
		return convertToStringList(input, seperator);
	}

	template<>
	inline std::vector<std::string> ConvertT<std::vector<std::string> >::convert(const char* input, const char* seperator) const
	{
		return convertToStringArray(input, seperator);
	}

	template<>
	inline int ConvertT<int>::convert(const char* input, const char* seperator) const
	{
		return convertToInt(input);
	}

	template<>
	inline std::list<int> ConvertT<std::list<int> >::convert(const char* input, const char* seperator) const
	{
		return convertToIntList(input, seperator);
	}

	template<>
	inline std::vector<int> ConvertT<std::vector<int> >::convert(const char* input, const char* seperator) const
	{
		return convertToIntArray(input, seperator);
	}

	template<>
	inline bool ConvertT<bool>::convert(const char* input, const char* seperator) const
	{
		return convertToBool(input);
	}
} //end namespace Config

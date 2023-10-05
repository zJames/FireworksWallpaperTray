//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Config_ConvertT.h"
#include "Config_IConverter.h"
#include "Config_IOption.h"
#include "../Pointers/SharedPtr.h"

////////////////////////////////////////////////////////////////////////////////


namespace Config
{
	//------------------------------------------------------------------------------
	//	Config::Option
	//------------------------------------------------------------------------------
	template<class T>
	class Option : public IOption
	{
	public:
		typedef T MyType;
		typedef IConverter<T> ConverterType;

		Option(const char* name, SharedPtr<ConverterType> converter)
			:
			IOption(name),
			mConverter(converter)
		{
		}

		Option(const char* name)
			:
			IOption(name),
			mConverter(new ConvertT<MyType>())
		{
		}

		virtual ~Option() {}

		inline T convert(const char* input, const char* seperator) const
		{
			return mConverter->convert(input, seperator);
		}

	private:
		SharedPtr<ConverterType> mConverter;
	};
} //end namespace Config

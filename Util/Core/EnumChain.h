//------------------------------------------------------------------------------
//  Copyright : (c) 2010
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "EnumChainDetail.h"
#include "../Containers/Maybe.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	EnumChain
//------------------------------------------------------------------------------
template<class ChainRepArg>
class EnumChain
{
public:
	typedef ChainRepArg ChainRep;
	typedef EnumChain<ChainRep> This;

	// Produces a new EnumChain type that's equivalent to this type with
	// another segment appended.
	template<class RawEnum, size_t kRawEnumSize>
	struct Append
	{
		typedef
			typename EnumDetail::AppendEnumToChain<ChainRep, RawEnum, kRawEnumSize>::Result
			ResultRep;

		typedef EnumChain<ResultRep> Result;
	};

	EnumChain(): mVal(0) {}

	template<class EnumO>
	EnumChain(const EnumO val)
		:
		mVal(EnumDetail::Helper<ChainRep, EnumO>::rawToChain(val))
	{
	}

	int val() const { return mVal; }

	template<class Enum>
	Maybe<Enum> get() const
	{
		return EnumDetail::ChainValToRawEnum<ChainRep, Enum>::get(mVal);
	}

private:
	int mVal;
};


typedef EnumChain<EnumDetail::EmptyEnumChainRep> EmptyChain;
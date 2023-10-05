//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Containers/Maybe.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	EnumChainDetail
//------------------------------------------------------------------------------


namespace EnumDetail
{
	struct EmptyEnumChainRep
	{
		static const size_t kSize = 0;
	};


	template<class FirstArg, size_t kFirstSizeArg, class RestArg>
	struct EnumChainRep
	{
		typedef FirstArg First;
		typedef RestArg Rest;

		static const size_t kFirstSize = kFirstSizeArg;
		static const size_t kSize = kFirstSize + Rest::kSize;
	};


	template<class ChainRep, class Enum>
	struct Helper
	{
		template<int kRawVal>
		struct RawToChain
		{
			static const int kResultWithoutFirst =
				Helper<typename ChainRep::Rest, Enum>
				::template RawToChain<kRawVal>::kResult;

			static const int kResult = kResultWithoutFirst + ChainRep::kFirstVal;
		};

		static int rawToChain(const Enum rawVal)
		{
			const int kResultWithoutFirst =
				Helper<typename ChainRep::Rest, Enum>::rawToChain(rawVal);

			return kResultWithoutFirst + ChainRep::kFirstSize;
		}
	};

	template<size_t kRawEnumSize, class ChainRest, class RawEnum>
	struct Helper<EnumChainRep<RawEnum, kRawEnumSize, ChainRest>, RawEnum>
	{
		// Compile-time version.
		template<RawEnum kRawVal>
		struct RawToChain
		{
			static const int kResult = kRawVal;
		};

		// Runtime version.
		static int rawToChain(const RawEnum rawVal)
		{
			return rawVal;
		}
	};

	template<class ChainRep, class RawEnum>
	struct ChainValToRawEnum
	{
		static Maybe<RawEnum> get(const int chainVal)
		{
			// A build error here likely means that the raw enum you're requesting
			// is not part of the chain.
			const int chainValInRest = chainVal - int(ChainRep::kFirstSize);
			typedef typename ChainRep::Rest ChainRest;
			return ChainValToRawEnum<ChainRest, RawEnum>::get(chainValInRest);
		}
	};


	// Terminating case: Requested enum is at the start of the chain.
	template<size_t kRawEnumSize, class ChainRest, class RawEnum>
	struct ChainValToRawEnum<EnumChainRep<RawEnum, kRawEnumSize, ChainRest>, RawEnum>
	{
		static inline Maybe<RawEnum> get(const int chainVal)
		{
			// Check if the chain value is outside the requested segment.
			if (chainVal < 0 || chainVal >= int(kRawEnumSize))
				return nothing;

			return Maybe<RawEnum>(RawEnum(chainVal));
		}
	};


	template<class ChainRep, class RawEnum, size_t kRawEnumSize>
	struct AppendEnumToChain
	{
		typedef typename ChainRep::First ChainFirst;
		typedef typename ChainRep::Rest ChainRest;
		typedef typename RawEnum EnumType;

		typedef AppendEnumToChain<ChainRest, RawEnum, kRawEnumSize> RestInstance;
		typedef typename RestInstance::Result RestResult;

		typedef EnumChainRep<ChainFirst, ChainRep::kFirstSize, RestResult> Result;
	};


	// Terminating case: The given chain is empty.
	template<class RawEnum, size_t kRawEnumSize>
	struct AppendEnumToChain<EmptyEnumChainRep, RawEnum, kRawEnumSize>
	{
		typedef EnumChainRep<RawEnum, kRawEnumSize, EmptyEnumChainRep> Result;
	};

} //EnumDetail
//------------------------------------------------------------------------------
//  Copyright : (c) 2006
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------
#ifndef BITSET_H
#define BITSET_H

#include <cstring>
#include <intrin.h>
////////////////////////////////////////////////////////////////////////////////


#ifndef bit
#define bit(n) (1 << (n))
#endif

//------------------------------------------------------------------------------
//	Class Definition
//------------------------------------------------------------------------------
template<int N, class T = unsigned int>
class BitSet
{
public:
	typedef BitSet<N, T> This;

	static const int kNumPerIndex = sizeof(T) * 8;
	static const int kArraySize = (N + kNumPerIndex - 1) / kNumPerIndex;
	static const T kAllBits = -1;

	BitSet() { clear(); }
	BitSet(const This& val) { copy(val); }
	BitSet(T val) { clear(); mData[0] = val; }

	void set(size_t n, bool t = true)
	{
		size_t idx = index(n);
		size_t b = size_t(1) << shift(n);

		if (t)
		{
			mData[idx] |= b;
		}
		else
		{
			mData[idx] &= ~b;
		}
	}

	This operator | (const This& rhs) const
	{
		This ret;

		for (int i = 0; i < kArraySize; ++i)
		{
			ret = mData[i] | rhs.mData[i];
		}

		return ret;
	}

	This operator ^ (const This& rhs) const
	{
		This ret;

		for (int i = 0; i < kArraySize; ++i)
		{
			ret = mData[i] ^ rhs.mData[i];
		}

		return ret;
	}

	This operator & (const This& rhs) const
	{
		This ret;

		for (int i = 0; i < kArraySize; ++i)
		{
			ret = mData[i] ^ rhs.mData[i];
		}

		return ret;
	}

	This& operator |= (const This& rhs)
	{
		*this = *this | rhs;

		return *this;
	}

	This& operator ^= (const This& rhs)
	{
		*this = *this ^ rhs;

		return *this;
	}

	This& operator &= (const This& rhs)
	{
		*this = *this & rhs;

		return *this;
	}

	This& operator = (const This& rhs)
	{
		copy(rhs);
		return *this;
	}

	void flip()
	{
		for (int i = 0; i < kArraySize; ++i)
		{
			mData[i] ^= kAllBits;
		}
	}

	This flipped()
	{
		This ret = *this;

		ret.flip();

		return ret;
	}

	int firstSet() const
	{
		for (int i = 0; i < N; ++i)
		{
			if (test(i))
			{
				return i;
			}
		}

		return -1;
	}

	int firstNotSet() const
	{
		for (int i = 0; i < N; ++i)
		{
			if (!test(i))
			{
				return i;
			}
		}

		return -1;
	}

	int lastSet() const
	{
		for (int i = N - 1; i >= 0; --i)
		{
			if (test(i))
			{
				return i;
			}
		}

		return -1;
	}

	int lastNotSet()
	{
		for (int i = N - 1; i >= 0; --i)
		{
			if (!test(i))
			{
				return i;
			}
		}

		return -1;
	}

	bool any() const
	{
		for (int i = 0; i < kArraySize; ++i)
		{
			if (mData[i])
			{
				return true;
			}
		}

		return false;
	}

	bool none() const
	{
		return !any();
	}

	size_t count() const
	{
		size_t ret = 0;

		for (size_t i = 0; i < kArraySize; ++i)
		{
			ret += __popcnt(mData[i]);
		}

		return ret;
	}

	bool operator[] (size_t n) const { return test(n); }
	void copy(const This& rhs) { std::memcpy(mData, rhs.mData, sizeof(mData)); }
	
	void clear()
	{
		for (int i = 0; i < kArraySize; ++i)
		{
			mData[i] = 0;
		}
	}
	
	bool test(size_t n) const { return (mData[index(n)] & (1 << shift(n))) != 0; }

	static size_t shift(size_t n) { return n % kNumPerIndex; }
	static size_t index(size_t n) { return n / kNumPerIndex; }

private:
	T mData[kArraySize];
};


////////////////////////////////////////////////////////////////////////////////
#endif

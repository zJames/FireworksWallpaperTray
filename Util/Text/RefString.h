//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Util.h"
#include "../Pointers/Ptr.h"
#include "../Pointers/SharedPtr.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	RefStringHelper
//------------------------------------------------------------------------------
namespace RefStringHelper
{
	size_t length(const char* text);
	size_t length(const wchar_t* text);
	int compare(const char* first, const char* second);
	int compare(const wchar_t* first, const wchar_t* second);

	template<class T>
	bool compare(const T* first, const T* second)
	{
		if (first == second)
		{
			return true;
		}

		if (first == NULL || second == NULL)
		{
			return false;
		}

		while (*first != 0 && *second != 0)
		{
			if (*first != *second)
			{
				return false;
			}

			++first;
			++second;
		}

		return true;
	}
}


//------------------------------------------------------------------------------
//	RefString
//------------------------------------------------------------------------------
template<class T>
class RefStringT
{
public:
	typedef RefStringT<T> This;

	RefStringT(): mDataSize(0), mConstData(NULL) {}

	RefStringT(size_t dataSize)
		:
		mDataSize(0),
		mConstData(NULL)
	{
		resize(dataSize);
	}
	
	RefStringT(const This& other)
		:
		mDataSize(0),
		mConstData(NULL)
	{
		*this = other; 
	}

	RefStringT(T text[])
		:
		mDataSize(0),
		mConstData(NULL)
	{
		*this = copy(text);
	}

	RefStringT(const T* text)
		:
		mDataSize(0),
		mConstData(text)
	{
	}

	static This copy(const T* text, size_t size = -1)
	{
		This ret;

		if (size == -1)
		{
			size = RefStringHelper::length(text);
		}

		ret.resize(size + 1);

		for (size_t i = 0; i < size; ++i)
		{
			ret.mData[i] = text[i];
		}

		ret.mData[size] = 0;

		return ret;
	}

	T* ptr()
	{
		if (mData.get() == NULL)
		{
			assert(mConstData.get() != NULL);
			*this = copy(mConstData.get());
		}

		return mData.get();
	}

	size_t length() const
	{
		return RefStringHelper::length(const_ptr());
	}

	void set(const T* text)
	{
		mDataSize = 0;
		mData.reset(NULL);
		mConstData = text;
	}

	This& operator=(const T* text)
	{
		set(text);

		return *this;
	}

	This& operator=(const This& other)
	{
		mData = other.mData;
		mDataSize = other.mDataSize;
		mConstData = other.mConstData;

		return *this;
	}

	This& operator +=(const T* text)
	{
		if (text == NULL)
		{
			return *this;
		}

		const size_t oldLen = RefStringHelper::length(const_ptr());
		const size_t copyLen = RefStringHelper::length(text);
		const size_t newLen = max(oldLen + copyLen + 1, mDataSize);
		const Maybe<int> refCount = mData.refCount();

		if ((refCount.get() && *refCount > 1) || newLen > mDataSize) //make intelligent copy at new size
		{
			becomeNextSize(max(newLen, mDataSize * 2));
		}

		for (size_t i = 0; i < (copyLen + 1); ++i)
		{
			mData[i + oldLen] = text[i];
		}

		return *this;
	}

	This& operator += (const T text)
	{
		if (text == 0)
		{
			return *this;
		}

		const size_t oldLen = RefStringHelper::length(const_ptr());
		const size_t newLen = oldLen + 2;
		const Maybe<int> refCount = mData.refCount();

		if ((refCount.get() && *refCount > 1) || newLen > mDataSize) //make intelligent copy at new size
		{
			becomeNextSize(newLen);
		}

		mData[oldLen] = text;
		mData[oldLen + 1] = 0;

		return *this;
	}

	T& operator [](size_t n)
	{
		assert(ptr() != NULL);
		return ptr()[n];
	}

	T& operator [](int n)
	{
		assert(ptr() != NULL);
		return ptr()[n];
	}

	const T& operator [](size_t n) const
	{
		assert(const_ptr() != NULL);
		return const_ptr()[n];
	}

	const T& operator [](int n) const
	{
		assert(const_ptr() != NULL);
		return const_ptr()[n];
	}

	void resize(size_t size)
	{
		if (size == mDataSize) //we are already this size
		{
			return;
		}

		if (size == 0)
		{
			clear();
			return;
		}

		SharedArray<T> newData(new T[size]);

		if (mDataSize > 0)
		{
			for (size_t i = 0; i < mDataSize && i < size; ++i)
			{
				newData[i] = mData[i];
			}
		}
		else
		{
			newData[0] = 0;
		}

		mData = newData;
		mDataSize = size;
		mConstData = NULL;
	}

	void clear()
	{
		mData.reset(0);
		mDataSize = 0;
		mConstData = NULL;
	}

	This& exchange(T toFind, T toReplace)
	{
		if (const_ptr() == NULL)
		{
			return *this;
		}

		if (mData.get() == NULL || mData.refCount() > 1)
		{
			*this = copy();
		}

		for (T* i = ptr(); *i != 0; ++i)
		{
			if (*i == toFind)
			{
				*i = toReplace;
			}
		}

		return *this;
	}

	This& exchange(const T* toFind, const T* toReplace)
	{
		if (const_ptr() == NULL)
		{
			return *this;
		}

		This old = *this;

		*this = "";
		const size_t toFindLen = RefStringHelper::length(toFind);

		for (const T* i = old.c_str(); *i != 0; ++i)
		{
			if (RefStringHelper::compare(i, toFind) == 0)
			{
				*this += toReplace;
				i += toFindLen - 1;
			}
			else
			{
				*this += *i;
			}
		}

		return *this;
	}

	bool empty() const
	{
		return (mConstData.get() == NULL) && (mData.get() == NULL);
	}

	This copy() const
	{
		This ret;

		const size_t len = (mDataSize == 0) ? length() + 1 : mDataSize;
		ret.mData = makeCopy(len);
		ret.mDataSize = len;

		return ret;
	}

	This replace(T toFind, T toReplace) const
	{
		This ret = copy();

		ret.exchange(toFind, toReplace);

		return ret;
	}

	This replace(const T* toFind, const T* toReplace) const
	{
		This ret = copy();

		ret.exchange(toFind, toReplace);

		return ret;
	}

	const T* const_ptr() const
	{
		return mConstData.get() ? mConstData.get() : mData.get();
	}

	inline const T* c_str() const
	{
		return const_ptr();
	}

	size_t count(T toFind) const
	{
		size_t ret = 0;

		for (const T* i = const_ptr(); i != NULL && *i != 0; ++i)
		{
			if (*i == toFind)
			{
				++ret;
			}
		}

		return ret;
	}

	bool operator <(const char* other) const
	{
		return RefStringHelper::compare(const_ptr(), other) < 0;
	}

	bool operator ==(const This& other) const
	{
		return *this == other.const_ptr();
	}

	bool operator ==(const T* other) const
	{
		const T* myText = const_ptr();

		if (other == myText)
		{
			return true;
		}

		if (myText == NULL && other != NULL && *other == 0)
		{
			return true;
		}

		if (myText == NULL || other == NULL)
		{
			return false;
		}

		while (*myText && *other)
		{
			if (*myText != *other)
			{
				return false;
			}

			++myText;
			++other;
		}

		//We need one final check in case one of the pointers became NULL
		return *myText == *other;
	}

	bool operator !=(const T* other) const
	{
		return !(*this == other);
	}

	This operator + (const T text) const
	{
		This ret = *this;

		ret += text;

		return ret;
	}

	This operator + (const T* text) const
	{
		This ret = *this;

		ret += text;

		return ret;
	}

	operator const T*() const
	{
		return const_ptr();
	}

	size_t dataSize() const
	{
		return mDataSize;
	}

private:
	static SharedArray<T> copyBuffer(size_t newBufferSize, const T* source, size_t sourceSize)
	{
		SharedArray<T> ret(new T[newBufferSize]);

		for (size_t i = 0; i < newBufferSize && i < sourceSize; ++i)
		{
			ret[i] = source[i];
		}

		return ret;
	}

	SharedArray<T> makeCopy(size_t copyBufferSize) const
	{
		SharedArray<T> ret;

		if (mConstData.get())
		{
			size_t len = RefStringHelper::length(mConstData.get());
			ret = copyBuffer(copyBufferSize, mConstData.get(), len + 1);
		}
		else if (mData.get())
		{
			ret = copyBuffer(copyBufferSize, mData.get(), mDataSize);
		}
		else
		{
			ret.reset(new T[copyBufferSize]);
			ret[0] = 0;
		}

		return ret;
	}

	size_t nextDataSize() const
	{
		return max(mDataSize, 16) * 2;
	}

	void becomeNextSize(size_t minDataSize)
	{
		const size_t newDataSize = max(nextDataSize(), minDataSize);
		mData = makeCopy(newDataSize);
		mDataSize = newDataSize;
		mConstData = NULL;
	}

	size_t mDataSize;
	Ptr<const T> mConstData;
	SharedArray<T> mData;
};


typedef RefStringT<char> RefString;
typedef RefStringT<wchar_t> RefStringW;
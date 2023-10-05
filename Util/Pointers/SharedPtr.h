//------------------------------------------------------------------------------
//	SharedPtr.h
//	iOGL
//
//	Created by James Prettyman on 9/2/10.
//	Copyright 2010 Smashing Ideas Inc. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#include "..\Containers\Maybe.h"
#include "..\Core\ReferenceCounter.h"

#include <assert.h>
////////////////////////////////////////////////////////////////////////////////


template<class T>
class SharedPtr
{
public:
	typedef SharedPtr<T> This;

	SharedPtr(T* ptr = NULL) : mPtr(NULL) { set(ptr); }

	template<class S>
	SharedPtr(const SharedPtr<S>& other) : mPtr(NULL) { set(other); }

	~SharedPtr() { clear(); }

	void set(T* ptr)
	{
		clear();

		if (ptr != NULL)
		{
			mPtr = ptr;
			mRefCount.create();
		}
	}

	template<class S>
	void set(const SharedPtr<S>& other)
	{
		clear();

		mPtr = other.mPtr;
		mRefCount = other.mRefCount;
	}

	void clear()
	{
		const Maybe<int>& ref = mRefCount.release();

		if (ref == 0)
		{
			delete mPtr;
			mPtr = NULL;
		}
	}

	T* operator * () { return mPtr; }
	T* operator -> () { return mPtr; }
	T* get() { return mPtr; }

	Maybe<int> refCount() const { return mRefCount.current(); }

	const T* operator -> () const { return mPtr; }
	const T* get() const { return mPtr; }
	const T* operator * () const { return mPtr; }

	This& operator = (T* ptr) { set(ptr); return *this; }
	This& operator = (const This& other) { set(other); return *this; }

	bool operator == (const This& other) const { return mPtr == other.mPtr; }

private:
	friend class SharedPtr;

	ReferenceCounter mRefCount;
	T* mPtr;
};




//------------------------------------------------------------------------------
//	Shared Array
//------------------------------------------------------------------------------
template<class T>
class SharedArray
{
public:
	typedef SharedArray<T> This;

	SharedArray() : mPtr(0) {}

	SharedArray(const This& other)
		: mPtr(other.mPtr)
		, mRefCount(other.mRefCount)
	{
	}

	SharedArray(T* array) : mPtr(0)
	{
		reset(array);
	}

	~SharedArray()
	{
		reset(0);
	}

	void reset(T* ptr)
	{
		decRef();

		if (ptr)
		{
			mPtr = ptr;
			mRefCount.create();
		}
	}

	This& operator =(const This& other)
	{
		decRef();

		mPtr = other.mPtr;
		mRefCount = other.mRefCount;

		return *this;
	}

	T* get() { return mPtr; }
	const T* get() const { return mPtr; }

	Maybe<int> refCount() const
	{
		return mRefCount.current();
	}

	T& operator[] (size_t n) { assert(mPtr != NULL); return mPtr[n]; }
	const T& operator[] (size_t n) const { assert(mPtr != NULL); return mPtr[n]; }

private:
	void decRef()
	{
		if (mRefCount.release() == 0)
		{
			delete[] mPtr;
		}

		mPtr = 0;
	}

	ReferenceCounter mRefCount;
	T* mPtr;
};

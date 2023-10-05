//------------------------------------------------------------------------------
//  Copyright : (c) 2007
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------
#pragma once

#include <assert.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Pointer Wrapper
//------------------------------------------------------------------------------
template<class T>
class Ptr
{
public:
	typedef Ptr<T> This;

	Ptr(T* ptr = NULL): mPtr(ptr) {}

	T* operator -> () const				{ assert(mPtr != NULL); return mPtr; }
	T& operator * () const				{ assert(mPtr != NULL); return *mPtr; }
	const T& operator[] (int n) const	{ assert(mPtr != NULL); return mPtr[n]; }
	T* get() const						{ return mPtr; }
	void set(T* ptr)						{ mPtr = ptr; }
	T& operator[] (int n)				{ assert(mPtr != NULL); return mPtr[n]; }
	This& operator++()					{ assert(mPtr != 0);  ++mPtr; return *this; }

	bool operator == (const This& other) const { return mPtr == other.mPtr; }
	bool operator != (const This& other) const { return mPtr != other.mPtr; }

private:
	T* mPtr;
};

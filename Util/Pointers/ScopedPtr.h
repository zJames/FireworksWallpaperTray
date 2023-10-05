//------------------------------------------------------------------------------
//  Copyright : (c) 2006
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "..\Core\NonCopyable.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Scoped Pointer Class
//------------------------------------------------------------------------------
template<class T>
class ScopedPtr : NonCopyable
{
public:
	typedef ScopedPtr<T> This;

	ScopedPtr(): mPtr(0) {}
	ScopedPtr(T* ptr): mPtr(0) { set(ptr); }
	~ScopedPtr() { delete mPtr; }

	T* operator -> () const			{ return mPtr; }
	void set(T* ptr)				{ delete mPtr; mPtr = ptr; }
	void set(This& ptr)				{ delete mPtr; mPtr = ptr.mPtr; ptr.mPtr = 0; }
	This& operator = (T* ptr)		{ set(ptr); return *this; }
	This& operator = (This& rhs)	{ set(rhs); return *this; }
	operator bool () const			{ return mPtr != 0; }
	T* get() const					{ return mPtr; }
	T* release()					{ T* ptr = mPtr; mPtr = 0; return ptr; }

private:
	T* mPtr;
};


//------------------------------------------------------------------------------
//	Scoped Array Class
//------------------------------------------------------------------------------
template<class T>
class ScopedArray : NonCopyable
{
public:
	typedef ScopedArray<T> This;

	ScopedArray(): mPtr(0)		{}
	ScopedArray(T* ptr): mPtr(0)	{ set(ptr); }
	~ScopedArray()					{ delete [] mPtr; }

	T& operator [] (size_t n) const	{ return mPtr[n]; }
	void set(T* ptr)				{ delete mPtr; mPtr = ptr; }
	void set(This& ptr)				{ delete mPtr; mPtr = ptr.mPtr; ptr.mPtr = 0; }
	This& operator = (T* ptr)		{ set(ptr); return *this; }
	This& operator = (This& rhs)	{ set(rhs); return *this; }
	operator bool () const			{ return mPtr != 0; }
	T* get() const					{ return mPtr; }
	T* release()					{ T* ptr = mPtr; mPtr = 0; return ptr; }

private:
	T* mPtr;
};

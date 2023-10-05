//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	NonNull
//------------------------------------------------------------------------------
template<class T>
class NonNull
{
public:
	typedef NonNull<T> This;

	NonNull(T* ptr): mPtr(ptr) { assert(ptr != NULL); }

	T* operator -> () const				{ return mPtr; }
	T& operator * () const				{ return *mPtr; }
	const T& operator[] (int n) const	{ return mPtr[n]; }
	T* get() const						{ return mPtr; }
	void set(T* ptr)					{ mPtr = ptr; }
	T& operator[] (int n)				{ return mPtr[n]; }
	This& operator++()					{ ++mPtr; return *this; }

	bool operator == (const This& other) const { return mPtr == other.mPtr; }

private:
	T* mPtr;
};

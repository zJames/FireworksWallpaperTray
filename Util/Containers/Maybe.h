//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "ObjBuff.h"
#include "../Core/SafeBool.h"

#include <assert.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Maybe
//------------------------------------------------------------------------------
namespace MaybeDetail
{
struct Nothing
{
};
}


extern const MaybeDetail::Nothing nothing;


template<class T>
class Maybe : public SafeBool<Maybe<T> >
{
public:
	Maybe();

	Maybe(MaybeDetail::Nothing);
	Maybe(const T*);
	Maybe(const T&);
	Maybe(const Maybe<T>&);

	~Maybe();

	Maybe<T>& operator=(const Maybe<T>&);

	void construct();

	template<class A1>
	void construct(A1&);

	template<class A1, class A2>
	void construct(A1&, A2&);

	template<class A1, class A2, class A3>
	void construct(A1&, A2&, A3&);

	template<class A1, class A2, class A3, class A4>
	void construct(A1&, A2&, A3&, A4&);

	template<class A1, class A2, class A3, class A4, class A5>
	void construct(A1&, A2&, A3&, A4&, A5&);

	template<class A1, class A2, class A3, class A4, class A5, class A6>
	void construct(A1&, A2&, A3&, A4&, A5&, A6&);

	template<class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	void construct(A1&, A2&, A3&, A4&, A5&, A6&, A7&);

	template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	void construct(A1&, A2&, A3&, A4&, A5&, A6&, A7&, A8&);

	T* get();
	const T* get() const;

	T& operator*();
	const T& operator*() const;

	T* operator->();
	const T* operator->() const;

	bool safeBoolValue() const;

	bool operator==(const Maybe&) const;
	bool operator!=(const Maybe&) const;

	Maybe<T>& operator=(MaybeDetail::Nothing);
	bool operator==(MaybeDetail::Nothing) const;
	bool operator!=(MaybeDetail::Nothing) const;

	Maybe<T>& operator=(const T&);
	bool operator==(const T&) const;
	bool operator!=(const T&) const;

private:
	ObjBuf<T> mObj;
};


template<class T>
inline Maybe<T>::Maybe()
{
}


template<class T>
inline Maybe<T>::Maybe(MaybeDetail::Nothing)
{
}


template<class T>
inline Maybe<T>::Maybe(const T& t)
{
	mObj.construct(t);
}


template<class T>
inline Maybe<T>::Maybe(const T* ptr)
{
	if (ptr != NULL)
	{
		mObj.construct(*ptr);
	}
}


template<class T>
inline Maybe<T>::Maybe(const Maybe<T>& rv)
{
	if (rv.mObj.constructed())
	{
		mObj.construct(*rv.mObj);
	}
}


template<class T>
inline Maybe<T>::~Maybe()
{
}


template<class T>
inline Maybe<T>& Maybe<T>::operator=(const Maybe<T>& rv)
{
	if (this != &rv)
	{
		mObj.erase();

		if (rv.mObj.constructed())
		{
			mObj.construct(*rv.mObj);
		}
	}

	return *this;
}


template<class T>
inline void Maybe<T>::construct()
{
	mObj.construct();
}


template<class T>
template<class A1>
inline void Maybe<T>::construct(A1& a1)
{
	mObj.construct(a1);
}


template<class T>
template<class A1, class A2>
inline void Maybe<T>::construct(A1& a1, A2& a2)
{
	mObj.construct(a1, a2);
}


template<class T>
template<class A1, class A2, class A3>
inline void Maybe<T>::construct(A1& a1, A2& a2, A3& a3)
{
	mObj.construct(a1, a2, a3);
}


template<class T>
template<class A1, class A2, class A3, class A4>
inline void Maybe<T>::construct(A1& a1, A2& a2, A3& a3, A4& a4)
{
	mObj.construct(a1, a2, a3, a4);
}


template<class T>
template<class A1, class A2, class A3, class A4, class A5>
inline void Maybe<T>::construct(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5)
{
	mObj.construct(a1, a2, a3, a4, a5);
}


template<class T>
template<class A1, class A2, class A3, class A4, class A5, class A6>
inline void Maybe<T>::construct(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6)
{
	mObj.construct(a1, a2, a3, a4, a5, a6);
}


template<class T>
template<class A1, class A2, class A3, class A4, class A5, class A6, class A7>
inline void Maybe<T>::construct(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6, A7& a7)
{
	mObj.construct(a1, a2, a3, a4, a5, a6, a7);
}


template<class T>
template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
inline void Maybe<T>::construct(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6, A7& a7, A8& a8)
{
	mObj.construct(a1, a2, a3, a4, a5, a6, a7, a8);
}


template<class T>
inline T* Maybe<T>::get()
{
	return mObj.constructed() ? mObj.get() : 0;
}


template<class T>
inline const T* Maybe<T>::get() const
{
	return mObj.constructed() ? mObj.get() : 0;
}


template<class T>
inline T& Maybe<T>::operator*()
{
	assert(mObj.constructed());
	return *mObj.get();
}


template<class T>
inline const T& Maybe<T>::operator*() const
{
	assert(mObj.constructed());
	return *mObj.get();
}


template<class T>
inline T* Maybe<T>::operator->()
{
	assert(mObj.constructed());
	return mObj.get();
}


template<class T>
inline const T* Maybe<T>::operator->() const
{
	assert(mObj.constructed());
	return mObj.get();
}


template<class T>
inline bool Maybe<T>::safeBoolValue() const
{
	return get();
}


template<class T>
inline bool Maybe<T>::operator==(const Maybe<T>& rv) const
{
	if (!mObj.constructed())
		return !rv.constructed();

	return rv.mObj.constructed() && *mObj == *rv.mObj;
}


template<class T>
inline bool Maybe<T>::operator!=(const Maybe& rv) const
{
	return !(*this == rv);
}


template<class T>
inline Maybe<T>& Maybe<T>::operator=(MaybeDetail::Nothing)
{
	mObj.erase();

	return *this;
}


template<class T>
inline bool Maybe<T>::operator==(MaybeDetail::Nothing) const
{
	return !mObj.constructed();
}


template<class T>
inline bool Maybe<T>::operator!=(MaybeDetail::Nothing) const
{
	return mObj.constructed();
}


template<class T>
inline Maybe<T>& Maybe<T>::operator=(const T& rv)
{
	mObj.construct(rv);

	return *this;
}


template<class T>
inline bool Maybe<T>::operator==(const T& rv) const
{
	return mObj.constructed() && *mObj == rv;
}


template<class T>
inline bool Maybe<T>::operator!=(const T& rv) const
{
	return !(*this == rv);
}
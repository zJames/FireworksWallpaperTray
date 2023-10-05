//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Core/NonCopyable.h"

#include <new>
////////////////////////////////////////////////////////////////////////////////


typedef unsigned char BYTE;


template<class T>
class StructBuf : NonCopyable
{
public:
	static const int kBuffSize = sizeof(T);

	inline T* get() { return reinterpret_cast<T*>(mData); }
	inline T* operator ->() { return get(); }
	inline T& operator *() { return *get(); }

	inline const T* get() const { return reinterpret_cast<const T*>(mData); }
	inline const T* operator ->() const { return get(); }
	inline const T& operator *() const { return *get(); }

	void construct()
	{
		new (mData) T();
	}

	template<typename... Arguments>
	void construct(Arguments&... args)
	{
		new (mData) T(args...);
	}

	void erase()
	{
		get()->~T();
	}

private:
	BYTE mData[kBuffSize];
};


//------------------------------------------------------------------------------
//	ObjBuff
//------------------------------------------------------------------------------
template<class T>
class ObjBuf : public StructBuf<T>
{
public:
	static const int kBuffSize = sizeof(T);

	ObjBuf() : mConstructed(false) {}
	~ObjBuf() { erase(); }

	inline bool constructed() const { return mConstructed; }

	void construct()
	{
		erase();
		StructBuf<T>::construct();
		mConstructed = true;
	}

	template<typename... Arguments>
	void construct(Arguments&... args)
	{
		erase();
		StructBuf<T>::construct(args...);
		mConstructed = true;
	}

	void erase()
	{
		if (mConstructed)
		{
			StructBuf<T>::erase();
		}

		mConstructed = false;
	}

private:
	bool mConstructed;
};

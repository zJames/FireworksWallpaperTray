//------------------------------------------------------------------------------
//  Copyright : (c) 2007
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------
#ifndef COMPTR_H
#define COMPTR_H
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	COM Pointer
//------------------------------------------------------------------------------
template<class T>
class ComPtr
{
public:
	typedef ComPtr<T> This;

	ComPtr(): mPtr(NULL) {}
	ComPtr(T ptr): mPtr(NULL) { set(ptr); }
	ComPtr(const This& rhs): mPtr(NULL) { set(rhs.mPtr); }
	~ComPtr() { release(); }

	inline T operator ->() const { return mPtr; }

	inline T* operator& () { return mPtr ? NULL : &mPtr; }
	inline T operator* () const { return mPtr; }

	inline operator bool() const { return mPtr != NULL; }

	inline This& operator =(const This& rhs)
	{
		set(rhs.mPtr);
		return *this;
	}

	void attach(T ptr) // Warning, this function is dangerous
	{
		release();

		mPtr = ptr;
	}

	T get() const { return mPtr; }

	void set(T ptr)
	{
		release();

		if (ptr == NULL)
			return;

		mPtr = ptr;
		ptr->AddRef();
	}

	void release()
	{
		if (mPtr)
			mPtr->Release();

		mPtr = 0;
	}

private:
	T mPtr;
};


////////////////////////////////////////////////////////////////////////////////
#endif
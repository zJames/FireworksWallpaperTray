//------------------------------------------------------------------------------
//  Copyright : (c) 2006
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------
#ifndef REFPTR_H
#define REFPTR_H
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Referenced Pointer Data
//------------------------------------------------------------------------------
struct RefData
{
protected:
	static void addRef(void*);
	static bool decRef(void*);
};


//------------------------------------------------------------------------------
//	Referenced Pointer
//------------------------------------------------------------------------------
template<class T>
class RefPtr: public RefData
{
public:
	typedef RefPtr<T> This;

	RefPtr(): mPtr(0)		{}
	RefPtr(T* t): mPtr(0)	{ set(t); }
	~RefPtr()				{ set((T*)0); }

	void set(T* ptr)					{ if (decRef(mPtr)) delete mPtr; mPtr = ptr; addRef(mPtr); }
	This& operator =(const This& rhs)	{ set(rhs.mPtr); return *this; }
	
	operator T* ()						{ return mPtr;}
	operator const T* () const			{ return mPtr;}
	T& operator[] (int idx)				{ return mPtr[idx]; }
	const T& operator[] (int idx) const	{ return mPtr[idx]; }
	T* operator ->()					{ return mPtr; }
	const T* operator ->() const		{ return mPtr; }
	operator bool ()					{ return mPtr != nullptr; }

private:
	T* mPtr;
};


////////////////////////////////////////////////////////////////////////////////
#endif
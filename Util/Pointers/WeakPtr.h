//------------------------------------------------------------------------------
//  Copyright : (c) 2007
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------
#ifndef WEAKPTR_H
#define WEAKPTR_H

#include <map>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Forward Declarations
//------------------------------------------------------------------------------
class WeakPtrBase;
class Weakable;


//------------------------------------------------------------------------------
//	Weak Pointer Base Class
//------------------------------------------------------------------------------
class WeakPtrBase
{
private:
	const Weakable* weakObject;
	WeakPtrBase* nextPtr;

	void weakLink(const Weakable* w);
	void weakUnlink();

	void operator=(WeakPtrBase const&);
	void operator=(const Weakable*);

protected:
	inline WeakPtrBase() : weakObject(0), nextPtr(0) {}
	inline WeakPtrBase(WeakPtrBase const& w)	{ weakLink(w.weakObject); }
	inline WeakPtrBase(const Weakable* p)		{ weakLink(p); }
	inline ~WeakPtrBase() { weakUnlink(); }

public:
	inline void weakSet(const Weakable* w)
	{
		weakUnlink();
		weakLink(w);
	}

	inline const Weakable* weakGet() const { return weakObject; }
	inline bool isLast() const { return !nextPtr; }
};


//------------------------------------------------------------------------------
//	Weakable class, the class to inheirit from
//------------------------------------------------------------------------------
class Weakable
{
private:
	mutable WeakPtrBase* weakPtrList;
	friend class WeakPtrBase;

public:
	inline Weakable() : weakPtrList(0) {}
	inline Weakable(Weakable const&) : weakPtrList(0) {}
	inline Weakable const& operator=(Weakable const&) { return *this; }

protected:
	void clearWeakPtrList();
	inline ~Weakable() { clearWeakPtrList(); }
};


template<class T> class WeakableTraits {
public:
	inline static const Weakable* toWeak(T* t) { return t; }

	inline static T* fromWeak(const Weakable* w)
	{
		return static_cast<T*>(const_cast<Weakable*>(w));
	}
};


//------------------------------------------------------------------------------
//	Weak Pointer, point to weakables
//------------------------------------------------------------------------------
template<class T>
class WeakPtr : private WeakPtrBase
{
public:
	typedef WeakableTraits<T> Traits;

	inline WeakPtr() { }
	inline WeakPtr(const WeakPtr<T>& p): WeakPtrBase(p) {}
	inline explicit WeakPtr(T* p): WeakPtrBase(Traits::toWeak(p)) {}

	operator bool () const { return get() != NULL; }

	template<class Base>
	inline operator WeakPtr<Base>() const
	{
		return WeakPtr<Base>(get());
	}

	inline const WeakPtr& operator=(const WeakPtr<T>& p)
	{
		set(p.get());
		return *this;
	}

	inline bool operator==(const WeakPtr& rv) const
	{
		return get() == rv.get();
	}

	inline bool operator!=(const WeakPtr& rv) const { return !(*this == rv); }
	inline void set(T* p = NULL) { weakSet(Traits::toWeak(p)); }

	inline T* get() const { return Traits::fromWeak(WeakPtrBase::weakGet()); }

	inline T& operator*() const
	{
		T* p = get();
		return *p;
	}

	inline T* operator->() const
	{
		T* p = get();
		return p;
	}
};


//------------------------------------------------------------------------------
//	Indirect Weak Pointer, point to data other than the weakable
//------------------------------------------------------------------------------
template<class T>
class IndirectWeakPtr
{
public:
	inline IndirectWeakPtr() { }

	inline IndirectWeakPtr(const IndirectWeakPtr<T>& p)
		:mWeakPtr(p.mWeakPtr)
		,mRawPtr(p.mRawPtr)
	{
	}

	inline explicit IndirectWeakPtr(const Weakable* weakable, T* rawPtr)
		:mWeakPtr(weakable)
		,mRawPtr(rawPtr)
	{
	}

	template<class Base>
	inline operator IndirectWeakPtr<Base>() const
	{
		return IndirectWeakPtr<Base>(mWeakPtr, mRawPtr);
	}

	inline const IndirectWeakPtr& operator=(const IndirectWeakPtr<T>& p)
	{
		mWeakPtr = p.mWeakPtr;
		mRawPtr = p.mRawPtr;

		return *this;
	}

	inline void set(const Weakable* weakable, T* rawPtr)
	{
		mWeakPtr.set(weakable);
		mRawPtr = rawPtr;
	}

	inline const Weakable* getWeakable() const { return mWeakPtr.get(); }

	inline T* get() const { return mWeakPtr.get() ? mRawPtr : NULL; }
	inline T& operator*() const { return *mRawPtr; }
	inline T* operator->() const { return mRawPtr; }

private:
	WeakPtr<const Weakable> mWeakPtr;
	T* mRawPtr;
};


template<class T>
inline WeakPtr<T> makeWeakPtr(T* p)
{
	return WeakPtr<T>(p);
}

////////////////////////////////////////////////////////////////////////////////
#endif
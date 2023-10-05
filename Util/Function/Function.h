#pragma once

#include "FastDelegate.h"
#include "../Pointers/SharedPtr.h"

////////////////////////////////////////////////////////////////////////////////

namespace FuncDetail
{
	template<class R>
	class FuncBase0;

	template<class R, class Af>
	class FuncBase1;

	template<class R, class Af, class Bf>
	class FuncBase2;

	template<class R, class Af, class Bf, class Cf>
	class FuncBase3;

	template<class R, class Af, class Bf, class Cf, class Df>
	class FuncBase4;

	template<class R, class Af, class Bf, class Cf, class Df, class Ef>
	class FuncBase5;
}


template<typename Signature>
class Function;


template<typename R>
class Function<R()>
{
public:
	typedef Function<typename R()> SelfType;
	typedef FuncDetail::FuncBase0<typename R> PtrType;

	Function() {}
	Function(const SelfType& other): mPtr(other.mPtr) {}
	Function(const SharedPtr<PtrType>& ptr): mPtr(ptr) {}

	typename R operator() () const { return (*mPtr.get())(); }

	SelfType& operator =(const SelfType& other) { mPtr = other.mPtr; return *this; }

	bool empty() const { return mPtr.get() == NULL; }

private:
	SharedPtr<PtrType> mPtr;
};


template<typename R, class Af>
class Function<R(Af)>
{
public:
	typedef Function<typename R(Af)> SelfType;
	typedef FuncDetail::FuncBase1<typename R, Af> PtrType;

	Function() {}
	Function(const SelfType& other): mPtr(other.mPtr) {}
	Function(const SharedPtr<PtrType>& ptr): mPtr(ptr) {}

	typename R operator() (Af a) { return (*mPtr.get())(a); }

	SelfType& operator =(const SelfType& other) { mPtr = other.mPtr; return *this; }

	bool empty() const { return mPtr.get() == NULL; }

private:
	SharedPtr<PtrType> mPtr;
};


template<typename R, class Af, class Bf>
class Function<R(Af, Bf)>
{
public:
	typedef Function<typename R(Af, Bf)> SelfType;
	typedef FuncDetail::FuncBase2<typename R, Af, Bf> PtrType;

	Function() {}
	Function(const SelfType& other): mPtr(other.mPtr) {}
	Function(const SharedPtr<PtrType>& ptr): mPtr(ptr) {}

	typename R operator() (Af a, Bf b) { return (*mPtr.get())(a, b); }

	SelfType& operator =(const SelfType& other) { mPtr = other.mPtr; return *this; }

	bool empty() const { return mPtr.get() == NULL; }

private:
	SharedPtr<PtrType> mPtr;
};


template<typename R, class Af, class Bf, class Cf>
class Function<R(Af, Bf, Cf)>
{
public:
	typedef Function<typename R(Af, Bf, Cf)> SelfType;
	typedef FuncDetail::FuncBase3<typename R, Af, Bf, Cf> PtrType;

	Function() {}
	Function(const SelfType& other): mPtr(other.mPtr) {}
	Function(const SharedPtr<PtrType>& ptr): mPtr(ptr) {}

	typename R operator() (Af a, Bf b, Cf c) { return (*mPtr.get())(a, b, c); }

	SelfType& operator =(const SelfType& other) { mPtr = other.mPtr; return *this; }

	bool empty() const { return mPtr.get() == NULL; }

private:
	SharedPtr<PtrType> mPtr;
};


template<typename R, class Af, class Bf, class Cf, class Df>
class Function<R(Af, Bf, Cf, Df)>
{
public:
	typedef Function<typename R(Af, Bf, Cf, Df)> SelfType;
	typedef FuncDetail::FuncBase4<typename R, Af, Bf, Cf, Df> PtrType;

	Function() {}
	Function(const SelfType& other): mPtr(other.mPtr) {}
	Function(const SharedPtr<PtrType>& ptr): mPtr(ptr) {}

	typename R operator() (Af a, Bf b, Cf c, Df d) { return (*mPtr.get())(a, b, c, d); }

	SelfType& operator =(const SelfType& other) { mPtr = other.mPtr; return *this; }

	bool empty() const { return mPtr.get() == NULL; }

private:
	SharedPtr<PtrType> mPtr;
};


template<typename R, class Af, class Bf, class Cf, class Df, class Ef>
class Function<R(Af, Bf, Cf, Df, Ef)>
{
public:
	typedef Function<typename R(Af, Bf, Cf, Df, Ef)> SelfType;
	typedef FuncDetail::FuncBase5<typename R, Af, Bf, Cf, Df, Ef> PtrType;

	Function() {}
	Function(const SelfType& other): mPtr(other.mPtr) {}
	Function(const SharedPtr<PtrType>& ptr): mPtr(ptr) {}

	typename R operator() (Af a, Bf b, Cf c, Df d, Ef e) { return (*mPtr.get())(a, b, c, d, e); }

	SelfType& operator =(const SelfType& other) { mPtr = other.mPtr; return *this; }

	bool empty() const { return mPtr.get() == NULL; }

private:
	SharedPtr<PtrType> mPtr;
};


#include "Function_Impl.h"
#include "Function_Makers.h"

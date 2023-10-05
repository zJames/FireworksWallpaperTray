#pragma once

#include "../Pointers/SharedPtr.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Function<typename R()>
//------------------------------------------------------------------------------
template<class R>
Function<typename R()> func(typename R(*func)())
{
	return
		Function<R()>(
			SharedPtr<FuncDetail::FuncBase0<R> >(
				new FuncDetail::Function0a0<R>(func)));
}


template<class R, class A, class Af>
Function<typename R()> func(typename R(*func)(typename Af), const A& a)
{
	return
		Function<R()>(
			SharedPtr<FuncDetail::FuncBase0<R> >(
				new FuncDetail::Function1a0<R, A, Af>(func, a)));
}


template<class R, class A, class B, class Af, class Bf>
Function<typename R()> func(typename R(*func)(typename Af, typename Bf), const A& a, const B& b)
{
	return
		Function<R()>(
			SharedPtr<FuncDetail::FuncBase0<R> >(
				new FuncDetail::Function2a0<R, A, B, Af, Bf>(func, a, b)));
}


template<class R, class A, class B, class C, class Af, class Bf, class Cf>
Function<typename R()> func(typename R(*func)(typename Af, typename Bf, typename Cf), const A& a, const B& b, const C& c)
{
	return
		Function<R()>(
			SharedPtr<FuncDetail::FuncBase0<R> >(
				new FuncDetail::Function3a0<R, A, B, C, Af, Bf, Cf>(func, a, b, c)));
}


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df>
Function<typename R()> func(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df), const A& a, const B& b, const C& c, const D& d)
{
	return
		Function<R()>(
			SharedPtr<FuncDetail::FuncBase0<R> >(
				new FuncDetail::Function4a0<R, A, B, C, D, Af, Bf, Cf, Df>(func, a, b, c, d)));
}


//------------------------------------------------------------------------------
//	Function<typename R(Af)>
//------------------------------------------------------------------------------
template<class R, class Af>
Function<typename R(typename Af)> func(typename R(*func)(Af))
{
	return
		Function<R(Af)>(
			SharedPtr<FuncDetail::FuncBase1<R, Af> >(
				new FuncDetail::Function0a1<R, Af>(func)));
}


template<class R, class A, class Af, class Bf>
Function<typename R(typename Bf)> func(typename R(*func)(typename Af, Bf), const A& a)
{
	return
		Function<R(Bf)>(
			SharedPtr<FuncDetail::FuncBase1<R, Bf> >(
				new FuncDetail::Function1a1<R, A, Af, Bf>(func, a)));
}


template<class R, class A, class B, class Af, class Bf, class Cf>
Function<typename R(typename Cf)> func(typename R(*func)(typename Af, typename Bf, Cf), const A& a, const B& b)
{
	return
		Function<R(Cf)>(
			SharedPtr<FuncDetail::FuncBase1<R, Cf> >(
				new FuncDetail::Function2a1<R, A, B, Af, Bf, Cf>(func, a, b)));
}


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df>
Function<typename R(typename Df)> func(typename R(*func)(typename Af, typename Bf, typename Cf, Df), const A& a, const B& b, const C& c)
{
	return
		Function<R(Df)>(
			SharedPtr<FuncDetail::FuncBase1<R, Df> >(
				new FuncDetail::Function3a1<R, A, B, C, Af, Bf, Cf, Df>(func, a, b, c)));
}


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef>
Function<typename R(typename Ef)> func(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef), const A& a, const B& b, const C& c, const D& d)
{
	return
		Function<R(Ef)>(
			SharedPtr<FuncDetail::FuncBase1<R, Ef> >(
				new FuncDetail::Function4a1<R, A, B, C, D, Af, Bf, Cf, Df, Ef>(func, a, b, c, d)));
}


//------------------------------------------------------------------------------
//	Function<typename R(Af, Bf)>
//------------------------------------------------------------------------------
template<class R, class Af, class Bf>
Function<typename R(typename Af, typename Bf)> func(typename R(*func)(Af, Bf))
{
	return
		Function<R(Af, Bf)>(
			SharedPtr<FuncDetail::FuncBase2<R, Af, Bf> >(
				new FuncDetail::Function0a2<R, Af, Bf>(func)));
}


template<class R, class A, class Af, class Bf, class Cf>
Function<typename R(typename Bf, typename Cf)> func(typename R(*func)(typename Af, Bf, Cf), const A& a)
{
	return
		Function<R(Bf, Cf)>(
			SharedPtr<FuncDetail::FuncBase2<R, Bf, Cf> >(
				new FuncDetail::Function1a2<R, A, Af, Bf, Cf>(func, a)));
}


template<class R, class A, class B, class Af, class Bf, class Cf, class Df>
Function<typename R(typename Cf, typename Df)> func(typename R(*func)(typename Af, typename Bf, Cf, Df), const A& a, const B& b)
{
	return
		Function<R(Cf, Df)>(
			SharedPtr<FuncDetail::FuncBase2<R, Cf, Df> >(
				new FuncDetail::Function2a2<R, A, B, Af, Bf, Cf, Df>(func, a, b)));
}


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df, class Ef>
Function<typename R(typename Df, typename Ef)> func(typename R(*func)(typename Af, typename Bf, typename Cf, Df, Ef), const A& a, const B& b, const C& c)
{
	return
		Function<R(Df, Ef)>(
			SharedPtr<FuncDetail::FuncBase2<R, Df, Ef> >(
				new FuncDetail::Function3a2<R, A, B, C, Af, Bf, Cf, Df, Ef>(func, a, b, c)));
}


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef, class Ff>
Function<typename R(typename Ef, typename Ff)> func(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff), const A& a, const B& b, const C& c, const D& d)
{
	return
		Function<R(Ef, Ff)>(
			SharedPtr<FuncDetail::FuncBase2<R, Ef, Ff> >(
				new FuncDetail::Function4a2<R, A, B, C, D, Af, Bf, Cf, Df, Ef, Ff>(func, a, b, c, d)));
}


//------------------------------------------------------------------------------
//	Function<typename R(Af, Bf, Cf)>
//------------------------------------------------------------------------------
template<class R, class Af, class Bf, class Cf>
Function<typename R(typename Af, typename Bf, typename Cf)> func(typename R(*func)(Af, Bf, Cf))
{
	return
		Function<R(Af, Bf, Cf)>(
			SharedPtr<FuncDetail::FuncBase3<R, Af, Bf, Cf> >(
				new FuncDetail::Function0a3<R, Af, Bf, Cf>(func)));
}


template<class R, class A, class Af, class Bf, class Cf, class Df>
Function<typename R(typename Bf, typename Cf, typename Df)> func(typename R(*func)(typename Af, Bf, Cf, Df), const A& a)
{
	return
		Function<R(Bf, Cf, Df)>(
			SharedPtr<FuncDetail::FuncBase3<R, Bf, Cf, Df> >(
				new FuncDetail::Function1a3<R, A, Af, Bf, Cf, Df>(func, a)));
}


template<class R, class A, class B, class Af, class Bf, class Cf, class Df, class Ef>
Function<typename R(typename Cf, typename Df, typename Ef)> func(typename R(*func)(typename Af, typename Bf, Cf, Df, Ef), const A& a, const B& b)
{
	return
		Function<R(Cf, Df, Ef)>(
			SharedPtr<FuncDetail::FuncBase3<R, Cf, Df, Ef> >(
				new FuncDetail::Function2a3<R, A, B, Af, Bf, Cf, Df, Ef>(func, a, b)));
}


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df, class Ef, class Ff>
Function<typename R(typename Df, typename Ef, typename Ff)> func(typename R(*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff), const A& a, const B& b, const C& c)
{
	return
		Function<R(Df, Ef, Ff)>(
			SharedPtr<FuncDetail::FuncBase3<R, Df, Ef, Ff> >(
				new FuncDetail::Function3a3<R, A, B, C, Af, Bf, Cf, Df, Ef, Ff>(func, a, b, c)));
}


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf>
Function<typename R(typename Ef, typename Ff, typename Gf)> func(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf), const A& a, const B& b, const C& c, const D& d)
{
	return
		Function<R(Ef, Ff, Gf)>(
			SharedPtr<FuncDetail::FuncBase3<R, Ef, Ff, Gf> >(
				new FuncDetail::Function4a3<R, A, B, C, D, Af, Bf, Cf, Df, Ef, Ff, Gf>(func, a, b, c, d)));
}


//------------------------------------------------------------------------------
//	Function<typename R(Af, Bf, Cf, Df)>
//------------------------------------------------------------------------------
template<class R, class Af, class Bf, class Cf, class Df>
Function<typename R(typename Af, typename Bf, typename Cf, typename Df)> func(typename R(*func)(Af, Bf, Cf, Df))
{
	return
		Function<R(Af, Bf, Cf, Df)>(
			SharedPtr<FuncDetail::FuncBase4<R, Af, Bf, Cf, Df> >(
				new FuncDetail::Function0a4<R, Af, Bf, Cf, Df>(func)));
}


template<class R, class A, class Af, class Bf, class Cf, class Df, class Ef>
Function<typename R(typename Bf, typename Cf, typename Df, typename Ef)> func(typename R(*func)(typename Af, Bf, Cf, Df, Ef), const A& a)
{
	return
		Function<R(Bf, Cf, Df, Ef)>(
			SharedPtr<FuncDetail::FuncBase4<R, Bf, Cf, Df, Ef> >(
				new FuncDetail::Function1a4<R, A, Af, Bf, Cf, Df, Ef>(func, a)));
}


template<class R, class A, class B, class Af, class Bf, class Cf, class Df, class Ef, class Ff>
Function<typename R(typename Cf, typename Df, typename Ef, typename Ff)> func(typename R(*func)(typename Af, typename Bf, Cf, Df, Ef, Ff), const A& a, const B& b)
{
	return
		Function<R(Cf, Df, Ef, Ff)>(
			SharedPtr<FuncDetail::FuncBase4<R, Cf, Df, Ef, Ff> >(
				new FuncDetail::Function2a4<R, A, B, Af, Bf, Cf, Df, Ef, Ff>(func, a, b)));
}


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf>
Function<typename R(typename Df, typename Ef, typename Ff, typename Gf)> func(typename R(*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff, Gf), const A& a, const B& b, const C& c)
{
	return
		Function<R(Df, Ef, Ff, Gf)>(
			SharedPtr<FuncDetail::FuncBase4<R, Df, Ef, Ff, Gf> >(
				new FuncDetail::Function3a4<R, A, B, C, Af, Bf, Cf, Df, Ef, Ff, Gf>(func, a, b, c)));
}


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf, class Hf>
Function<typename R(typename Ef, typename Ff, typename Gf, typename Hf)> func(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf, Hf), const A& a, const B& b, const C& c, const D& d)
{
	return
		Function<R(Ef, Ff, Gf, Hf)>(
			SharedPtr<FuncDetail::FuncBase4<R, Ef, Ff, Gf, Hf> >(
				new FuncDetail::Function4a4<R, A, B, C, D, Af, Bf, Cf, Df, Ef, Ff, Gf, Hf>(func, a, b, c, d)));
}


//------------------------------------------------------------------------------
//	Function<typename R(Af, Bf, Cf, Df, Ef)>
//------------------------------------------------------------------------------
template<class R, class Af, class Bf, class Cf, class Df, class Ef>
Function<typename R(typename Af, typename Bf, typename Cf, typename Df, typename Ef)> func(typename R(*func)(Af, Bf, Cf, Df, Ef))
{
	return
		Function<R(Af, Bf, Cf, Df, Ef)>(
			SharedPtr<FuncDetail::FuncBase5<R, Af, Bf, Cf, Df, Ef> >(
				new FuncDetail::Function0a5<R, Af, Bf, Cf, Df, Ef>(func)));
}


template<class R, class A, class Af, class Bf, class Cf, class Df, class Ef, class Ff>
Function<typename R(typename Bf, typename Cf, typename Df, typename Ef, typename Ff)> func(typename R(*func)(typename Af, Bf, Cf, Df, Ef, Ff), const A& a)
{
	return
		Function<R(Bf, Cf, Df, Ef, Ff)>(
			SharedPtr<FuncDetail::FuncBase5<R, Bf, Cf, Df, Ef, Ff> >(
				new FuncDetail::Function1a5<R, A, Af, Bf, Cf, Df, Ef, Ff>(func, a)));
}


template<class R, class A, class B, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf>
Function<typename R(typename Cf, typename Df, typename Ef, typename Ff, typename Gf)> func(typename R(*func)(typename Af, typename Bf, Cf, Df, Ef, Ff, Gf), const A& a, const B& b)
{
	return
		Function<R(Cf, Df, Ef, Ff, Gf)>(
			SharedPtr<FuncDetail::FuncBase5<R, Cf, Df, Ef, Ff, Gf> >(
				new FuncDetail::Function2a5<R, A, B, Af, Bf, Cf, Df, Ef, Ff, Gf>(func, a, b)));
}


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf, class Hf>
Function<typename R(typename Df, typename Ef, typename Ff, typename Gf, typename Hf)> func(typename R(*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff, Gf, Hf), const A& a, const B& b, const C& c)
{
	return
		Function<R(Df, Ef, Ff, Gf, Hf)>(
			SharedPtr<FuncDetail::FuncBase5<R, Df, Ef, Ff, Gf, Hf> >(
				new FuncDetail::Function3a5<R, A, B, C, Af, Bf, Cf, Df, Ef, Ff, Gf, Hf>(func, a, b, c)));
}


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf, class Hf, class If>
Function<typename R(typename Ef, typename Ff, typename Gf, typename Hf, typename If)> func(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf, Hf, If), const A& a, const B& b, const C& c, const D& d)
{
	return
		Function<R(Ef, Ff, Gf, Hf, If)>(
			SharedPtr<FuncDetail::FuncBase5<R, Ef, Ff, Gf, Hf, If> >(
				new FuncDetail::Function4a5<R, A, B, C, D, Af, Bf, Cf, Df, Ef, Ff, Gf, Hf, If>(func, a, b, c, d)));
}


//------------------------------------------------------------------------------
//	Extra
//------------------------------------------------------------------------------
template<class R, class X, class Y>
Function<typename R()> func(X* ptr, typename R(Y::*func)())
{
	return
		Function<R()>(
			SharedPtr<FuncDetail::FuncBase0<R> >(
				new FuncDetail::Function0a0<R>(ptr, func)));
}


template<class R, class X, class Y, class A, class Af>
Function<typename R()> func(X* ptr, typename R(Y::*func)(typename Af), const A& a)
{
	return
		Function<R()>(
			SharedPtr<FuncDetail::FuncBase0<R> >(
				new FuncDetail::Function1a0<R, A, Af>(ptr, func, a)));
}

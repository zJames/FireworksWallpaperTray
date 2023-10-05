#pragma once
////////////////////////////////////////////////////////////////////////////////


namespace FuncDetail
{
//------------------------------------------------------------------------------
//	Function<typename R>
//------------------------------------------------------------------------------
template<class R>
class FuncBase0
{
public:
	FuncBase0() {}

	virtual typename R operator() () const = 0;

private:
	FuncBase0(FuncBase0&);
	void operator =(const FuncBase0&);
};


template<class R>
class Function0a0: public FuncBase0<typename R>
{
public:
	typedef fastdelegate::FastDelegate<typename R()> FuncType;

	Function0a0() {}

	Function0a0(const FuncType& func)
		:
		mFunc(func)
	{}

	Function0a0(typename R(*func)())
		:
		mFunc(func)
	{}

	template<class X, class Y>
	Function0a0(X* ptr, R(Y::*func)())
		:
		mFunc(ptr, func)
	{
	}

	virtual typename R operator() () const { return mFunc(); }

private:
	FuncType mFunc;
};


template<class R, class A, class Af>
class Function1a0: public FuncBase0<typename R>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af)> FuncType;

	Function1a0() {}

	Function1a0(const FuncType& func, const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	Function1a0(typename R(*func)(typename Af), const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	template<class X, class Y>
	Function1a0(X* ptr, R(Y::*func)(typename Af), const A& a)
		:
		mFunc(ptr, func),
		mVal0(a)
	{
	}

	virtual typename R operator() () const { return mFunc(mVal0); }

private:
	FuncType mFunc;
	A mVal0;
};


template<class R, class A, class B, class Af, class Bf>
class Function2a0: public FuncBase0<typename R>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf)> FuncType;

	Function2a0() {}

	Function2a0(const FuncType& func, const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	Function2a0(typename R(*func)(typename Af, typename Bf), const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	template<class X, class Y>
	Function2a0(X* ptr, R(Y::*func)(typename Af, typename Bf), const A& a, const B& b)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b)
	{
	}

	virtual typename R operator() () const { return mFunc(mVal0, mVal1); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
};


template<class R, class A, class B, class C, class Af, class Bf, class Cf>
class Function3a0: public FuncBase0<typename R>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf)> FuncType;

	Function3a0() {}

	Function3a0(const FuncType& func, const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	Function3a0(typename R(*func)(typename Af, typename Bf, typename Cf), const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	template<class X, class Y>
	Function3a0(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf), const A& a, const B& b, const C& c)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{
	}

	virtual typename R operator() () const { return mFunc(mVal0, mVal1, mVal2); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
};


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df>
class Function4a0: public FuncBase0<typename R>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, typename Df)> FuncType;

	Function4a0() {}

	Function4a0(const FuncType& func, const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	Function4a0(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	template<class X, class Y>
	Function4a0(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, typename Df), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{
	}

	virtual typename R operator() () const { return mFunc(mVal0, mVal1, mVal2, mVal3); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
	D mVal3;
};


//------------------------------------------------------------------------------
//	Function<typename R, Af>
//------------------------------------------------------------------------------
template<class R, class Af>
class FuncBase1
{
public:
	FuncBase1() {}

	virtual typename R operator() (Af) = 0;

private:
	FuncBase1(FuncBase1&);
	void operator =(const FuncBase1&);
};


template<class R, class Af>
class Function0a1: public FuncBase1<typename R, Af>
{
public:
	typedef fastdelegate::FastDelegate<typename R(Af)> FuncType;

	Function0a1() {}

	Function0a1(const FuncType& func)
		:
		mFunc(func)
	{}

	Function0a1(typename R(*func)(Af))
		:
		mFunc(func)
	{}

	template<class X, class Y>
	Function0a1(X* ptr, R(Y::*func)(Af))
		:
		mFunc(ptr, func)
	{
	}

	virtual typename R operator() (Af a) { return mFunc(a); }

private:
	FuncType mFunc;
};


template<class R, class A, class Af, class Bf>
class Function1a1: public FuncBase1<typename R, Bf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, Bf)> FuncType;

	Function1a1() {}

	Function1a1(const FuncType& func, const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	Function1a1(typename R(*func)(typename Af, Bf), const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	template<class X, class Y>
	Function1a1(X* ptr, R(Y::*func)(typename Af, Bf), const A& a)
		:
		mFunc(ptr, func),
		mVal0(a)
	{
	}

	virtual typename R operator() (Bf b) { return mFunc(mVal0, b); }

private:
	FuncType mFunc;
	A mVal0;
};


template<class R, class A, class B, class Af, class Bf, class Cf>
class Function2a1: public FuncBase1<typename R, Cf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, Cf)> FuncType;

	Function2a1() {}

	Function2a1(const FuncType& func, const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	Function2a1(typename R(*func)(typename Af, typename Bf, Cf), const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	template<class X, class Y>
	Function2a1(X* ptr, R(Y::*func)(typename Af, typename Bf, Cf), const A& a, const B& b)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b)
	{
	}

	virtual typename R operator() (Cf c) { return mFunc(mVal0, mVal1, c); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
};


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df>
class Function3a1: public FuncBase1<typename R, Df>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, Df)> FuncType;

	Function3a1() {}

	Function3a1(const FuncType& func, const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	Function3a1(typename R(*func)(typename Af, typename Bf, typename Cf, Df), const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	template<class X, class Y>
	Function3a1(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, Df), const A& a, const B& b, const C& c)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{
	}

	virtual typename R operator() (Df d) { return mFunc(mVal0, mVal1, mVal2, d); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
};


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef>
class Function4a1: public FuncBase1<typename R, Ef>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, typename Df, Ef)> FuncType;

	Function4a1() {}

	Function4a1(const FuncType& func, const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	Function4a1(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	template<class X, class Y>
	Function4a1(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, typename Df, Ef), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{
	}

	virtual typename R operator() (Ef e) { return mFunc(mVal0, mVal1, mVal2, mVal3, e); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
	D mVal3;
};


//------------------------------------------------------------------------------
//	Function<typename R, Af, Bf>
//------------------------------------------------------------------------------
template<class R, class Af, class Bf>
class FuncBase2
{
public:
	FuncBase2() {}

	virtual typename R operator() (Af, Bf) = 0;

private:
	FuncBase2(FuncBase2&);
	void operator =(const FuncBase2&);
};


template<class R, class Af, class Bf>
class Function0a2: public FuncBase2<typename R, Af, Bf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(Af, Bf)> FuncType;

	Function0a2() {}

	Function0a2(const FuncType& func)
		:
		mFunc(func)
	{}

	Function0a2(typename R(*func)(Af, Bf))
		:
		mFunc(func)
	{}

	template<class X, class Y>
	Function0a2(X* ptr, R(Y::*func)(Af, Bf))
		:
		mFunc(ptr, func)
	{
	}

	virtual typename R operator() (Af a, Bf b) { return mFunc(a, b); }

private:
	FuncType mFunc;
};


template<class R, class A, class Af, class Bf, class Cf>
class Function1a2: public FuncBase2<typename R, Af, Bf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, Bf, Cf)> FuncType;

	Function1a2() {}

	Function1a2(const FuncType& func, const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	Function1a2(typename R(*func)(typename Af, Bf, Cf), const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	template<class X, class Y>
	Function1a2(X* ptr, R(Y::*func)(typename Af, Bf, Cf), const A& a)
		:
		mFunc(ptr, func),
		mVal0(a)
	{
	}

	virtual typename R operator() (Bf b, Cf c) { return mFunc(mVal0, b, c); }

private:
	FuncType mFunc;
	A mVal0;
};


template<class R, class A, class B, class Af, class Bf, class Cf, class Df>
class Function2a2: public FuncBase2<typename R, Af, Bf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, Cf, Df)> FuncType;

	Function2a2() {}

	Function2a2(const FuncType& func, const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	Function2a2(typename R(*func)(typename Af, typename Bf, Cf, Df), const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	template<class X, class Y>
	Function2a2(X* ptr, R(Y::*func)(typename Af, typename Bf, Cf, Df), const A& a, const B& b)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b)
	{
	}

	virtual typename R operator() (Cf c, Df d) { return mFunc(mVal0, mVal1, c, d); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
};


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df, class Ef>
class Function3a2: public FuncBase2<typename R, Af, Bf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, Df, Ef)> FuncType;

	Function3a2() {}

	Function3a2(const FuncType& func, const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	Function3a2(typename R(*func)(typename Af, typename Bf, typename Cf, Df, Ef), const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	template<class X, class Y>
	Function3a2(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, Df, Ef), const A& a, const B& b, const C& c)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{
	}

	virtual typename R operator() (Df d, Ef e) { return mFunc(mVal0, mVal1, mVal2, d, e); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
};


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef, class Ff>
class Function4a2: public FuncBase2<typename R, Af, Bf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff)> FuncType;

	Function4a2() {}

	Function4a2(const FuncType& func, const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	Function4a2(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	template<class X, class Y>
	Function4a2(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{
	}

	virtual typename R operator() (Ef e, Ff f) { return mFunc(mVal0, mVal1, mVal2, mVal3, e, f); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
	D mVal3;
};


//------------------------------------------------------------------------------
//	Function<typename R, Af, Bf, Cf>
//------------------------------------------------------------------------------
template<class R, class Af, class Bf, class Cf>
class FuncBase3
{
public:
	FuncBase3() {}

	virtual typename R operator() (Af, Bf, Cf) = 0;

private:
	FuncBase3(FuncBase3&);
	void operator =(const FuncBase3&);
};


template<class R, class Af, class Bf, class Cf>
class Function0a3: public FuncBase3<typename R, Af, Bf, Cf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(Af, Bf, Cf)> FuncType;

	Function0a3() {}

	Function0a3(const FuncType& func)
		:
		mFunc(func)
	{}

	Function0a3(typename R(*func)(Af, Bf, Cf))
		:
		mFunc(func)
	{}

	template<class X, class Y>
	Function0a3(X* ptr, R(Y::*func)(Af, Bf, Cf))
		:
		mFunc(ptr, func)
	{
	}

	virtual typename R operator() (Af a, Bf b, Cf c) { return mFunc(a, b, c); }

private:
	FuncType mFunc;
};


template<class R, class A, class Af, class Bf, class Cf, class Df>
class Function1a3: public FuncBase3<typename R, Af, Bf, Cf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, Bf, Cf, Df)> FuncType;

	Function1a3() {}

	Function1a3(const FuncType& func, const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	Function1a3(typename R(*func)(typename Af, Bf, Cf, Df), const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	template<class X, class Y>
	Function1a3(X* ptr, R(Y::*func)(typename Af, Bf, Cf, Df), const A& a)
		:
		mFunc(ptr, func),
		mVal0(a)
	{
	}

	virtual typename R operator() (Bf b, Cf c, Df d) { return mFunc(mVal0, b, c, d); }

private:
	FuncType mFunc;
	A mVal0;
};


template<class R, class A, class B, class Af, class Bf, class Cf, class Df, class Ef>
class Function2a3: public FuncBase3<typename R, Af, Bf, Cf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, Cf, Df, Ef)> FuncType;

	Function2a3() {}

	Function2a3(const FuncType& func, const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	Function2a3(typename R(*func)(typename Af, typename Bf, Cf, Df, Ef), const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	template<class X, class Y>
	Function2a3(X* ptr, R(Y::*func)(typename Af, typename Bf, Cf, Df, Ef), const A& a, const B& b)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b)
	{
	}

	virtual typename R operator() (Cf c, Df d, Ef e) { return mFunc(mVal0, mVal1, c, d, e); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
};


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df, class Ef, class Ff>
class Function3a3: public FuncBase3<typename R, Af, Bf, Cf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, Df, Ef, Ff)> FuncType;

	Function3a3() {}

	Function3a3(const FuncType& func, const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	Function3a3(typename R(*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff), const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	template<class X, class Y>
	Function3a3(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff), const A& a, const B& b, const C& c)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{
	}

	virtual typename R operator() (Df d, Ef e, Ff f) { return mFunc(mVal0, mVal1, mVal2, d, e, f); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
};


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf>
class Function4a3: public FuncBase3<typename R, Af, Bf, Cf>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf)> FuncType;

	Function4a3() {}

	Function4a3(const FuncType& func, const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	Function4a3(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	template<class X, class Y>
	Function4a3(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{
	}

	virtual typename R operator() (Ef e, Ff f, Gf g) { return mFunc(mVal0, mVal1, mVal2, mVal3, e, f, g); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
	D mVal3;
};


//------------------------------------------------------------------------------
//	Function<typename R, Af, Bf, Cf, Df>
//------------------------------------------------------------------------------
template<class R, class Af, class Bf, class Cf, class Df>
class FuncBase4
{
public:
	FuncBase4() {}

	virtual typename R operator() (Af, Bf, Cf, Df) = 0;

private:
	FuncBase4(FuncBase4&);
	void operator =(const FuncBase4&);
};


template<class R, class Af, class Bf, class Cf, class Df>
class Function0a4: public FuncBase4<typename R, Af, Bf, Cf, Df>
{
public:
	typedef fastdelegate::FastDelegate<typename R(Af, Bf, Cf, Df)> FuncType;

	Function0a4() {}

	Function0a4(const FuncType& func)
		:
		mFunc(func)
	{}

	Function0a4(typename R(*func)(Af, Bf, Cf, Df))
		:
		mFunc(func)
	{}

	template<class X, class Y>
	Function0a4(X* ptr, R(Y::*func)(Af, Bf, Cf, Df))
		:
		mFunc(ptr, func)
	{
	}

	virtual typename R operator() (Af a, Bf b, Cf c, Df d) { return mFunc(a, b, c, d); }

private:
	FuncType mFunc;
};


template<class R, class A, class Af, class Bf, class Cf, class Df, class Ef>
class Function1a4: public FuncBase4<typename R, Af, Bf, Cf, Df>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, Bf, Cf, Df, Ef)> FuncType;

	Function1a4() {}

	Function1a4(const FuncType& func, const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	Function1a4(typename R(*func)(typename Af, Bf, Cf, Df, Ef), const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	template<class X, class Y>
	Function1a4(X* ptr, R(Y::*func)(typename Af, Bf, Cf, Df, Ef), const A& a)
		:
		mFunc(ptr, func),
		mVal0(a)
	{
	}

	virtual typename R operator() (Bf b, Cf c, Df d, Ef e) { return mFunc(mVal0, b, c, d, e); }

private:
	FuncType mFunc;
	A mVal0;
};


template<class R, class A, class B, class Af, class Bf, class Cf, class Df, class Ef, class Ff>
class Function2a4: public FuncBase4<typename R, Af, Bf, Cf, Df>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, Cf, Df, Ef, Ff)> FuncType;

	Function2a4() {}

	Function2a4(const FuncType& func, const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	Function2a4(typename R(*func)(typename Af, typename Bf, Cf, Df, Ef, Ff), const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	template<class X, class Y>
	Function2a4(X* ptr, R(Y::*func)(typename Af, typename Bf, Cf, Df, Ef, Ff), const A& a, const B& b)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b)
	{
	}

	virtual typename R operator() (Cf c, Df d, Ef e, Ff f) { return mFunc(mVal0, mVal1, c, d, e, f); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
};


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf>
class Function3a4: public FuncBase4<typename R, Af, Bf, Cf, Df>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, Df, Ef, Ff, Gf)> FuncType;

	Function3a4() {}

	Function3a4(const FuncType& func, const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	Function3a4(typename R(*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff, Gf), const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	template<class X, class Y>
	Function3a4(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff, Gf), const A& a, const B& b, const C& c)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{
	}

	virtual typename R operator() (Df d, Ef e, Ff f, Gf g) { return mFunc(mVal0, mVal1, mVal2, d, e, f, g); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
};


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf, class Hf>
class Function4a4: public FuncBase4<typename R, Af, Bf, Cf, Df>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf, Hf)> FuncType;

	Function4a4() {}

	Function4a4(const FuncType& func, const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	Function4a4(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf, Hf), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	template<class X, class Y>
	Function4a4(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf, Hf), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{
	}

	virtual typename R operator() (Ef e, Ff f, Gf g, Hf h) { return mFunc(mVal0, mVal1, mVal2, mVal3, e, f, g, h); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
	D mVal3;
};


//------------------------------------------------------------------------------
//	Function<typename R, Af, Bf, Cf, Df, Ef>
//------------------------------------------------------------------------------
template<class R, class Af, class Bf, class Cf, class Df, class Ef>
class FuncBase5
{
public:
	FuncBase5() {}

	virtual typename R operator() (Af, Bf, Cf, Df, Ef) = 0;

private:
	FuncBase5(FuncBase5&);
	void operator =(const FuncBase5&);
};


template<class R, class Af, class Bf, class Cf, class Df, class Ef>
class Function0a5: public FuncBase5<typename R, Af, Bf, Cf, Df, Ef>
{
public:
	typedef fastdelegate::FastDelegate<typename R(Af, Bf, Cf, Df, Ef)> FuncType;

	Function0a5() {}

	Function0a5(const FuncType& func)
		:
		mFunc(func)
	{}

	Function0a5(typename R(*func)(Af, Bf, Cf, Df, Ef))
		:
		mFunc(func)
	{}

	template<class X, class Y>
	Function0a5(X* ptr, R(Y::*func)(Af, Bf, Cf, Df, Ef))
		:
		mFunc(ptr, func)
	{
	}

	virtual typename R operator() (Af a, Bf b, Cf c, Df d, Ef e) { return mFunc(a, b, c, d, e); }

private:
	FuncType mFunc;
};


template<class R, class A, class Af, class Bf, class Cf, class Df, class Ef, class Ff>
class Function1a5: public FuncBase5<typename R, Af, Bf, Cf, Df, Ef>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, Bf, Cf, Df, Ef, Ff)> FuncType;

	Function1a5() {}

	Function1a5(const FuncType& func, const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	Function1a5(typename R(*func)(typename Af, Bf, Cf, Df, Ef, Ff), const A& a)
		:
		mFunc(func),
		mVal0(a)
	{}

	template<class X, class Y>
	Function1a5(X* ptr, R(Y::*func)(typename Af, Bf, Cf, Df, Ef, Ff), const A& a)
		:
		mFunc(ptr, func),
		mVal0(a)
	{
	}

	virtual typename R operator() (Bf b, Cf c, Df d, Ef e, Ff f) { return mFunc(mVal0, b, c, d, e, f); }

private:
	FuncType mFunc;
	A mVal0;
};


template<class R, class A, class B, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf>
class Function2a5: public FuncBase5<typename R, Af, Bf, Cf, Df, Ef>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, Cf, Df, Ef, Ff, Gf)> FuncType;

	Function2a5() {}

	Function2a5(const FuncType& func, const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	Function2a5(typename R(*func)(typename Af, typename Bf, Cf, Df, Ef, Ff, Gf), const A& a, const B& b)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b)
	{}

	template<class X, class Y>
	Function2a5(X* ptr, R(Y::*func)(typename Af, typename Bf, Cf, Df, Ef, Ff, Gf), const A& a, const B& b)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b)
	{
	}

	virtual typename R operator() (Cf c, Df d, Ef e, Ff f, Gf g) { return mFunc(mVal0, mVal1, c, d, e, f, g); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
};


template<class R, class A, class B, class C, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf, class Hf>
class Function3a5: public FuncBase5<typename R, Af, Bf, Cf, Df, Ef>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, Df, Ef, Ff, Gf, Hf)> FuncType;

	Function3a5() {}

	Function3a5(const FuncType& func, const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	Function3a5(typename R(*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff, Gf, Hf), const A& a, const B& b, const C& c)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{}

	template<class X, class Y>
	Function3a5(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, Df, Ef, Ff, Gf, Hf), const A& a, const B& b, const C& c)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c)
	{
	}

	virtual typename R operator() (Df d, Ef e, Ff f, Gf g, Hf h) { return mFunc(mVal0, mVal1, mVal2, d, e, f, g, h); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
};


template<class R, class A, class B, class C, class D, class Af, class Bf, class Cf, class Df, class Ef, class Ff, class Gf, class Hf, class If>
class Function4a5: public FuncBase5<typename R, Af, Bf, Cf, Df, Ef>
{
public:
	typedef fastdelegate::FastDelegate<typename R(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf, Hf, If)> FuncType;

	Function4a5() {}

	Function4a5(const FuncType& func, const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	Function4a5(typename R(*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf, Hf, If), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{}

	template<class X, class Y>
	Function4a5(X* ptr, R(Y::*func)(typename Af, typename Bf, typename Cf, typename Df, Ef, Ff, Gf, Hf, If), const A& a, const B& b, const C& c, const D& d)
		:
		mFunc(ptr, func),
		mVal0(a),
		mVal1(b),
		mVal2(c),
		mVal3(d)
	{
	}

	virtual typename R operator() (Ef e, Ff f, Gf g, Hf h, If i) { return mFunc(mVal0, mVal1, mVal2, mVal3, e, f, g, h, i); }

private:
	FuncType mFunc;
	A mVal0;
	B mVal1;
	C mVal2;
	D mVal3;
};

} //end namespace FuncDetail

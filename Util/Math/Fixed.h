#ifndef FIXED_H
#define FIXED_H

template<class T, int FRACTION_BITS>
class Fixed
{
public:
	typedef Fixed<T, FRACTION_BITS> This;
	static const int kFixedMul = 1 << FRACTION_BITS;

	Fixed() {}
	constexpr Fixed(const float n): mVal(T(n * kFixedMul)) {}
	constexpr Fixed(const double n): mVal(T(n * kFixedMul)) {}
	constexpr Fixed(const int n): mVal(T(n * kFixedMul)) {}

	template<class RV_T, int RV_FRACTION_BITS>
	explicit Fixed(const Fixed<RV_T, RV_FRACTION_BITS>& rv): mVal(
		FRACTION_BITS >= RV_FRACTION_BITS
			? rv.val() << (FRACTION_BITS - RV_FRACTION_BITS)
			: rv.val() >> (RV_FRACTION_BITS - FRACTION_BITS))
	{
	}

	This operator + (const This& rhs) const
	{
		return noConvert(mVal + rhs.mVal);
	}

	This operator - (const This& rhs) const
	{
		return noConvert(mVal - rhs.mVal);
	}
	This operator - () const
	{
		return noConvert(-mVal);
	}

	This operator * (const This& rhs) const
	{
		__int64 tmp = __int64(mVal) * rhs.mVal;

		return noConvert(T(tmp >> FRACTION_BITS));
	}

	This operator / (const This& rhs) const
	{
		if (rhs.mVal == 0)
			return 0;

		__int64 tmp = __int64(mVal) << FRACTION_BITS;
		return noConvert(T(tmp / rhs.mVal));
	}

	This operator * (const int rhs) const { return noConvert(mVal * rhs); }
	This operator / (const int rhs) const { return noConvert(mVal / rhs); }
	This operator >> (const int rhs) const { return noConvert(mVal >> rhs); }
	This operator << (const int rhs) const { return noConvert(mVal << rhs); }

	This& operator += (const This& rhs)
	{
		mVal += rhs.mVal;
		return *this;
	}

	This& operator -= (const This& rhs)
	{
		mVal -= rhs.mVal;
		return *this;
	}

	This& operator *= (const This& rhs)
	{
		*this = *this * rhs;
		
		return *this;
	}

	This& operator /= (const This& rhs)
	{
		*this = *this / rhs;

		return *this;
	}

	This& operator *= (const int rhs)
	{
		mVal *= rhs;
		return *this;
	}

	This& operator /= (const int rhs)
	{
		mVal /= rhs;
		return *this;
	}

	This operator * (const double rhs) const
	{
		return *this * This(rhs);
	}

	This operator / (const double rhs) const
	{
		return *this / This(rhs);
	}

	template<class RV>
	const This& operator *= (RV rhs)
	{
		return *this = *this * rhs;
	}

	template<class RV>
	const This& operator /= (RV rhs)
	{
		return *this = *this * rhs;
	}

	This& operator >>= (const int rhs) { mVal >>= rhs; }
	This& operator <<= (const int rhs) { mVal <<= rhs; }

	T val() const { return mVal; }
	T toInt() const { return mVal >> FRACTION_BITS; }
	T fraction() const { return mVal & ((1 << FRACTION_BITS) - 1); }
	float toFloat() const { return mVal / float(kFixedMul); }

	bool operator == (const This& rhs) const { return mVal == rhs.mVal; }
	bool operator != (const This& rhs) const { return mVal != rhs.mVal; }
	bool operator < (const This& rhs) const { return mVal < rhs.mVal; }
	bool operator > (const This& rhs) const { return mVal > rhs.mVal; }
	bool operator <= (const This& rhs) const { return mVal <= rhs.mVal; }
	bool operator >= (const This& rhs) const { return mVal >= rhs.mVal; }

	operator float () { return (float)mVal / (float)(1 << FRACTION_BITS); }
	operator int () { return toInt(); }
	
	This floor() const { return noConvert(mVal & ~((1 << FRACTION_BITS) - 1)); }
	This ceiling() const { return noConvert(mVal + ((1 << FRACTION_BITS) - 1)).floor(); }
	This truncate() const { return noConvert(mVal & ((1 << FRACTION_BITS) - 1)); }

	static This minVal() { return noConvert(1 << (sizeof(T) * 8 - 1)); }
	static This maxVal() { return noConvert(minVal().mVal - 1); }

	static This noConvert(T n)
	{
		This f;
		f.mVal = n;
		return f;
	}

	static int fractionBits() { return FRACTION_BITS; }
	static int integerBits() { return sizeof(T) * 8 - FRACTION_BITS; }

private:
	T mVal;
};


/*
	static const float invMul = 1.0f / 4096.0f;
	#define FX32toFx32(n) ((n) * invMul)
	#define Fx32toInt(n) (int(n))
	typedef float Fx32;
/*/
	#define FX32toFx32(n) (Fx32::noConvert(n))
	#define Fx32toInt(n) ((n).toInt())
	typedef Fixed<int, 12> Fx32;
	typedef Fixed<__int64, 12> Fx64;
/**/

typedef Fixed<short, 12> Fx16;

#endif
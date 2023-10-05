#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

template <class T>
struct Vector3
{
public:
	typedef Vector3<T> This;

	constexpr Vector3(): x(0), y(0), z(0) {}
	constexpr Vector3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {}

	This withX(T x) const
	{
		return This(x, this->y, this->z);
	}

	This withY(T y) const
	{
		return This(this->x, y, this->z);
	}

	This withZ(T z) const
	{
		return This(this->x, this->y, z);
	}

	This operator + (const This& rhs) const
	{
		return This(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	This operator - (const This& rhs) const
	{
		return This(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	This operator * (const This& rhs) const
	{
		return This(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	This operator / (const This& rhs) const
	{
		return This(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	This& operator += (const This& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	This& operator -= (const This& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	This& operator *= (const T rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;

		return *this;
	}

	This operator * (int n) const
	{
		return This(x * n, y * n, z * n);
	}

	This operator * (T n) const
	{
		return This(x * n, y * n, z * n);
	}

	This operator / (int n) const
	{
		return This(x / n, y / n, z / n);
	}

	This operator / (T n) const
	{
		return This(x / n, y / n, z / n);
	}

	This operator - () const
	{
		return This(-x, -y, -z);
	}

	bool operator == (const This& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	bool operator != (const This& rhs) const
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}

	This cross(const This& rhs) const
	{
		return This(
			(y * rhs.z) - (rhs.y * z),
			(z * rhs.x) - (rhs.z * x),
			(x * rhs.y) - (rhs.x * y));
	}

	T dot(const This& rhs) const
	{
		return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
	}

	T lenSquared() const
	{
		return (x * x) + (y * y) + (z * z);
	}

	T len() const
	{
		return T(sqrt(lenSquared()));
	}

	void clear()
	{
		x = y = z = 0;
	}

	This normalized() const
	{
		return *this / len();
	}

	const T& operator () (size_t n) const
	{
		return ((float*)this)[n];
	}

	T& operator () (size_t n)
	{
		return ((float*)this)[n];
	}

	const T& operator [] (size_t n) const
	{
		return ((float*)this)[n];
	}

	T& operator [] (size_t n)
	{
		return ((float*)this)[n];
	}

	T x;
	T y;
	T z;
};

template <class T>
struct Vector2
{
public:
	typedef Vector2<T> This;

	constexpr Vector2(): x(0), y(0) {}
	constexpr Vector2(T _x, T _y): x(_x), y(_y) {}

	This withX(T x) const
	{
		return This(x, this->y);
	}

	This withY(T y) const
	{
		return This(this->x, y);
	}

	This operator + (const This& rhs) const
	{
		return This(x + rhs.x, y + rhs.y);
	}

	This operator - (const This& rhs) const
	{
		return This(x - rhs.x, y - rhs.y);
	}

	template<class S>
	This operator * (const Vector2<S>& rhs) const
	{
		return This(x * rhs.x, y * rhs.y);
	}

	This operator * (const T rhs) const
	{
		return This(x * rhs, y * rhs);
	}

	This operator / (const This& rhs) const
	{
		return This(x / rhs.x, y / rhs.y);
	}

	This operator / (const T rhs) const
	{
		return This(x / rhs, y / rhs);
	}

	This operator - () const
	{
		return This(-x, -y);
	}

	This& operator += (const This& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	This& operator -= (const This& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	This& operator *= (const This& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;

		return *this;
	}

	This& operator *= (const T rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	This& operator /= (const This& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;

		return *this;
	}

	This& operator /= (const T rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator == (const This& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator != (const This& rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}
	
	T dot(const This& rhs) const
	{
		return (x * rhs.x) + (y * rhs.y);
	}

	T len() const
	{
		return T(sqrt((x * x) + (y * y)));
	}

	void clear()
	{
		x = y = 0;
	}

	This normalized() const
	{
		return *this / len();
	}

	const T& operator () (size_t n) const
	{
		return ((float*)this)[n];
	}

	T& operator () (size_t n)
	{
		return ((float*)this)[n];
	}

	const T& operator [] (size_t n) const
	{
		return ((float*)this)[n];
	}

	T& operator [] (size_t n)
	{
		return ((float*)this)[n];
	}

	template<class C>
	Vector2<C> convert() const
	{
		return Vector2<C>(C(x), C(y));
	}

	T x;
	T y;
};


typedef Vector3<float> Vector3f;
typedef Vector2<float> Vector2f;
typedef Vector3<int> Vector3i;
typedef Vector2<int> Vector2i;
typedef Vector3<double> Vector3d;
typedef Vector2<double> Vector2d;


template<class T>
Vector3<T> toVector3(const Vector2<T>& v, T z = T(0))
{
	return Vector3<T>(v.x, v.y, z);
}


template<class T>
Vector3<T> toVector3Y(const Vector2<T>& v, T y = T(0))
{
	return Vector3<T>(v.x, y, v.y);
}


template<class T>
Vector2<T> toVector2(const Vector3<T>& v)
{
	return Vector2<T>(v.x, v.y);
}


#endif
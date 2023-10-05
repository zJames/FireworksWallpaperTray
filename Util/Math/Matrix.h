//////////////////////////////////////////////////////////////////////////
// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
//////////////////////////////////////////////////////////////////////////


class Angle;


template<int SIZE, class T = float>
class Matrix
{
public:
	typedef Matrix<SIZE, typename T> This;

	Matrix() {}
	
	This operator * (const This& rhs) const
	{
		This m;

		m.clear();

		for (int i = 0; i < SIZE; ++i)
			for (int j = 0; j < SIZE; ++j)
				for (int k = 0; k < SIZE; ++k)
					m(i, j) += mData[i][k] * rhs(k, j);
		return m;
	}

	template<class RV>
	Vector3<RV> operator * (const Vector3<RV>& vtx) const
	{
		Vector3<RV> tVtx;

		for (int i = 0; i < 3 && i < SIZE; ++i)
			for (int j = 0; j < 3 && j < SIZE; ++j)
				tVtx[i] += RV(mData[j][i]) * vtx[j];

		if (SIZE == 4)
		{
			for (int i = 0; i < 3; ++i)
				tVtx[i] += RV(mData[3][i]);
		}

		return tVtx;
	}

	This operator * (const T& rhs) const
	{
		This m(*this);

		for (int j = 0; j < SIZE; ++j)
			for (int i = 0; i < SIZE; ++i)
				m.mData[j][i] *= rhs;

		return m;
	}

	This operator ~ ()
	{
		This m(*this);

		for (int j = 0; j < SIZE; ++j)
			for (int i = 0; i < SIZE; ++i)
				m(j, i) = mData[i][j];

		return m;
	}

	inline T& operator () (int row, int col)
	{
		return mData[row][col];
	}

	inline const T& operator() (int row, int col) const
	{
		return mData[row][col];
	}

	T* operator[] (int row) const
	{
		return (T*)(mData[row]);
	}

	T det() const
	{
		size_t	i,
				j,
				k;
		T		piv,
				detVal = T(1);

		This temp(*this);

		for (k = 0; k < SIZE; ++k)
		{
			int indx = temp.pivot(k);
			
			if (indx == -1)
				return 0;
			
			if (indx != 0)
				detVal = - detVal;
			
			detVal = detVal * temp[k][k];
			
			for (i = k + 1; i < SIZE; ++i)
			{
				piv = temp[i][k] / temp[k][k];
				
				for (j = k + 1; j < SIZE; ++j)
					temp[i][j] -= piv * temp[k][j];
			}
		}
		
		return detVal;
	}


	int pivot(int row) const
	{
		int k = row;
		double	amax,
				temp;

		amax = -1;
		
		for (int i = row; i < SIZE; ++i)
		{
			if ( (temp = abs( mData[i][row])) > amax && temp != 0.0)
			{
				amax = temp;
				k = i;
			}
		}
			
		if (mData[k][row] == T(0))
			return -1;

		if (k != row)
		{
			for (int i = 0; i < SIZE; ++i)
				swapT(mData[k][i], mData[row][i]);

			return k;
		}
			
		return 0;
	}

	void clear()
	{
		for (int j = 0; j < SIZE; ++j)
			for (int i = 0; i < SIZE; ++i)
				mData[j][i] = T(0);
	}

	static This identity()
	{
		This m;

		for (int j = 0; j < SIZE; ++j)
			for (int i = 0; i < SIZE; ++i)
				m(j, i) = T((j == i) ? 1 : 0);

		return m;
	}

	const T* data() const
	{
		return (T*)mData;
	}

private:
	T mData[SIZE][SIZE];
};


typedef Matrix<4, float> Mtx44;

Mtx44 rotatedX(float angle);
Mtx44 rotatedY(float angle);
Mtx44 rotatedZ(float angle);
Mtx44 rotatedX(Angle angle);
Mtx44 rotatedY(Angle angle);
Mtx44 rotatedZ(Angle angle);
Mtx44 scaled(const Vector3f& scale);
Mtx44 translated(const Vector3f& trans);

//////////////////////////////////////////////////////////////////////////
#endif
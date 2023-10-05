//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "MatrixStack.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	MatrixStack
//------------------------------------------------------------------------------
MatrixStack::MatrixStack()
{
	mMatrices.push_back(Mtx44::identity());
}

void MatrixStack::push(const Mtx44& mtx)
{
	mMatrices.push_back(mtx * mMatrices.back());
}

void MatrixStack::pushRevMul(const Mtx44& mtx)
{
	mMatrices.push_back(mMatrices.back() * mtx);
}

void MatrixStack::pop()
{
	if (mMatrices.size() > 1)
	{
		mMatrices.pop_back();
	}
}

void MatrixStack::pop(int n)
{
	for (int i = 0; i < n && mMatrices.size() > 1; ++i)
	{
		mMatrices.pop_back();
	}
}

void MatrixStack::set(const Mtx44& mtx)
{
	if (mMatrices.size() <= 1)
	{
		mMatrices.push_back(Mtx44::identity());
	}

	mMatrices.back() = mtx;
}

size_t MatrixStack::size() const
{
	return mMatrices.size();
}

const Mtx44& MatrixStack::top()
{
	return mMatrices.back();
}

Vector3f MatrixStack::operator *(const Vector3f& vec)
{
	return top() * vec;
}

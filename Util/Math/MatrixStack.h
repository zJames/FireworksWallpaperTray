//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Matrix.h"
#include "../Containers.h"

#include <vector>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	MatrixStack
//------------------------------------------------------------------------------
class MatrixStack
{
public:
	MatrixStack();

	void push(const Mtx44& mtx);
	void pushRevMul(const Mtx44& mtx);
	void pop();
	void pop(int n);
	void set(const Mtx44& mtx);

	size_t size() const;
	const Mtx44& top(); //should be const, need const_iterator in BlockArray

	Vector3f operator *(const Vector3f&); //should be const, need const_iterator in BlockArray

private:
	std::vector<Mtx44> mMatrices;
};

//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Matrix.h"

#include "Angle.h"
#include "Vector.h"

#include <math.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Matrix
//------------------------------------------------------------------------------
Mtx44 rotatedX(float angle)
{
	return rotatedX(Angle(angle));
}


Mtx44 rotatedX(Angle angle)
{
	Mtx44 mtx = Mtx44::identity();

	float s = float(angle.sin());
	float c = float(angle.cos());

	mtx[1][1] = c;
	mtx[1][2] = s;
	mtx[2][1] = -s;
	mtx[2][2] = c;

	return mtx;
}


Mtx44 rotatedY(Angle angle)
{
	Mtx44 mtx = Mtx44::identity();

	float s = float(angle.sin());
	float c = float(angle.cos());

	mtx[0][0] = c;
	mtx[0][2] = -s;
	mtx[2][0] = s;
	mtx[2][2] = c;

	return mtx;
}


Mtx44 rotatedY(float angle)
{
	return rotatedY(Angle(angle));
}


Mtx44 rotatedZ(Angle angle)
{
	Mtx44 mtx = Mtx44::identity();

	float s = float(angle.sin());
	float c = float(angle.cos());

	mtx[0][0] = c;
	mtx[0][1] = s;
	mtx[1][0] = -s;
	mtx[1][1] = c;

	return mtx;
}


Mtx44 rotatedZ(float angle)
{
	return rotatedZ(Angle(angle));
}


Mtx44 translated(const Vector3f& trans)
{
	Mtx44 m = Mtx44::identity();

	m(3, 0) = trans.x;
	m(3, 1) = trans.y;
	m(3, 2) = trans.z;

	return m;
}


Mtx44 scaled(const Vector3f& scale)
{
	Mtx44 m;

	m.clear();

	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
	m[3][3] = 1;

	return m;
}
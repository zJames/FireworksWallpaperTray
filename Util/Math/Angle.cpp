//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Angle.h"

#define _USE_MATH_DEFINES
#include <math.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
static double wrap(double rads)
{
	while (rads >= double(M_PI * 2.0f))
	{
		rads -= double(M_PI * 2.0f);
	}

	while (rads < double(0))
	{
		rads += double(M_PI * 2.0f);
	}

	return rads;
}


//------------------------------------------------------------------------------
//	Angle Implementation
//------------------------------------------------------------------------------
const double Angle::kPI = double(M_PI);


Angle::Angle(): val(0) {}


Angle::Angle(double rad): val(wrap(rad)) {}


Angle::Angle(double x, double y): val(double(atan2(x, y))) {}


Angle Angle::fromDegrees(double deg) { return Angle(deg * double(M_PI) / double(180)); }


bool Angle::operator ==(const Angle other) const { return fabs(val - other.val) < double(0.05); }
bool Angle::operator !=(const Angle other) const { return !(*this == other); }


Angle Angle::operator + (Angle other) const { return Angle(val + other.val); }
Angle Angle::operator - (Angle other) const { return Angle(val - other.val); }
Angle Angle::operator * (double other) const { return Angle(val * other); }
Angle Angle::operator / (double other) const { return Angle(val / other); }
Angle Angle::operator * (int other) const { return Angle(val * other); }
Angle Angle::operator / (int other) const { return Angle(val / other); }

Angle& Angle::operator += (Angle other) { *this = *this + other; return *this; }
Angle& Angle::operator -= (Angle other) { *this = *this - other; return *this; }
Angle& Angle::operator *= (double other) { *this = *this * other; return *this; }
Angle& Angle::operator /= (double other) { *this = *this / other; return *this; }
Angle& Angle::operator *= (int other) { *this = *this * other; return *this; }
Angle& Angle::operator /= (int other) { *this = *this / other; return *this; }

double Angle::cos() const { return double(::cos(val)); }
double Angle::sin() const { return double(::sin(val)); }
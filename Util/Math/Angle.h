//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Angle Class
//------------------------------------------------------------------------------
class Angle
{
public:
	static const double kPI;

	Angle();
	Angle(double);
	Angle(double x, double y);

	static Angle fromDegrees(double);

	bool operator ==(const Angle other) const;
	bool operator !=(const Angle other) const;

	Angle operator + (const Angle) const;
	Angle operator - (const Angle) const;
	Angle operator * (double) const;
	Angle operator / (double) const;
	Angle operator * (int) const;
	Angle operator / (int) const;

	Angle& operator += (Angle);
	Angle& operator -= (Angle);
	Angle& operator *= (double);
	Angle& operator /= (double);
	Angle& operator *= (int);
	Angle& operator /= (int);

	double cos() const;
	double sin() const;

	double val;
};
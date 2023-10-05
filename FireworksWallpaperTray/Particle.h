//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Particle
//------------------------------------------------------------------------------
struct Particle
{
	Particle();

	FastDelegate<void(Particle*, std::list<Particle>&)> update;
	
	Vector2<Fx32> position;
	Vector2<Fx32> velocity;
	Vector2<Fx32> acceleration;
	ARGB color;

	int varA;
	int lifeTime;
};

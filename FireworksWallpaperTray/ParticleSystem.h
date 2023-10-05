//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "Particle.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	ParticleSystem
//------------------------------------------------------------------------------
class ParticleSystem
{
public:
	typedef std::list<Particle> ParticleList;

	void draw(Bitmap& dest, const Bitmap& desktop) const;
	
	void add(Particle& particle);
	void update();

private:
	ParticleList mParticles;
};

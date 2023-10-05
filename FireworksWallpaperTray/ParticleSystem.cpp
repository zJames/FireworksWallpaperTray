//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "ParticleSystem.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
void setPixel(const Vector2i& pos, Bitmap& dest, const ARGB& color)
{
	if (pos.x < 0
		|| pos.y < 0
		|| pos.x >= dest.width()
		|| pos.y >= dest.height())
	{
		return;
	}

	ARGB* destPxl = (ARGB*)dest.pixelPtr(pos.x, pos.y);
	*destPxl = color;
}



//------------------------------------------------------------------------------
//	ParticleSystem
//------------------------------------------------------------------------------
void ParticleSystem::draw(Bitmap& dest, const Bitmap& desktop) const
{
	for (ParticleList::const_iterator i = mParticles.begin(); i != mParticles.end(); ++i)
	{
		Vector2i pos(i->position.x.toInt(), i->position.y.toInt());

		setPixel(pos, dest, i->color);
	}
}

void ParticleSystem::add(Particle& particle)
{
	mParticles.push_back(particle);
}

void ParticleSystem::update()
{
	ParticleList toAdd;

	for (ParticleList::iterator i = mParticles.begin(); i != mParticles.end();)
	{
		--(i->lifeTime);

		i->update(&(*i), toAdd);

		if (i->lifeTime == 0)
		{
			i = mParticles.erase(i);
		}
		else
		{
			++i;
		}
	}

	mParticles.splice(mParticles.begin(), toAdd);
}

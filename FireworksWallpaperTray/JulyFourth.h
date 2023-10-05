//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "ParticleSystem.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	JulyFourth
//------------------------------------------------------------------------------
class JulyFourth : public Process
{
public:
	JulyFourth(Bitmap& backBuffer);

protected:
	virtual void update() override;

private:
	Bitmap& mBackBuffer;
	Bitmap mFadedDesktop;
	ParticleSystem mParticles;
	int mTimer;
	DWORD mLastTime;
};

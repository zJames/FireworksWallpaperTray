//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "JulyFourth.h"

#include "Fade.h"
#include "Fireworks.h"
#include "RandMath.h"

#include <time.h>

#pragma comment(lib, "Winmm")
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Const Data
//------------------------------------------------------------------------------
const DWORD kTimeWait = 1000 / 20;


//------------------------------------------------------------------------------
//	JulyFourth
//------------------------------------------------------------------------------
JulyFourth::JulyFourth(Bitmap& backBuffer)
	:
	mBackBuffer(backBuffer),
	mTimer(0),
	mLastTime(0)
{
	mFadedDesktop.setSize(backBuffer.width(), backBuffer.height(), Bitmap::F_A8R8G8B8);
	backBuffer.drawAll(mFadedDesktop);
}

void JulyFourth::update()
{
	const DWORD newTime = timeGetTime();

	if ((newTime - mLastTime) < kTimeWait)
	{
		return;
	}

	mLastTime = newTime;
	mParticles.update();

	if (--mTimer < 0)
	{
		int numAdd = randRange(2, 8);

		do 
		{
			addSparkler(mParticles, Vector2i(mBackBuffer.width(), mBackBuffer.height()));
		} while (--numAdd > 0);

		mTimer = randRange(20, 100);
	}

	fadeToBlack(mBackBuffer);

	mParticles.draw(mBackBuffer, mFadedDesktop);
}

//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Fibre.h"

#include "../Pointers/Ptr.h"

#include <windows.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Fibre
//------------------------------------------------------------------------------
static LPVOID sPrimaryFibre = NULL;


Fibre::Fibre(int stackSize, FastDelegate<void()> entry)
	:
	mResult(0),
	mFinished(true),
	mFibre(NULL),
	mEntry(entry)
{
	if (sPrimaryFibre == NULL)
	{
		sPrimaryFibre = ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH);
		
		if (sPrimaryFibre ==  NULL)
			return;
	}

	reset(entry);
}


Fibre::~Fibre()
{
	if (mFibre != NULL)
		DeleteFiber(mFibre);
}


void Fibre::exec()
{
	if (mFinished)
		return;

	SwitchToFiber(mFibre);
}


void Fibre::reset(FastDelegate<void()> entry)
{
	if (mFibre != NULL)
		DeleteFiber(mFibre);

	mEntry = entry;
	mResult = 0;
	mFibre = CreateFiber(0, runCallback, this);

	mFinished = (mFibre != NULL);
}


void Fibre::terminate(DWORD result)
{
	mFinished = true;
	mResult = result;
}


void Fibre::yield()
{
	SwitchToFiber(sPrimaryFibre);
}


void Fibre::runCallback(LPVOID voidFibre)
{
	if (voidFibre == NULL)
		return;

	Ptr<Fibre> fibre = (Fibre*)voidFibre;

	if (!fibre->mEntry.empty())
		fibre->mEntry();

	fibre->mFinished = true;
}

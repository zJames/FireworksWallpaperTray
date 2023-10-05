//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "FastDelegate.h"

#include <wtypes.h>
////////////////////////////////////////////////////////////////////////////////


using namespace fastdelegate;


//------------------------------------------------------------------------------
//	Fibre
//------------------------------------------------------------------------------
class Fibre
{
public:
	Fibre(int stackSize, FastDelegate<void()> entry);
	~Fibre();

	void exec();
	void reset(FastDelegate<void()> entry);
	void terminate(DWORD result);

	DWORD result() const { return mResult; }
	bool finished() const { return mFinished; }

protected:
	static void yield();

private:
	static void CALLBACK runCallback(LPVOID);

	bool mFinished;
	DWORD mResult;
	FastDelegate<void()> mEntry;
	LPVOID mFibre;
};
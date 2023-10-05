//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include <wtypes.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Thread
//------------------------------------------------------------------------------
class Thread
{
public:
	Thread();
	virtual ~Thread();

	void start();
	void stop();
	void stopAndWait();
	void hasEnded();

	bool isRunning() const;

protected:
	virtual void update() = 0;

private:
	static DWORD WINAPI threadFunc(LPVOID pData);

	volatile bool mIsRunning;
	volatile bool mEnded;
	HANDLE mHandle;
	DWORD mThreadId;
};

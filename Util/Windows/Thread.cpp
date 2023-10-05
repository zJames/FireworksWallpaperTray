//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Thread.h"

#include "../Core/Debug.h"

#include <windows.h>
////////////////////////////////////////////////////////////////////////////////


struct ThreadInfo
{
	Thread* thread;
	volatile bool* ended;
};


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
DWORD WINAPI Thread::threadFunc(LPVOID pData)
{
	Thread* thread = (Thread*)pData;

	while (thread->isRunning())
	{
		thread->update();
	}

	thread->hasEnded();

	return 0;
}


//------------------------------------------------------------------------------
//	Thread
//------------------------------------------------------------------------------
Thread::Thread()
	:
	mIsRunning(false),
	mEnded(false)
{
}

Thread::~Thread()
{
	stopAndWait();
}

void Thread::start()
{
	if (mIsRunning)
		return;

	mIsRunning = true;
	mEnded = false;

	mHandle = CreateThread(
		NULL,
		0,
		&threadFunc,
		this,
		0,
		&mThreadId);

	if (mHandle == NULL)
	{
		LogDebugArgs(
			"Thread: Unable to create thread (%08X)\n",
			GetLastError());

		return;
	}
}

void Thread::stop()
{
	mIsRunning = false;
}

void Thread::stopAndWait()
{
	stop();

	while (!mEnded && mIsRunning) //while we're running and we have not ended
	{
		Sleep(10);
	}
}

void Thread::hasEnded()
{
	mEnded = true;
}

bool Thread::isRunning() const
{
	return mIsRunning;
}
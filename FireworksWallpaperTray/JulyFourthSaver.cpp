//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "JulyFourthSaver.h"

#include "EnumMonitors.h"
#include "JulyFourth.h"
#include "Resource.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Local Functions
//------------------------------------------------------------------------------
static HWND getHwndFromCommand(const TCHAR* pstrCommandLine)
{
	HWND hWndParent = NULL;
	++pstrCommandLine;

	while (*pstrCommandLine && !isdigit(*pstrCommandLine))
	{
		++pstrCommandLine;
	}

	if (isdigit(*pstrCommandLine))
	{
#ifdef _WIN64
		hWndParent = HWND(_ttoll(pstrCommandLine));
#else
		hWndParent = HWND(_ttol(pstrCommandLine));
#endif
	}

	return hWndParent;
}


static DWORD windowStyle(HWND hParent)
{
	return hParent == NULL ? WS_POPUP : WS_CHILD | WS_VISIBLE;
}


//------------------------------------------------------------------------------
//	JulyFourthSaver
//------------------------------------------------------------------------------
JulyFourthSaver::JulyFourthSaver(
	HINSTANCE hInstance,
	HWND hParent,
	const MonitorInfo* monitorInfo /*= NULL*/)
	:
	mAlive(true)
{
	const bool createSuccess =
		create(
			_T("July Fourth Saver"),
			_T("JulyFourthSaver"),
			hInstance,
			hParent,
			windowStyle(hParent),
			1,
			1,
			SW_SHOW);

	mBackBuffer.setSize(960, 540, Bitmap::F_A8R8G8B8);

	if (!createSuccess)
	{
		LogDebug("ProgressSaver: Window creation failed.\n");
		return;
	}

	int width,
		height,
		xPos = 0,
		yPos = 0;

	if (hParent == NULL)
	{
		BOOL bUnused;
		SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, &bUnused, 0);

		ShowCursor(FALSE);

		xPos = monitorInfo->rcMonitor.left;
		yPos = monitorInfo->rcMonitor.top;
		width = monitorInfo->rcMonitor.width();
		height = monitorInfo->rcMonitor.height();

		HDC hdc = GetDC(NULL);
		
		StretchBlt(
			mBackBuffer,
			0,
			0,
			mBackBuffer.width(),
			mBackBuffer.height(),
			hdc,
			xPos,
			yPos,
			width,
			height,
			SRCCOPY);

		ReleaseDC(NULL, hdc);
	}
	else
	{
		const UINT dpi = GetDpiForWindow(hParent);
		Rect rcParent;
		
		GetClientRect(hParent, &rcParent);

		width = (rcParent.width() * dpi) / 96;
		height = (rcParent.height() * dpi) / 96;
	}

	SetWindowPos(
		hWnd(),
		HWND_TOP,
		xPos,
		yPos,
		width,
		height,
		0);

	mProcessList.add(new JulyFourth(mBackBuffer));
}

void JulyFourthSaver::setRunTimer()
{
	SetTimer(hWnd(), 0, 100, NULL);
}

bool JulyFourthSaver::alive() const
{
	return (hWnd() != NULL) && mAlive;
}

bool JulyFourthSaver::localMulti() const
{
#ifdef _DEBUG
	return false;
#else
	return true;
#endif
}

void JulyFourthSaver::run()
{
	if (!alive())
	{
		return;
	}

	mProcessList.exec();

	renderWindow();
}

void JulyFourthSaver::quit()
{
	PostQuitMessage(0);
	mAlive = false;
}

Maybe<LRESULT> JulyFourthSaver::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	case WM_QUIT:
		quit();
		break;

	case WM_TIMER:
		run();
		break;
	}

	return nothing;
}

void JulyFourthSaver::renderWindow()
{
	if (hWnd() == NULL)
	{
		return;
	}

	HDC hdc = GetDC(hWnd());

	Rect rcClient;

	GetClientRect(hWnd(), &rcClient);

	StretchBlt(
		hdc,
		0,
		0,
		rcClient.width(),
		rcClient.height(),
		mBackBuffer,
		0,
		0,
		mBackBuffer.width(),
		mBackBuffer.height(),
		SRCCOPY);

	ReleaseDC(hWnd(), hdc);
}

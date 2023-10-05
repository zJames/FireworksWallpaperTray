//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "EmptyWindow.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	EmptyWindow Implementation
//------------------------------------------------------------------------------
EmptyWindow::EmptyWindow(HINSTANCE hInst, HWND hParent, const Rect& pos)
	:
	mRect(pos)
{
	create("empty", "empty", hInst, hParent, 1, 1, SW_SHOW, WS_CHILD|WS_POPUP);

	MoveWindow(hWnd(), pos.left, pos.top, pos.width(), pos.height(), TRUE);
}


Maybe<LRESULT> EmptyWindow::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		{
			HDC hdc = GetDC(hWnd());

			BitBlt(
				hdc,
				0,
				0,
				mRect.width(),
				mRect.height(),
				NULL,
				0,
				0,
				BLACKNESS);

			ReleaseDC(hWnd(), hdc);
		}
		break;
	}

	return Maybe<LRESULT>(TRUE);
}
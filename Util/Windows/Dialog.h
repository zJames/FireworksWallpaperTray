//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "BaseWindow.h"
//////////////////////////////////////////////////////////////////////////


class Dialog: public BaseWindow
{
public:
	Dialog();
	virtual ~Dialog();

	INT_PTR create(HWND hWndParent, HINSTANCE hInstance, TCHAR* lpTemplate, bool stayOpen = false);

	HWND hWnd() const;
	HINSTANCE hInstance() const;
	
	void destroy();

protected:
	virtual INT_PTR onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:
	static INT_PTR CALLBACK dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

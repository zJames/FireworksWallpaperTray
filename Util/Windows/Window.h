//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "BaseWindow.h"

#include "..\Containers\Maybe.h"
////////////////////////////////////////////////////////////////////////////////


class Window : public BaseWindow
{
public:
	Window();

	virtual ~Window();

	bool create(
		const TCHAR* szTitle,
		const TCHAR* szWindowClass,
		HINSTANCE hInstance,
		HWND hParent,
		DWORD dwStyle,
		int width,
		int height,
		int nCmdShow,
		HICON hIcon = (HICON)0,
		LPCSTR lpszMenuName = NULL);

	void setIdeal(int width, int height);
	void destroy();

protected:
	virtual Maybe<LRESULT> onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:
	static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

	void registerClass(HINSTANCE hInstance,
						const TCHAR* szWindowClass,
						HICON hIcon,
						LPCSTR lpszMenuName);
};

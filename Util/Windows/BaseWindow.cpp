//------------------------------------------------------------------------------
//  Copyright : (c) 2015
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "BaseWindow.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	BaseWindow
//------------------------------------------------------------------------------
BaseWindow::BaseWindow()
	:
	m_hWnd(NULL),
	m_hParent(NULL),
	m_hMenu(NULL),
	m_hInstance(NULL)
{
}


BaseWindow::~BaseWindow()
{
	destroyMenu();
}


void BaseWindow::setTitle(const TCHAR* szTitle)
{
	SetWindowText(m_hWnd, szTitle);
}

void BaseWindow::destroyMenu()
{
	if (m_hMenu != NULL)
	{
		DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}
}


HWND BaseWindow::hWnd() const
{
	return m_hWnd;
}


HWND BaseWindow::hParent() const
{
	return m_hParent;
}


HMENU BaseWindow::hMenu() const
{
	return m_hMenu;
}


HINSTANCE BaseWindow::hInstance() const
{
	return m_hInstance;
}


Rect BaseWindow::clientRect() const
{
	RECT rcClient;

	GetClientRect(hWnd(), &rcClient);

	return rcClient;
}


Rect BaseWindow::windowRect() const
{
	RECT rcWindow;

	GetWindowRect(hWnd(), &rcWindow);

	return rcWindow;
}

//------------------------------------------------------------------------------
//  Copyright : (c) 2006
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Dialog.h"
////////////////////////////////////////////////////////////////////////////////


// Disables broken type size check for SetWindowLongPtr and GetWindowLongPtr
#pragma warning(disable:4311)
#pragma warning(disable:4312)


//------------------------------------------------------------------------------
//	Constructors & Destructor
//------------------------------------------------------------------------------
Dialog::Dialog() {}

Dialog::~Dialog()
{
	if (m_hWnd != NULL)
	{
		EndDialog(m_hWnd, 0);
	}
}


//------------------------------------------------------------------------------
//	Public Member Functions
//------------------------------------------------------------------------------
INT_PTR Dialog::create(HWND hWndParent, HINSTANCE hInstance, TCHAR* lpTemplate, bool stayOpen)
{
	m_hInstance = hInstance;
	m_hParent = hWndParent;

	if (stayOpen)
	{
		const HWND hWnd =
			CreateDialogParam(
				hInstance,
				lpTemplate,
				hWndParent,
				dlgProc,
				(LPARAM)this);

		return hWnd ? FALSE : TRUE;
	}

	return DialogBoxParam(hInstance, lpTemplate, hWndParent, dlgProc, (LPARAM)this);
}

HWND Dialog::hWnd() const
{
	return m_hWnd;
}

HINSTANCE Dialog::hInstance() const
{
	return m_hInstance;
}

void Dialog::destroy()
{
	if (m_hWnd == NULL)
	{
		return;
	}

	EndDialog(m_hWnd, 0);

	m_hWnd = NULL;
}

INT_PTR Dialog::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}


//------------------------------------------------------------------------------
//	Private Member Functions
//------------------------------------------------------------------------------
INT_PTR CALLBACK Dialog::dlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT_PTR ret = FALSE;

	if (message == WM_INITDIALOG)
	{
		SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)lParam);
		((Dialog*)lParam)->m_hWnd = hDlg;
		ret = (INT_PTR)TRUE;
	}

	Dialog* pDialog = reinterpret_cast<Dialog*>(GetWindowLongPtr(hDlg, DWLP_USER));

	if (pDialog == NULL)
	{
		return ret;
	}

	if (message == WM_DESTROY)
	{
		pDialog->destroy();
	}

	return pDialog->onMessage(message, wParam, lParam) | ret;
}

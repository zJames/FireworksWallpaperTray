//------------------------------------------------------------------------------
//  Copyright : (c) 2006
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "Window.h"
//////////////////////////////////////////////////////////////////////////


// Disables broken type size check for SetWindowLongPtr and GetWindowLongPtr
#pragma warning(disable:4311)
#pragma warning(disable:4312)


//------------------------------------------------------------------------------
//	Constructors & Destructor
//------------------------------------------------------------------------------
Window::Window()
{
}

Window::~Window()
{
	destroyMenu();

	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
	}
}


//------------------------------------------------------------------------------
//	Public Member Functions
//------------------------------------------------------------------------------
bool Window::create(
	const TCHAR* szTitle,
	const TCHAR* szWindowClass,
	HINSTANCE hInstance,
	HWND hParent,
	DWORD dwStyle,
	int width,
	int height,
	int nCmdShow,
	HICON hIcon,
	LPCSTR lpszMenuName)
{
	registerClass(hInstance, szWindowClass, hIcon, lpszMenuName);

	if (lpszMenuName)
	{
		m_hMenu = LoadMenu(hInstance, lpszMenuName);
	}

	m_hWnd =
		CreateWindow(
			szWindowClass,
			szTitle,
			dwStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			width,
			height,
			hParent,
			m_hMenu,
			hInstance,
			(LPVOID)this);

	m_hInstance = hInstance;
	m_hParent = hParent;

	if (!m_hWnd)
	{
		return false;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return true;
}

void Window::setIdeal(int width, int height)
{
	Rect window = windowRect();
	Rect client = clientRect();

	MoveWindow(
		hWnd(),
		window.left,
		window.top,
		width + (window.width() - client.width()),
		height + (window.height() - client.height()),
		TRUE);
}


void Window::destroy()
{
	if (m_hWnd == NULL)
	{
		return;
	}

	destroyMenu();

#ifdef MOBILE_APP
	sWindowMap[m_hWnd] = NULL;
#else
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, LONG(0));
#endif

	m_hWnd = NULL;
}


//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------
Maybe<LRESULT> Window::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return nothing;
}


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------
LRESULT CALLBACK Window::wndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	Maybe<LRESULT> ret;

	switch (message)
	{
	case WM_CREATE:
		{
			SetWindowLongPtr(
				hWnd,
				GWLP_USERDATA,
				(LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
		}
		break;

	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		break;

	case WM_QUIT:
	case WM_DESTROY:
		{
			PostQuitMessage(0);

			Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

			if (pWindow)
			{
				pWindow->onMessage(message, wParam, lParam);
				pWindow->m_hWnd = NULL;
			}
		}
		break;
	}

	Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (pWindow != NULL)
	{
		ret = pWindow->onMessage(message, wParam, lParam);
	}

	return ret.get() ? *ret : DefWindowProc(hWnd, message, wParam, lParam);
}


void Window::registerClass(
	HINSTANCE hInstance,
	const TCHAR* szWindowClass,
	HICON hIcon,
	LPCSTR lpszMenuName)
{
	WNDCLASSEX wcex;

	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 4;
	wcex.hInstance = hInstance;
	wcex.hIcon = hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = lpszMenuName;
	wcex.lpszClassName = szWindowClass;

	RegisterClassEx(&wcex);
}

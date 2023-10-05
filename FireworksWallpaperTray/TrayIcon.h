//------------------------------------------------------------------------------
//  Copyright : (c) 2022
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//	TrayIcon
//------------------------------------------------------------------------------
class TrayIcon
{
public:
	TrayIcon();

	void initData(HINSTANCE hInstance, HWND hWnd, UINT icon, UINT callbackMessage, RefString tooltip);

	void addIcon();
	void removeIcon();

private:
	NOTIFYICONDATA mNotifyIconData;
};


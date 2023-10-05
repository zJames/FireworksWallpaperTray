//------------------------------------------------------------------------------
//  Copyright : (c) 2023
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "TrayIcon.h"
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// TrayIcon
//------------------------------------------------------------------------------
TrayIcon::TrayIcon()
{
	memset(&mNotifyIconData, 0, sizeof(NOTIFYICONDATA));
	mNotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	mNotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE;
}

void TrayIcon::initData(
	HINSTANCE hInstance,
	HWND hWnd,
	UINT icon,
	UINT callbackMessage,
	RefString tooltip)
{
	mNotifyIconData.hWnd = hWnd;
	mNotifyIconData.uID = icon;
	mNotifyIconData.uCallbackMessage = callbackMessage;
	mNotifyIconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(icon));

	if (!tooltip.empty())
	{
		const size_t maxChars = ARRAY_LENGTH(mNotifyIconData.szTip);

		memcpy(
			mNotifyIconData.szTip,
			tooltip.const_ptr(),
			min(maxChars, tooltip.dataSize()));

		mNotifyIconData.szTip[maxChars - 1] = 0;
	}
}

void TrayIcon::addIcon()
{
	Shell_NotifyIcon(NIM_ADD, &mNotifyIconData);
}

void TrayIcon::removeIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &mNotifyIconData);
}

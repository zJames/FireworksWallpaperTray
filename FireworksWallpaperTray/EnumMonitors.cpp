//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "EnumMonitors.h"

#include <shellscalingapi.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Enum Monitors Implementation
//------------------------------------------------------------------------------
BOOL CALLBACK monitorEnumProc(
	HMONITOR hMonitor,	// handle to display monitor
	HDC hdcMonitor,		// handle to monitor DC
	LPRECT lprcMonitor,	// monitor intersection rectangle
	LPARAM dwData)		// data
{
	if (dwData == NULL)
	{
		return FALSE;
	}

	MonitorInfo newInfo;

	newInfo.hMonitor = hMonitor;
	newInfo.hdcMonitor = hdcMonitor;

	MONITORINFOEX monInfo = { 0 };

	monInfo.cbSize = sizeof(monInfo);

	GetMonitorInfo(hMonitor, &monInfo);

	newInfo.dwFlags = monInfo.dwFlags;
	newInfo.rcMonitor = monInfo.rcMonitor;
	newInfo.rcWork = monInfo.rcWork;

	DEVMODE devmode = {0};

	devmode.dmSize = sizeof(DEVMODE);

	EnumDisplaySettings(monInfo.szDevice, ENUM_CURRENT_SETTINGS, &devmode);

	//Adjust rect for real pixels
	newInfo.rcMonitor.right = newInfo.rcMonitor.left + devmode.dmPelsWidth;
	newInfo.rcMonitor.bottom = newInfo.rcMonitor.top + devmode.dmPelsHeight;

	reinterpret_cast<std::list<MonitorInfo>*>(dwData)->push_back(newInfo);

	return TRUE;
}


void enumerateMonitors(std::list<MonitorInfo>* monitorInfo)
{
	EnumDisplayMonitors(NULL, NULL, monitorEnumProc, (LPARAM)monitorInfo);
}
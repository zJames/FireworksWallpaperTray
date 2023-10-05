//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "EnumMonitors.h"
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

	MONITORINFO monInfo = { 0 };

	monInfo.cbSize = sizeof(monInfo);

	GetMonitorInfo(hMonitor, &monInfo);

	newInfo.dwFlags = monInfo.dwFlags;
	newInfo.rcMonitor = monInfo.rcMonitor;
	newInfo.rcWork = monInfo.rcWork;

	reinterpret_cast<std::list<MonitorInfo>*>(dwData)->push_back(newInfo);

	return TRUE;
}


void enumerateMonitors(std::list<MonitorInfo>* monitorInfo)
{
	EnumDisplayMonitors(NULL, NULL, monitorEnumProc, (LPARAM)monitorInfo);
}
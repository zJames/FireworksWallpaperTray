//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	Information About Each Monitor
//------------------------------------------------------------------------------
struct MonitorInfo
{
	HMONITOR	hMonitor;		// handle to display monitor
	HDC			hdcMonitor;		// handle to monitor DC
	Rect		rcMonitor;		// monitor rectangle
	Rect		rcWork;
	DWORD		dwFlags;
};


//------------------------------------------------------------------------------
//	Enumerates the monitors and stores their info
//------------------------------------------------------------------------------
void enumerateMonitors(std::list<MonitorInfo>* monitorInfo);

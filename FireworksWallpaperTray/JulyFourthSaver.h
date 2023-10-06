//------------------------------------------------------------------------------
//  Copyright : (c) 2020
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "AppInterface.h"
////////////////////////////////////////////////////////////////////////////////


struct MonitorInfo;


//------------------------------------------------------------------------------
//	JulyFourthSaver
//------------------------------------------------------------------------------
class JulyFourthSaver : public AppInterface
{
public:
	JulyFourthSaver(
		HINSTANCE hInstance,
		HWND hParent,
		const MonitorInfo* monitorInfo = NULL);
	~JulyFourthSaver();

	void setRunTimer();

	virtual bool alive() const override;
	virtual bool localMulti() const override;

	virtual void run() override;
	virtual void quit() override;

protected:
	virtual Maybe<LRESULT> onMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	void renderWindow();

	bool mAlive;
	Bitmap mBackBuffer;
	ProcessList mProcessList;
};
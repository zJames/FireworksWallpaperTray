//------------------------------------------------------------------------------
//  Copyright : (c) 2019
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	AppInterface
//------------------------------------------------------------------------------
class AppInterface : public Window
{
public:
	virtual bool alive() const = 0;
	virtual bool localMulti() const = 0;

	virtual void run() = 0;
	virtual void quit() = 0;
};

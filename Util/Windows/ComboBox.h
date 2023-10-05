//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "../Text/RefString.h"
#include "WindowsInc.h"
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	ComboBox
//------------------------------------------------------------------------------
class ComboBox
{
public:
	ComboBox();
	ComboBox(HWND hParent, int id);

	void set(HWND hParent, int id);

	void addItem(const TCHAR*);
	RefString getItem(int index) const;
	void select(int);
	void remove(int);
	void clear();
	
	int size() const;
	int curSelection() const;

private:
	HWND mControl;
};
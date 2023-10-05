//------------------------------------------------------------------------------
//  Copyright : (c) 2009
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "ComboBox.h"

#include <WindowsX.h>
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	ComboBox
//------------------------------------------------------------------------------
ComboBox::ComboBox()
{
}


ComboBox::ComboBox(HWND hParent, int id)
{
	set(hParent, id);
}


void ComboBox::set(HWND hParent, int id)
{
	mControl = GetDlgItem(hParent, id);
}


void ComboBox::addItem(const TCHAR* szString)
{
	ComboBox_AddString(mControl, szString);
}

RefString ComboBox::getItem(int index) const
{
	if (size() >= index)
	{
		return "";
	}

	RefString text(size_t(ComboBox_GetLBTextLen(mControl, index) + 1));
	
	ComboBox_GetLBText(mControl, index, text.ptr());

	return text;
}


void ComboBox::select(int index)
{
	ComboBox_SetCurSel(mControl, index);
}


void ComboBox::remove(int index)
{
	ComboBox_DeleteString(mControl, index);
}


void ComboBox::clear()
{
	ComboBox_ResetContent(mControl);
}


int ComboBox::size() const
{
	return ComboBox_GetCount(mControl);
}


int ComboBox::curSelection() const
{
	return ComboBox_GetCurSel(mControl);
}

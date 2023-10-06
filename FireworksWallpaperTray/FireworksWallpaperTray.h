//------------------------------------------------------------------------------
//  Copyright : (c) 2023
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once

#include "TrayIcon.h"
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// TrayWindow
//------------------------------------------------------------------------------
class TrayWindow : public Window
{
public:
    TrayWindow(HINSTANCE hInstance);

protected:
    virtual Maybe<LRESULT> onMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

    TrayIcon mTrayIcon;
};


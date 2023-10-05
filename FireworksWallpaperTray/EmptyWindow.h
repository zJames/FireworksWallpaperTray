//------------------------------------------------------------------------------
//  Copyright : (c) 2008
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#pragma once
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
//	A blank (black) window that fills a rect
//------------------------------------------------------------------------------
class EmptyWindow: public Window
{
public:
	EmptyWindow(HINSTANCE hInst, HWND hParent, const Rect& pos);

protected:
	Maybe<LRESULT> onMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	Rect mRect;
};

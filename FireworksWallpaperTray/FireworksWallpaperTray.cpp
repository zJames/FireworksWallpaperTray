//------------------------------------------------------------------------------
//  Copyright : (c) 2023
//  Authors :
//	  JRP	James Prettyman
//------------------------------------------------------------------------------

#include "FireworksWallpaperTray.h"

#include "framework.h"
#include "JulyFourthSaver.h"
#include "TrayIcon.h"

#include <shellapi.h>
////////////////////////////////////////////////////////////////////////////////


class TrayWindow : public Window
{
public:
    TrayWindow(HINSTANCE hInstance);

protected:
    virtual Maybe<LRESULT> onMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

    TrayIcon mTrayIcon;
};


//------------------------------------------------------------------------------
//	Global Variables
//------------------------------------------------------------------------------
std::vector<SharedImage> sImageFiles;
std::vector<RefString> sTexts;


//------------------------------------------------------------------------------
//	Local Const Data
//------------------------------------------------------------------------------
const RefString kProjectName = "FirworksWallpaperTray";
const RefString kHookTarget = "Progman";
const RefString kWorkerWindow = "WorkerW";
const RefString kShellWindow = "SHELLDLL_DefView";
const UINT16 kSecretMessage = 0x052C;
const UINT16 kTrayCallback = WM_USER + 1;


//------------------------------------------------------------------------------
//	TrayWindow
//------------------------------------------------------------------------------
TrayWindow::TrayWindow(HINSTANCE hInstance)
{
    create(
        kProjectName,
        kProjectName,
        hInstance,
        NULL,
        WS_OVERLAPPEDWINDOW,
        1,
        1,
        SW_HIDE);

    m_hMenu = CreatePopupMenu();

    AppendMenu(m_hMenu, MF_STRING, IDM_EXIT, TEXT("Exit"));

    mTrayIcon.initData(hInstance, hWnd(), IDI_ICON1, kTrayCallback, kProjectName);
    mTrayIcon.addIcon();
}

Maybe<LRESULT> TrayWindow::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case kTrayCallback:
    {
        switch (wParam)
        {
        case IDI_ICON1:
            break;
        }

        // the mouse button has been released.

        // I'd LIKE TO do this on WM_LBUTTONDOWN, it makes
        // for a more responsive-feeling app but actually
        // the guy who made the original post is right.
        // Most apps DO respond to WM_LBUTTONUP, so if you
        // restore your window on WM_LBUTTONDOWN, then some
        // other icon will scroll in under your mouse so when
        // the user releases the mouse, THAT OTHER ICON will
        // get the WM_LBUTTONUP command and that's quite annoying.
        if (lParam == WM_LBUTTONUP)
        {
        }
        else if (lParam == WM_RBUTTONDOWN) // I'm using WM_RBUTTONDOWN here because
        {
            // Get current mouse position.
            POINT curPoint;
            GetCursorPos(&curPoint);

            // should SetForegroundWindow according
            // to original poster so the popup shows on top
            SetForegroundWindow(hWnd());

            // TrackPopupMenu blocks the app until TrackPopupMenu returns
            UINT clicked = TrackPopupMenu(
                hMenu(),
                TPM_RETURNCMD | TPM_NONOTIFY, // don't send me WM_COMMAND messages about this window, instead return the identifier of the clicked menu item
                curPoint.x,
                curPoint.y,
                0,
                hWnd(),
                NULL

            );

            // Original poster's line of code.  Haven't deleted it,
            // but haven't seen a need for it.
            //SendMessage(hwnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
            if (clicked == IDM_EXIT)
            {
                // quit the application.
                PostQuitMessage(0);
            }
        }
    }
    break;
    }

    return nothing;
}


//------------------------------------------------------------------------------
//	Forward declarations of functions included in this code module
//------------------------------------------------------------------------------
Maybe<HWND> CreateWorker();

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    TrayWindow trayWindow(hInstance);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FIREWORKSWALLPAPERTRAY));

    MSG msg;

    Maybe<HWND> worker = CreateWorker();
    ScopedPtr<JulyFourthSaver> saver;

    if (worker == nothing)
    {
        LogDebug("Unable to create worker.");
    }
    else
    {
        saver.set(new JulyFourthSaver(hInstance, *worker));
        saver->setRunTimer();
    }

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

BOOL enumProc(HWND hWnd, LPARAM param)
{
    HWND hShell = FindWindowEx(hWnd, NULL, kShellWindow, NULL);

    if (!hShell)
    {
        //continue search
        return TRUE;
    }

    HWND hWorker = FindWindowEx(NULL, hWnd, kWorkerWindow, NULL);

    if (!hWorker)
    {
        //continue search
        return TRUE;
    }

    *(HWND*)param = hWorker;

    //stop search
    return FALSE;
}

Maybe<HWND> findWallpaperWindow(HWND hTopWindow)
{
    HWND result = 0;

    EnumWindows(&enumProc, LPARAM(&result));

    if (result != 0)
    {
        return result;
    }

    return nothing;
}

Maybe<HWND> CreateWorker()
{
    //Find the Desktop handler
    HWND hProgman = FindWindow(kHookTarget, NULL);

    if (!hProgman)
    {
        printf("Unable to find top level Window\n");
        return nothing;
    }

    printf("Creating worker Window on Desktop\n");
    SendMessage(hProgman, kSecretMessage, WPARAM(0x000D), LPARAM(1));

    return findWallpaperWindow(hProgman);
}
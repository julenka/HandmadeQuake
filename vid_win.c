#include "winquake.h"
#include "quakedef.h"

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // TODO: catch any relevant messages here.
    // switch(uMsg);
    // Do the default behavior defined by Windows
    LRESULT result = 0;

    switch (uMsg)
    {

    case WM_ACTIVATE:
        break;
    case WM_CREATE:
        break;
    case WM_KEYUP:
    case WM_DESTROY:
        Sys_Shutdown();
        break;
    default:
        result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }
    return result;
}

void VID_Init(void) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = g_hInstance;
    // Load the generic error as the cursor to use when program is running
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "Module 2";

    if (!RegisterClass(&wc))
        exit(EXIT_FAILURE);

    HWND mainWindow;
    DWORD windowStyle = WS_OVERLAPPEDWINDOW;

    RECT r;
    r.top = r.left = 0;
    r.right = 800;
    r.bottom = 600;

    AdjustWindowRect(&r, windowStyle, FALSE);

    mainWindow = CreateWindowEx(
        0,
        "Module 2",
        "Lesson 2.1",
        windowStyle,
        CWMO_DEFAULT,
        CWMO_DEFAULT,
        r.right - r.left,
        r.bottom - r.top,
        NULL,
        NULL,
        g_hInstance,
        0);

    ShowWindow(mainWindow, SW_SHOWDEFAULT);

    HDC deviceContext = GetDC(mainWindow);
    PatBlt(deviceContext, 0, 0, 800, 600, BLACKNESS);
    ReleaseDC(mainWindow, deviceContext);

}
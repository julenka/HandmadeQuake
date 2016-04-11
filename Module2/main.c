#include <windows.h>

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // TODO: catch any relevant messages here.
    // switch(uMsg);
    // Do the default behavior defined by Windows
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInstance;
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
        hInstance,
        0);

    ShowWindow(mainWindow, SW_SHOWDEFAULT);

    HDC deviceContext = GetDC(mainWindow);
    PatBlt(deviceContext, 0, 0, 800, 600, BLACKNESS);
    ReleaseDC(mainWindow, deviceContext);

    return 0;
}




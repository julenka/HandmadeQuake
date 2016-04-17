#include <windows.h>
#include <stdio.h>
#include "host.h"

static BOOL IsRunning = TRUE;

///////////////////////////////
// TIMER CODE
static double SecondsPerTick = 0;
static __int64 TimeCount;
// Number of seconds that have passed in the game so far
static double TimePassed;
///////////////////////////////
float Sys_InitFloatTime(void)
{
    LARGE_INTEGER frequency;
    QueryPerformanceCounter(&frequency);
    SecondsPerTick = 1.0 / frequency.QuadPart;

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    TimeCount = counter.QuadPart;
    return 0;
}

float Sys_FloatTime(void) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    __int64 interval = counter.QuadPart - TimeCount;
    TimeCount = counter.QuadPart;
    double secondsGoneBy = interval * SecondsPerTick;
    TimePassed += secondsGoneBy;
    return (float)TimePassed;
}
void Sys_Shutdown() {
    IsRunning = FALSE;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // TODO: catch any relevant messages here.
    // switch(uMsg);
    // Do the default behavior defined by Windows
    LRESULT result = 0;

    switch (uMsg)
    {
    case WM_KEYUP:
        break;
    case WM_ACTIVATE:
        break;
    case WM_CREATE:
        break;
    case WM_DESTROY:
        Sys_Shutdown();
        break;
    default:
        result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }
    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    Host_Init();
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

    float oldTime = Sys_InitFloatTime();
    float targetTime = 1.0f / 60.0f; // 60fps is target 
    float timeAccumulated = 0;

    MSG msg;
    while (IsRunning)
    {
        while (PeekMessage(
            &msg,
            NULL,
            0,
            0,
            PM_REMOVE
            ))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        float newTime = Sys_FloatTime();
        timeAccumulated += newTime - oldTime;
        oldTime = newTime;

        if (timeAccumulated > targetTime) 
        {
            Host_Frame(newTime - oldTime);
            timeAccumulated -= targetTime;
        }
        // Update
        // Draw
    }

    Host_Shutdown();
    return 0;
}




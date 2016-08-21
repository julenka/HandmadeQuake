#include "winquake.h"
#include "quakedef.h"

#include "host.h"

static BOOL IsRunning = TRUE;
HINSTANCE g_hInstance;

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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    COM_ParseCmdLine(lpCmdLine);

    g_hInstance = hInstance;
    Host_Init();

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

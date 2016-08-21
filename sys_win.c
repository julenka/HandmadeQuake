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
    QueryPerformanceFrequency(&frequency);
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

void Sys_SendKeyEvents(void) {
    MSG msg;
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

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    COM_ParseCmdLine(lpCmdLine);

    g_hInstance = hInstance;
    Host_Init();

    float oldTime = Sys_InitFloatTime();

    while (IsRunning)
    {
        float newTime = Sys_FloatTime();
        Host_Frame(newTime - oldTime);
        oldTime = newTime;
        // Update
        // Draw
    }

    Host_Shutdown();
    return 0;
}

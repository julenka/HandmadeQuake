#include "winquake.h"
#include "quakedef.h"


#define BUFFER_WIDTH 320
#define BUFFER_HEIGHT 240
#define INITIAL_WINDOW_WIDTH 800
#define INITIAL_WINDOW_HEIGHT 600
#define BYTES_PER_PIXEL 1

#define FULL_SCREEN 0 

static int g_windowWidth;
static int g_windowHeight;
static void* g_backBuffer;
static HWND g_mainWindow;
static LPCTSTR g_windowClassName = "Module 3";

typedef struct dibinfo_x
{
    BITMAPINFOHEADER    header;
    RGBQUAD             acolors[256];
} dibinfo_t;

dibinfo_t g_bitmapInfo = { 0 };


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
        if (wParam == VK_ESCAPE) {
            Sys_Shutdown();
        }
        else if (wParam == 0x49) { // i key
            Vid_SetMode(640, 480);
        }
        break ;
    default:
        result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }
    return result;
}

void Vid_SetMode(int windowWidth, int windowHeight) {
    if (g_backBuffer) {
        Vid_Shutdown();
    }
    g_windowWidth = windowWidth;
    g_windowHeight = windowHeight;

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    DWORD dwExStyle = 0;

    int fullScreen = FALSE;
    if (fullScreen)
    {
        DEVMODE dmScreenSettings = { 0 };
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = BUFFER_WIDTH;
        dmScreenSettings.dmPelsHeight = BUFFER_HEIGHT;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            dwExStyle = WS_EX_APPWINDOW;
            windowStyle = WS_POPUP;
        }
        else
        {
            fullScreen = 0;
        }
    }

    RECT r;
    r.top = r.left = 0;
    r.right = g_windowWidth;
    r.bottom = g_windowHeight;

    AdjustWindowRect(&r, windowStyle, FALSE);

    g_mainWindow = CreateWindowEx(
        dwExStyle,
        g_windowClassName,
        "Lesson 3.5",
        windowStyle,
        CWMO_DEFAULT,
        CWMO_DEFAULT,
        r.right - r.left,
        r.bottom - r.top,
        NULL,
        NULL,
        g_hInstance,
        0);


    if (fullScreen)
    {
        SetWindowLong(g_mainWindow, GWL_STYLE, 0);
    }

    ShowWindow(g_mainWindow, SW_SHOWDEFAULT);

    // Define bitmapinfo
    g_bitmapInfo.header.biSize = sizeof(g_bitmapInfo.header);
    g_bitmapInfo.header.biWidth = BUFFER_WIDTH;
    g_bitmapInfo.header.biHeight = -BUFFER_HEIGHT;
    g_bitmapInfo.header.biPlanes = 1;
    g_bitmapInfo.header.biBitCount = 8 * BYTES_PER_PIXEL;
    g_bitmapInfo.header.biCompression = BI_RGB;

    g_backBuffer = malloc(BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL);


    HDC deviceContext = GetDC(g_mainWindow);
    PatBlt(deviceContext, 0, 0, 800, 600, BLACKNESS);
    ReleaseDC(g_mainWindow, deviceContext);
}

void Vid_Init(void) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = g_hInstance;
    // Load the generic error as the cursor to use when program is running
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = g_windowClassName;

    if (!RegisterClass(&wc)) {
        exit(GetLastError());
    }

    Vid_SetMode(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
}

void Vid_Update(void) {
    HDC dc = GetDC(g_mainWindow);
    StretchDIBits(
        dc,
        0, 0, g_windowWidth, g_windowHeight,
        0, 0, g_windowWidth, g_windowHeight,
        g_backBuffer,
        &g_bitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
    DeleteDC(dc);
}

void Vid_Shutdown(void) {
    DestroyWindow(g_mainWindow);
    free(g_backBuffer);
    g_backBuffer = 0;
}
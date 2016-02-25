#include "quakedef.h"
#include "winquake.h"

static int BufferWidth = 640;
static int BufferHeight = 480;
static int WindowWidth = 640;
static int WindowHeight = 480;
static int BytesPerPixel = 4;
void* BackBuffer = NULL;
HWND MainWindow;

BITMAPINFO BitMapInfo = { 0 };

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT Result = 0;

    // catch any relevant messages here
    switch (uMsg)
    {
    case WM_ACTIVATE:
       
        break;

    case WM_CREATE:
        
        break;

    case WM_DESTROY:
        //Sys_Shutdown();
        break;

    case WM_KEYDOWN:
    {
        if (wParam == 'A')
        {
            VID_SetMode(640, 480);
        }
        else if (wParam == 'S')
        {
            VID_SetMode(800, 600);
        }
        else if (wParam == 'D')
        {
            VID_SetMode(1024, 768);
        }
        else if (wParam == 'Q')
        {
            Sys_Shutdown();
        }

    } break;

    default:
        Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return Result;
}

void VID_SetMode(int Width, int Height)
{
    if (BackBuffer)
    {
        VID_Shutdown();
    }

    WindowWidth = Width;
    WindowHeight = Height;
    
    BufferHeight = WindowHeight;
    BufferWidth = WindowWidth;

    //WNDCLASSEX wc = { 0 };
    //wc.cbSize = sizeof(wc);
    //wc.lpfnWndProc = MainWndProc;
    //wc.hInstance = GlobalInstance;
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.lpszClassName = "Module 3";

    //if (!RegisterClassEx(&wc))
    //    exit(EXIT_FAILURE);

    DWORD WindowExStyle = 0;
    DWORD WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    //BOOL Fullscreen = FALSE;
    //
    //if (Fullscreen)
    //{
    //    DEVMODE dmScreenSettings = { 0 };
    //    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    //    dmScreenSettings.dmPelsWidth = BufferWidth;
    //    dmScreenSettings.dmPelsHeight = BufferHeight;
    //    dmScreenSettings.dmBitsPerPel = 32;
    //    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
    //
    //    if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) == //DISP_CHANGE_SUCCESSFUL)
    //    {
    //        dwExStyle = WS_EX_APPWINDOW;
    //        dwStyle = WS_POPUP;
    //    }
    //    else
    //    {
    //        Fullscreen = FALSE;
    //    }
    //}

    // create rectangle for window
    RECT r = { 0 };
    r.right = WindowWidth;
    r.bottom = WindowHeight;
    AdjustWindowRectEx(&r, WindowStyle, 0, WindowExStyle);

    // create our window
    MainWindow = CreateWindowEx(
        WindowExStyle, "Module 3",
        "Lesson 3.5", WindowStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left, r.bottom - r.top,
        NULL, NULL,
        GlobalInstance, 0);

    ShowWindow(MainWindow, SW_SHOWDEFAULT);

    HDC DeviceContext = GetDC(MainWindow);
    PatBlt(DeviceContext, 0, 0, BufferWidth, BufferHeight, BLACKNESS);
    ReleaseDC(MainWindow, DeviceContext);

    // define our bitmap info
    BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
    BitMapInfo.bmiHeader.biWidth = BufferWidth;
    BitMapInfo.bmiHeader.biHeight = -BufferHeight;
    BitMapInfo.bmiHeader.biPlanes = 1;
    BitMapInfo.bmiHeader.biBitCount = 8 * BytesPerPixel;
    BitMapInfo.bmiHeader.biCompression = BI_RGB;

    BackBuffer = malloc(BufferWidth * BufferHeight * BytesPerPixel);

}

void VID_Init(void)
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = GlobalInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "Module 3";

    if (!RegisterClassEx(&wc))
        exit(EXIT_FAILURE);

    VID_SetMode(WindowWidth, WindowHeight);
}

void VID_Update(void)
{
    HDC dc = GetDC(MainWindow);
    StretchDIBits(dc,
        0, 0, WindowWidth, WindowHeight,
        0, 0, BufferWidth, BufferHeight,
        BackBuffer, (BITMAPINFO*)&BitMapInfo,
        DIB_RGB_COLORS, SRCCOPY);
    ReleaseDC(MainWindow, dc);
}

void VID_Shutdown(void)
{
    DestroyWindow(MainWindow);
    free(BackBuffer);
    BackBuffer = NULL;
}
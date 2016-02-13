#include <windows.h>

int Running = 1;
int BufferWidth = 640;
int BufferHeight = 480;
void* BackBuffer;

BITMAPINFO BitMapInfo = { 0 };

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT Result = 0;

    switch (uMsg)
    {
    case WM_DESTROY:
        Running = 0;
        break;

    case WM_KEYUP:
        Running = 0;
        break;

    default:
        Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return Result;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // define our window class
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Module 3";

    RegisterClassExA(&wc);

    DWORD dwExStyle = 0;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    BOOL Fullscreen = FALSE;

    if (Fullscreen)
    {
        DEVMODE dmScreenSettings = { 0 };
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = BufferWidth;
        dmScreenSettings.dmPelsHeight = BufferHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
        {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP;
        }
        else
        {
            Fullscreen = FALSE;
        }
    }

    // create rectangle for window
    RECT r = { 0 };
    r.right = BufferWidth;
    r.bottom = BufferHeight;
    AdjustWindowRectEx(&r, dwStyle, 0, dwExStyle);

    // create our window
    HWND MainWindow = CreateWindowEx(
        dwExStyle, "Module 3",
        "Lesson 3.2", dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left, r.bottom - r.top,
        NULL, NULL,
        hInstance, 0);

    if (Fullscreen)
        SetWindowLong(MainWindow, GWL_STYLE, 0);

    ShowWindow(MainWindow, nShowCmd);

    // define our bitmap info
    BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
    BitMapInfo.bmiHeader.biWidth = BufferWidth;
    BitMapInfo.bmiHeader.biHeight = BufferHeight;
    BitMapInfo.bmiHeader.biPlanes = 1;
    BitMapInfo.bmiHeader.biBitCount = 32;
    BitMapInfo.bmiHeader.biCompression = BI_RGB;

    BackBuffer = malloc(BufferWidth * BufferHeight * 4);

    MSG msg;
    while (Running)
    {
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        int *MemoryWalker = (int*)BackBuffer;
        for(int Height = 0; Height < BufferHeight; Height++)
        {
            for(int Width = 0; Width < BufferWidth; Width++)
            {
                char Red = rand() % 256;
                char Green = rand() % 256;
                char Blue = rand() % 256;

                *MemoryWalker++ = ((Red << 16) | (Green << 8) | Blue);
            }
        }

        HDC dc = GetDC(MainWindow);
        StretchDIBits(dc,
            0, 0, BufferWidth, BufferHeight,
            0, 0, BufferWidth, BufferHeight,
            BackBuffer, &BitMapInfo,
            DIB_RGB_COLORS, SRCCOPY);
        DeleteDC(dc);
    }

    return EXIT_SUCCESS;
}
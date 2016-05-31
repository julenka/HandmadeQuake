#include <Windows.h>

#define BUFFER_WIDTH 640
#define BUFFER_HEIGHT 480
#define BYTES_PER_PIXEL 4

#define FULL_SCREEN 0 

int g_running = 1;
void* g_backBuffer;


typedef struct dibinfo_x
{
    BITMAPINFOHEADER    header;
    RGBQUAD             acolors[256];
} dibinfo_t;

dibinfo_t g_bitmapInfo = { 0 };


void DrawRect(size_t x, size_t y, size_t w, size_t h, unsigned char r, unsigned char g, unsigned char b, unsigned char *buffer) {
    size_t pixel_width = sizeof(unsigned char) * 4;
    size_t start_index = y * BUFFER_WIDTH * pixel_width + x * pixel_width;
    unsigned int color = (r << 16) | (g << 8) | b;
    buffer = buffer + start_index;
    for (size_t row = 0; row < h; row++)
    {
        for (size_t col = 0; col < w; col++)
        {
            buffer[0] = b;
            buffer[1] = g;
            buffer[2] = r;
            buffer += pixel_width;
        }
        buffer += pixel_width * (BUFFER_WIDTH - w);
    }
}

void DrawRect8(int x, int y, int w, int h, unsigned char color, unsigned char *buffer) {
    size_t start_index = y * BUFFER_WIDTH + x;
    buffer = buffer + start_index;
    for (size_t row = 0; row < h; row++)
    {
        for (size_t col = 0; col < w; col++)
        {
            *buffer++ = color;
        }
        buffer += (BUFFER_WIDTH - w);
    }
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (uMsg)
    {
    case WM_DESTROY:
    case WM_KEYUP:
        g_running = 0;
        break;
    default:
        result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }
    return result;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Register window class
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "Module 3";

    if (!RegisterClassA(&wc))
        exit(EXIT_FAILURE);

    HWND mainWindow;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD dwExStyle = 0;

    int fullScreen = FULL_SCREEN;
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
            dwStyle = WS_POPUP;
        }
        else 
        {
            fullScreen = 0;
        }
    }

    // Create Window
    RECT r = { 0 };
    r.top = r.left = 0;
    r.right = BUFFER_WIDTH;
    r.bottom = BUFFER_HEIGHT;

    AdjustWindowRect(&r, dwStyle, FALSE);

    mainWindow = CreateWindowEx(
        dwExStyle,
        "Module 3",
        "Lesson 3.3",
        dwStyle,
        CWMO_DEFAULT,
        CWMO_DEFAULT,
        r.right - r.left,
        r.bottom - r.top,
        NULL,
        NULL,
        hInstance,
        0);

    if (fullScreen)
    {
        SetWindowLong(mainWindow, GWL_STYLE, 0);
    }

    ShowWindow(mainWindow, SW_SHOWDEFAULT);


    // Create bitmapinfo
    g_bitmapInfo.header.biSize = sizeof(g_bitmapInfo.header);
    g_bitmapInfo.header.biWidth = BUFFER_WIDTH;
    g_bitmapInfo.header.biHeight = -BUFFER_HEIGHT;
    g_bitmapInfo.header.biPlanes = 1;
    g_bitmapInfo.header.biBitCount = 8 * BYTES_PER_PIXEL;
    g_bitmapInfo.header.biCompression = BI_RGB;

    g_backBuffer = malloc(BUFFER_WIDTH * BUFFER_HEIGHT * BYTES_PER_PIXEL);


    if (BYTES_PER_PIXEL == 1) {
        g_bitmapInfo.acolors[0].rgbRed = 0;
        g_bitmapInfo.acolors[0].rgbGreen = 0;
        g_bitmapInfo.acolors[0].rgbBlue = 0;

        for (size_t i = 0; i < 256; i++)
        {
            g_bitmapInfo.acolors[i].rgbRed = rand() % 256;
            g_bitmapInfo.acolors[i].rgbGreen = rand() % 256;
            g_bitmapInfo.acolors[i].rgbBlue = rand() % 256;

        }
    }


    // Game loop
    MSG msg;
    while (g_running)
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

            // set to solid color
            if (BYTES_PER_PIXEL == 4) {
                int* idx = g_backBuffer;
                for (size_t i = 0; i < BUFFER_WIDTH * BUFFER_HEIGHT; i++)
                {
                    // unsigned char r = (rand() & 0x0000ff00) >> 8;
                    // unsigned char g = (rand() & 0x0000ff00) >> 8;
                    // unsigned char b = (rand() & 0x0000ff00) >> 8;

                    unsigned char r = rand() % 256;
                    unsigned char g = rand() % 256;
                    unsigned char b = rand() % 256;

                    *idx++ = (r << 16 | g << 8 | b);
                }
                DrawRect(10, 10, 300, 200, 255, 0, 0, g_backBuffer);

            }
            else {
                unsigned char* idx = g_backBuffer;
                for (size_t i = 0; i < BUFFER_WIDTH * BUFFER_HEIGHT; i++)
                {
                    *idx++ = 1;
                }
                DrawRect8(10, 10, 300, 200, 2, g_backBuffer);
            }

            HDC dc = GetDC(mainWindow);
            StretchDIBits(
                dc,
                0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
                0, 0, BUFFER_WIDTH, BUFFER_HEIGHT,
                g_backBuffer,
                &g_bitmapInfo,
                DIB_RGB_COLORS,
                SRCCOPY
            );
            DeleteDC(dc);
        }
    }

    return EXIT_SUCCESS;
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <windows.h>

int Running = 1;
int BufferWidth = 640;
int BufferHeight = 480;
int WindowWidth = 640;
int WindowHeight = 480;
int BytesPerPixel = 4;
void* BackBuffer;


typedef struct dibinfo_s
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          acolors[256];
} dibinfo_t;

dibinfo_t BitMapInfo = { 0 };

void DrawPic8(int X, int Y, int Width, int Height, 
    unsigned char* Source, unsigned char* Dest)
{
    // move to first pixel
    Dest += (BufferWidth * BytesPerPixel * Y) + (X*BytesPerPixel);

    unsigned char* BufferWalker = Dest;

    for (int HeightWalker = 0; HeightWalker < Height; HeightWalker++)
    {
        for (int WidthWalker = 0; WidthWalker < Width; WidthWalker++)
        {
            *BufferWalker = *Source;
            BufferWalker++;
            Source++;
        }

        Dest += BufferWidth * BytesPerPixel;
        BufferWalker = Dest;
    }
}

void DrawPic32(int X, int Y, int Width, int Height,
    unsigned char* Source, unsigned char* Dest)
{
    // move to first pixel
    Dest += (BufferWidth * BytesPerPixel * Y) + (X*BytesPerPixel);

    unsigned int* BufferWalker = (unsigned int*)Dest;

    for (int HeightWalker = 0; HeightWalker < Height; HeightWalker++)
    {
        for (int WidthWalker = 0; WidthWalker < Width; WidthWalker++)
        {
            int* ColorValue = (int*)&BitMapInfo.acolors[*Source];

            *BufferWalker = *ColorValue;
            BufferWalker++;
            Source++;
        }

        Dest += BufferWidth * BytesPerPixel;
        BufferWalker = (unsigned int*)Dest;
    }

}

void DrawRect8(int X, int Y, int Width, int Height, unsigned char Color, unsigned char* Buffer)
{
    if (X < 0)
        X = 0;

    if (Y < 0)
        Y = 0;

    if ((X + Width) > BufferWidth)
    {
        Width = BufferWidth - X;
    }

    if ((Y + Height) > BufferHeight)
    {
        Height = BufferHeight - Y;
    }

    // move to first pixel
    Buffer += (BufferWidth * BytesPerPixel * Y) + (X*BytesPerPixel);

    unsigned char* BufferWalker = Buffer;
    for (int HeightWalker = 0; HeightWalker < Height; HeightWalker++)
    {
        for (int WidthWalker = 0; WidthWalker < Width; WidthWalker++)
        {
            *BufferWalker = Color;
            BufferWalker++;
        }

        Buffer += BufferWidth * BytesPerPixel;
        BufferWalker = Buffer;
    }

}

void DrawRect(int X, int Y, int Width, int Height, 
    unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char* Buffer)
{
    if (X < 0)
        X = 0;

    if (Y < 0)
        Y = 0;

    if ((X + Width) > BufferWidth)
    {
        Width = BufferWidth - X;
    }

    if ((Y + Height) > BufferHeight)
    {
        Height = BufferHeight - Y;
    }

    unsigned int Color = ((Red << 16) | (Green << 8) | Blue);

    // move to first pixel
    Buffer += (BufferWidth * BytesPerPixel * Y) + (X*BytesPerPixel);

    int* BufferWalker = (int*)Buffer;
    for (int HeightWalker = 0; HeightWalker < Height; HeightWalker++)
    {
        for (int WidthWalker = 0; WidthWalker < Width; WidthWalker++)
        {
            *BufferWalker = Color;
            BufferWalker++;
        }

        Buffer += BufferWidth * BytesPerPixel;
        BufferWalker = (int*)Buffer;
    }
}

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
    AdjustWindowRectEx(&r, dwStyle, 0, dwExStyle);

    // create our window
    HWND MainWindow = CreateWindowEx(
        dwExStyle, "Module 3",
        "Lesson 3.4", dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left, r.bottom - r.top,
        NULL, NULL,
        hInstance, 0);

    //if (Fullscreen)
    //    SetWindowLong(MainWindow, GWL_STYLE, 0);

    ShowWindow(MainWindow, nShowCmd);

    // define our bitmap info
    BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
    BitMapInfo.bmiHeader.biWidth = BufferWidth;
    BitMapInfo.bmiHeader.biHeight = -BufferHeight;
    BitMapInfo.bmiHeader.biPlanes = 1;
    BitMapInfo.bmiHeader.biBitCount = 8 * BytesPerPixel;
    BitMapInfo.bmiHeader.biCompression = BI_RGB;

    BackBuffer = malloc(BufferWidth * BufferHeight * BytesPerPixel);

    //if (BytesPerPixel == 1)
    {
        FILE *Palette = fopen("palette.lmp", "r");
        void *RawData = malloc(256 * 3);
        unsigned char* PaletteData = RawData;
        size_t Ret = fread(PaletteData, 1, 768, Palette);

        for (int i = 0; i < 256; i++)
        {
            BitMapInfo.acolors[i].rgbRed = *PaletteData++;
            BitMapInfo.acolors[i].rgbGreen = *PaletteData++;
            BitMapInfo.acolors[i].rgbBlue = *PaletteData++;
        }

        free(RawData);
        fclose(Palette);
    }

    FILE * Disc = fopen("DISC.lmp", "r");
    int DiscWidth, DiscHeight;

    size_t RetVal = fread(&DiscWidth, 1, 4, Disc);
    RetVal = fread(&DiscHeight, 1, 4, Disc);
    void* DiscData = malloc(DiscWidth * DiscHeight);
    RetVal = fread(DiscData, 1, DiscWidth * DiscHeight, Disc);

    fclose(Disc);

    FILE *Pause = fopen("pause.lmp", "r");
    int PauseWidth, PauseHeight;

    RetVal = fread(&PauseWidth, 1, 4, Pause);
    RetVal = fread(&PauseHeight, 1, 4, Pause);
    void *PauseData = malloc(PauseWidth * PauseHeight);
    RetVal = fread(PauseData, 1, PauseWidth * PauseHeight, Pause);

    fclose(Pause);
    MSG msg;
    while (Running)
    {
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (BytesPerPixel == 4)
        {
            int *MemoryWalker = (int*)BackBuffer;
            for (int Height = 0; Height < BufferHeight; Height++)
            {
                for (int Width = 0; Width < BufferWidth; Width++)
                {
                    unsigned char Red = rand() % 256;
                    unsigned char Green = rand() % 256;
                    unsigned char Blue = rand() % 256;

                    *MemoryWalker++ = ((Red << 16) | (Green << 8) | Blue);
                }
            }

            DrawPic32(10, 10, DiscWidth, DiscHeight, DiscData, BackBuffer);
            DrawPic32(100, 100, PauseWidth, PauseHeight, PauseData, BackBuffer);
            //DrawRect(10, 10, 300, 150, 255, 0, 255, BackBuffer);
        }
        else if (BytesPerPixel == 1)
        {
            unsigned char *MemoryWalker = BackBuffer;
            for (int Height = 0; Height < BufferHeight; Height++)
            {
                for (int Width = 0; Width < BufferWidth; Width++)
                {
                    *MemoryWalker++ = rand() % 256;
                }
            }

            DrawPic8(10, 10, DiscWidth, DiscHeight, DiscData, BackBuffer);
            DrawPic8(100, 100, PauseWidth, PauseHeight, PauseData, BackBuffer);
            //DrawRect8(10, 10, 300, 150, 1, BackBuffer);
        }
        HDC dc = GetDC(MainWindow);
        StretchDIBits(dc,
            0, 0, WindowWidth, WindowHeight,
            0, 0, BufferWidth, BufferHeight,
            BackBuffer, (BITMAPINFO*)&BitMapInfo,
            DIB_RGB_COLORS, SRCCOPY);
        DeleteDC(dc);
    }

    free(BackBuffer);
    free(DiscData);
    free(PauseData);

    return EXIT_SUCCESS;
}
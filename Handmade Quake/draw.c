#include "quakedef.h"

void DrawPic32(int X, int Y, int Width, int Height,
    unsigned char* Source)
{
    uint8* Dest = Vid.BackBuffer;
    // move to first pixel
    Dest += (Vid.BufferWidth * Vid.BytesPerPixel * Y) + (X*Vid.BytesPerPixel);

    unsigned int* BufferWalker = (unsigned int*)Dest;

    for (int HeightWalker = 0; HeightWalker < Height; HeightWalker++)
    {
        for (int WidthWalker = 0; WidthWalker < Width; WidthWalker++)
        {
            int* ColorValue = &Vid.ColorPtr[*Source];

            if (*Source != 0xff)
                *BufferWalker = *ColorValue;
            BufferWalker++;
            Source++;
        }

        Dest += Vid.BufferWidth * Vid.BytesPerPixel;
        BufferWalker = (unsigned int*)Dest;
    }

}

#pragma once

typedef struct
{
    uint8* BackBuffer;
    uint32* ColorPtr;
    int32 BufferHeight;
    int32 BufferWidth;
    int32 BytesPerPixel;
} viddef_t;

extern viddef_t Vid;

void VID_Init(void);
void VID_Update(void);
void VID_Shutdown(void);

void VID_SetMode(int ModeValue);
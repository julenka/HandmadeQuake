#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "file.h"

char* va(char* format, ...)
{
    va_list va;
    static char b[1024];

    va_start(va, format);

    vsprintf_s(b, 1024, format, va);


    va_end(va);
    return b;
}

int main()
{
    int Size;
    int FileHandle = Sys_FileOpenRead("Source.c", &Size);

    if (FileHandle == -1)
    {
        int a = 5; a;
    }

    void *Buffer = malloc(Size);

    Sys_FileRead(FileHandle, Buffer, Size);

    int WriteFile = Sys_FileOpenWrite("Source.out");

    if (WriteFile == -1)
    {
        int a = 5; a;
    }
    
    Sys_FileWrite(WriteFile, Buffer, Size);

    Sys_FileClose(FileHandle);
    Sys_FileClose(WriteFile);

    free(Buffer);
    Buffer = NULL;

    // char* ColorData = LoadFile("palette.lmp");

    return 0;
}

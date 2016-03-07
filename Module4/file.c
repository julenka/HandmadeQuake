#include <stdio.h>

#define MAX_HANDLES 10
static FILE* FileHandles[MAX_HANDLES] = { 0 };

static int FindHandle(void)
{
    for (int i = 0; i < MAX_HANDLES; i++)
    {
        if (FileHandles[i] == NULL)
            return i;
    }

    return -1;
}

static int FileLength(FILE *f)
{
    int Pos;
    int End;

    Pos = ftell(f);
    fseek(f, 0, SEEK_END);

    End = ftell(f);
    fseek(f, Pos, SEEK_SET);

    return End;
}

int Sys_FileOpenRead(char *Path, int *Size)
{
    int HandleIdx = FindHandle();

    FILE *FileHandle;

    fopen_s(&FileHandle, Path, "rb");

    if (FileHandle == 0)
    {
        if (Size != 0)
            *Size = -1;
        return -1;
    }

    FileHandles[HandleIdx] = FileHandle;

    if(Size != 0)
        *Size = FileLength(FileHandle);

    return HandleIdx;
}

int Sys_FileOpenWrite(char *Path)
{
    int HandleIdx = FindHandle();

    FILE *FileHandle = FileHandles[HandleIdx];

    fopen_s(&FileHandle, Path, "wb");

    if (FileHandle == 0)
    {
        HandleIdx = -1;
    }

    FileHandles[HandleIdx] = FileHandle;

    return HandleIdx;
}

void Sys_FileClose(int handle)
{
    if (handle < 0 || handle > MAX_HANDLES - 1)
        return;

    if (FileHandles[handle] != NULL)
    {
        fclose(FileHandles[handle]);
        FileHandles[handle] = NULL;
    }
}

void Sys_FileSeek(int Handle, int Position)
{
    if (Handle < 0 || Handle > MAX_HANDLES - 1)
        return;

    if (FileHandles[Handle] != NULL)
    {
        fseek(FileHandles[Handle], Position, SEEK_SET);
    }
}

int Sys_FileRead(int Handle, void *Dest, int Count)
{
    if (Handle < 0 || Handle > MAX_HANDLES - 1 || Dest == 0)
    {
        return 0;
    }

    int BytesRead = (int)fread(Dest, 1, Count, FileHandles[Handle]);

    return BytesRead;
}

int Sys_FileWrite(int Handle, void *Source, int Count)
{
    if (Handle < 0 || Handle > MAX_HANDLES - 1 || Source == 0)
    {
        return 0;
    }

    int BytesWritten = (int)fwrite(Source, 1, Count, FileHandles[Handle]);

    return BytesWritten;
}
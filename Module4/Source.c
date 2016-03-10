#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

typedef struct
{
    char Magic[4];
    int  DirectoryOffset;
    int  DirectoryLength;
} dpackheader_t;

typedef struct  
{
    char FileName[56];
    int  FilePosition;
    int  FileLength;
} dpackfile_t;

typedef struct  
{
    char FileName[64];
    int  FilePosition;
    int  FileLength;
} packfile_t;

typedef struct  
{
    char PackName[128];
    int  PackHandle;
    int  NumberOfFiles;
    packfile_t *PakFiles;
} pack_t;

char* va(char* format, ...)
{
    va_list va;
    static char b[1024];

    va_start(va, format);

    vsprintf_s(b, 1024, format, va);


    va_end(va);
    return b;
}

#define MAX_FILES_IN_PACK 2048

pack_t *COM_LoadPackFile(char *PakFile)
{
    int PakSize;
    int PakHandle = Sys_FileOpenRead(PakFile, &PakSize);

    if (PakHandle == -1)
    {
        int a = 5; a;
    }

    dpackheader_t PakHeader;

    Sys_FileRead(PakHandle, &PakHeader, sizeof(PakHeader));

    if (PakHeader.Magic[0] != 'P' || PakHeader.Magic[1] != 'A' || PakHeader.Magic[2] != 'C' || PakHeader.Magic[3] != 'K')
    {
        int a = 5; a;
    }

    int NumPackFiles = PakHeader.DirectoryLength / sizeof(dpackfile_t);

    packfile_t* FilesInMemory = malloc(NumPackFiles * sizeof(packfile_t));

    Sys_FileSeek(PakHandle, PakHeader.DirectoryOffset);

    dpackfile_t TempPackFile[MAX_FILES_IN_PACK];
    Sys_FileRead(PakHandle, (void *)TempPackFile, PakHeader.DirectoryLength);

    for (int i = 0; i < NumPackFiles; i++)
    {
        strcpy_s(FilesInMemory[i].FileName, 56, TempPackFile[i].FileName);
        FilesInMemory[i].FileLength = TempPackFile[i].FileLength;
        FilesInMemory[i].FilePosition = TempPackFile[i].FilePosition;
    }

    pack_t  *Pak0 = (void*)malloc(sizeof(pack_t));
    strcpy_s(Pak0->PackName, 64, PakFile);
    Pak0->PackHandle = PakHandle;
    Pak0->NumberOfFiles = NumPackFiles;
    Pak0->PakFiles = FilesInMemory;


    return Pak0;
}

int main()
{
    pack_t *Pak0 = COM_LoadPackFile("id1\\pak0.pak");
    pack_t *Pak1 = COM_LoadPackFile("id1\\pak1.pak");

    int a = 5; a;


    //int PakSize;
    //int PakHandle = Sys_FileOpenRead("id1\\pak0.pak", &PakSize);
    //
    //if (PakHandle == -1)
    //{
    //    int a = 5; a;
    //}
    //
    //dpackheader_t PakHeader;
    //
    //Sys_FileRead(PakHandle, &PakHeader, sizeof(PakHeader));
    //
    //if (PakHeader.Magic[0] != 'P' || PakHeader.Magic[1] != 'A' || PakHeader.Magic[2] != 'C' || PakHeader.Magic[3] != 'K')
    //{
    //    int a = 5; a;
    //}
    //
    //int NumPackFiles = PakHeader.DirectoryLength / sizeof(dpackfile_t);
    //
    //packfile_t* FilesInMemory = malloc(NumPackFiles * sizeof(packfile_t));
    //
    //Sys_FileSeek(PakHandle, PakHeader.DirectoryOffset);
    //
    //dpackfile_t TempPackFile[MAX_FILES_IN_PACK];
    //Sys_FileRead(PakHandle, (void *)TempPackFile, PakHeader.DirectoryLength);
    //
    //for (int i = 0; i < NumPackFiles; i++)
    //{
    //    strcpy_s(FilesInMemory[i].FileName, 56, TempPackFile[i].FileName);
    //    FilesInMemory[i].FileLength = TempPackFile[i].FileLength;
    //    FilesInMemory[i].FilePosition = TempPackFile[i].FilePosition;
    //}
    //
    //pack_t  *Pak0 = (void*)malloc(sizeof(pack_t));
    //strcpy_s(Pak0->PackName, 64, "id1\\pak0.pak");
    //Pak0->PackHandle = PakHandle;
    //Pak0->NumberOfFiles = NumPackFiles;
    //Pak0->PakFiles = FilesInMemory;



    //int Size;
    //int FileHandle = Sys_FileOpenRead("Source.c", &Size);
    //
    //if (FileHandle == -1)
    //{
    //    int a = 5; a;
    //}
    //
    //void *Buffer = malloc(Size);
    //
    //Sys_FileRead(FileHandle, Buffer, Size);
    //
    //int WriteFile = Sys_FileOpenWrite("Source.out");
    //
    //if (WriteFile == -1)
    //{
    //    int a = 5; a;
    //}
    //
    //Sys_FileWrite(WriteFile, Buffer, Size);
    //
    //Sys_FileClose(FileHandle);
    //Sys_FileClose(WriteFile);
    //
    //free(Buffer);
    //Buffer = NULL;

    // char* ColorData = LoadFile("palette.lmp");

    return 0;
}

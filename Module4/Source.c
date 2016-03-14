#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

#pragma pack(push,1)
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
    char FileName[56];
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

typedef struct searchpath_s
{
    pack_t  *Pack;
    struct searchpath_s *Next;
} searchpath_t;
#pragma pack(pop)

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
        return NULL;
    }

    dpackheader_t PakHeader;

    Sys_FileRead(PakHandle, &PakHeader, sizeof(PakHeader));

    if (PakHeader.Magic[0] != 'P' || PakHeader.Magic[1] != 'A' || PakHeader.Magic[2] != 'C' || PakHeader.Magic[3] != 'K')
    {
        return NULL;
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

searchpath_t *COM_Searchpaths = NULL;

void COM_AddGameDirectory(const char *Directory)
{
    char Buffer[128];
    pack_t *PakPointer;
    

    for (int i = 0; ; i++)
    {
        sprintf_s(Buffer, 128, "%s\\pak%d.pak", Directory, i);
        PakPointer = COM_LoadPackFile(Buffer);
        if (!PakPointer)
            break;
        searchpath_t *NewPath = (searchpath_t *)malloc(sizeof(searchpath_t));
        NewPath->Pack = PakPointer;
        NewPath->Next = COM_Searchpaths;
        COM_Searchpaths = NewPath;
    }
}

char *COM_FindFile(const char *Filename, int* FileLength)
{
    if (!Filename)
        return NULL;

    searchpath_t *Jogger;

    for (Jogger = COM_Searchpaths; Jogger != NULL; Jogger = Jogger->Next)
    {
        pack_t *CurrentPak = Jogger->Pack;

        for (int i = 0; i < CurrentPak->NumberOfFiles; i++)
        {
            if (!strcmp(Filename, CurrentPak->PakFiles[i].FileName))
            {
                Sys_FileSeek(CurrentPak->PackHandle, CurrentPak->PakFiles[i].FilePosition);
                
                char *FileData = (char *)malloc(CurrentPak->PakFiles[i].FileLength);
                Sys_FileRead(CurrentPak->PackHandle, FileData, CurrentPak->PakFiles[i].FileLength);

                if (FileLength)
                    *FileLength = CurrentPak->PakFiles[i].FileLength;
                return (char*)FileData;
            }
        }
    }

    return NULL;
}

int main()
{
    COM_AddGameDirectory("id1");

    int Length = 0;
    char *PaletteBits = COM_FindFile("gfx/palette.lmp", &Length);

    int a = 5; a;

    return 0;
}

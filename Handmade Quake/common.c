#include "quakedef.h"

char* va(char* format, ...)
{
    va_list va;
    static char b[1024];

    va_start(va, format);

    vsprintf_s(b, 1024, format, va);


    va_end(va);
    return b;
}

//////////////////////////////
//    CMD LINE PARSING
//////////////////////////////

int32 com_argc = 1;
uint8 *com_argv[MAX_NUM_ARGVS + 1];

void COM_ParseCmdLine(uint8 *lpCmdLine)
{
    com_argv[0] = "";

    while (*lpCmdLine && (com_argc < MAX_NUM_ARGVS + 1))
    {
        while (*lpCmdLine && ((*lpCmdLine <= 32) || (*lpCmdLine > 126)))
            lpCmdLine++;

        if (*lpCmdLine)
        {
            com_argv[com_argc] = lpCmdLine;
            com_argc++;

            while (*lpCmdLine && ((*lpCmdLine > 32) && (*lpCmdLine <= 126)))
                lpCmdLine++;

            if (*lpCmdLine)
            {
                *lpCmdLine = 0;
                lpCmdLine++;
            }
        }
    }

}

int32 COM_CheckParm(uint8 *parm)
{
    for (int32 i = 1; i < com_argc; i++)
    {
        if (!Q_strcmp(parm, com_argv[i]))
            return i;
    }

    return 0;
}

//////////////////////////////
//   END CMD LINE PARSING
//////////////////////////////


//////////////////////////////
//    FILE MGMT
//////////////////////////////

//typedefs
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
#pragma pack(pop)

typedef struct searchpath_s
{
    pack_t  *Pack;
    struct searchpath_s *Next;
} searchpath_t;

// data structures
#define MAX_FILES_IN_PACK 2048
searchpath_t *COM_Searchpaths = NULL;

// functions
static pack_t *COM_LoadPackFile(char *PakFile)
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
        Q_strcpy(FilesInMemory[i].FileName, TempPackFile[i].FileName);
        FilesInMemory[i].FileLength = TempPackFile[i].FileLength;
        FilesInMemory[i].FilePosition = TempPackFile[i].FilePosition;
    }

    pack_t  *NewPak = (void*)malloc(sizeof(pack_t));
    Q_strcmp(NewPak->PackName, PakFile);
    NewPak->PackHandle = PakHandle;
    NewPak->NumberOfFiles = NumPackFiles;
    NewPak->PakFiles = FilesInMemory;

    return NewPak;
}

static void COM_AddGameDirectory(const char *Directory)
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
            if (!Q_strcmp(Filename, CurrentPak->PakFiles[i].FileName))
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

void COM_FileInit(void)
{
    COM_AddGameDirectory("id1");
}

void COM_FileShutdown(void)
{
    while (COM_Searchpaths)
    {
        searchpath_t* DeleteMe = COM_Searchpaths;
        COM_Searchpaths = COM_Searchpaths->Next;

        free(DeleteMe->Pack->PakFiles);
        free(DeleteMe->Pack);
        free(DeleteMe);
    }
}

//////////////////////////////
//    END FILE MGMT
//////////////////////////////

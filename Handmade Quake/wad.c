#include "quakedef.h"

typedef struct
{
    int32   FilePosition;
    int32   DiskSize;
    int32   Size;
    int8    FileType;
    int8    CompressionType;
    int16   Padding;
    int8    Name[16];
} lumpinfo_t;

typedef struct  
{
    int8    Magic[4];
    int32   NumberOfLumps;
    int32   DirectoryOffset;
} wadinfo_t;

static int8 *WadFile;
static int32 LumpCount;
static lumpinfo_t *Lumps;

static void W_CleanupName(int8 *In, int8 *Out)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        char c = In[i];
        if (c == 0)
            break;

        if (c >= 'A' && c <= 'Z')
        {
            c |= (1 << 5);  
        }

        Out[i] = c;
    }

    for (; i < 16; i++)
    {
        Out[i] = 0;
    }
}

void W_LoadWadFile(int8* Filename)
{
    int32 FileOffsetCount = 0;

    int Length = 0;
    WadFile = COM_FindFile(Filename, &Length);

    if (!WadFile)
    {
        assert(0);
    }

    wadinfo_t *WadHeader = (wadinfo_t*)WadFile;

    if (WadHeader->Magic[0] != 'W'
        || WadHeader->Magic[1] != 'A'
        || WadHeader->Magic[2] != 'D'
        || WadHeader->Magic[3] != '2')
    {
        assert(0);
    }

    LumpCount = WadHeader->NumberOfLumps;
    FileOffsetCount = WadHeader->DirectoryOffset;
    Lumps = (lumpinfo_t*)(WadFile + FileOffsetCount);

    lumpinfo_t *LumpWalker = Lumps;
    for (int i = 0; i < LumpCount; i++)
    {
        W_CleanupName(LumpWalker->Name, LumpWalker->Name);
        LumpWalker++;
    }
}

static lumpinfo_t *W_GetLumpInfo(int8 *Name)
{
    int8 CleanName[16];

    W_CleanupName(Name, CleanName);

    lumpinfo_t* LumpWalker = Lumps;
    for (int i = 0; i < LumpCount; i++)
    {
        if (!Q_strcmp(CleanName, LumpWalker->Name))
            return LumpWalker;

        LumpWalker++;
    }

    assert(0);
    return NULL;
}

void *W_GetLumpName (int8* Name)
{
    lumpinfo_t *Lump;

    Lump = W_GetLumpInfo(Name);

    return (void*)(WadFile + Lump->FilePosition);
}
#pragma once

typedef enum { false, true }	qboolean;

#define MAX_NUM_ARGVS 50

extern int32 com_argc;
extern uint8 *com_argv[MAX_NUM_ARGVS + 1];

void COM_ParseCmdLine(uint8 *lpCmdLine);
int32 COM_CheckParm(uint8 *parm);

// file mgmt
void COM_FileInit(void);
char *COM_FindFile(const char *Filename, int* FileLength);
void COM_FileShutdown(void);

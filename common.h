#pragma once

#define MAX_NUM_ARGVS 50

extern int32 com_argc;
extern uint8 *com_largv[MAX_NUM_ARGVS + 1];

int32 COM_CheckParm(uint8 *parm);
void COM_ParseCmdLine(uint8 *lpCmdLine);
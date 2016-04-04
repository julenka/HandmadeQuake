#include "quakedef.h"

int32 com_argc = 1;
uint8 *com_largv[MAX_NUM_ARGVS + 1];

void COM_ParseCmdLine(uint8 *lpCmdLine) 
{
    com_largv[0] = "";

    while (*lpCmdLine && (com_argc < MAX_NUM_ARGVS + 1))
    {
        while (*lpCmdLine && ((*lpCmdLine <= 32) || (*lpCmdLine > 126)))
            lpCmdLine++;

        if (*lpCmdLine)
        {
            com_largv[com_argc] = lpCmdLine;
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
        if (!Q_strcmp(parm, com_largv[i]))
            return i;
    }

    return 0;
}


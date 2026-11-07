#ifndef _SYS_MODE_H
#define _SYS_MODE_H

#include <nitro/fs.h>
#include "sys.h"

typedef struct SysModeParam_s {
    BOOL isBBP;
    BOOL exitBBP;
    void *heapStart;
    void *heapEnd;
    BBPSysBootParam bbp;
} SysModeParam;

extern SysModeParam Sys_ModeParam;

BOOL Sys_IsModeBBP(void);

#endif
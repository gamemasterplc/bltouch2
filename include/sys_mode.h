#ifndef _SYS_MODE_H
#define _SYS_MODE_H

#include <nitro/fs.h>

typedef struct SysBBPParam_s {
    u32 unk0;
    void *heapPtr;
    u32 heapSize;
    u32 unkC;
    u32 ovlMenu;
    u32 ovlBattle;
} SysBBPParam;


typedef struct SysModeParam_s {
    BOOL isBBP;
    BOOL exitBBP;
    void *heapStart;
    void *heapEnd;
    SysBBPParam bbp;
} SysModeParam;

extern SysModeParam Sys_ModeParam;

BOOL Sys_IsModeBBP(void);

#endif
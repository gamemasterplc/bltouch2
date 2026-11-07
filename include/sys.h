#ifndef SYS_H
#define SYS_H

#include <nitro/types.h>

typedef struct BBPSysBootParam_s {
    u32 unk0;
    void *heapPtr;
    u32 heapSize;
    u32 unkC;
    u32 ovlMenu;
    u32 ovlBattle;
} BBPSysBootParam;

#define BL_SYS_LANGUAGE_NONE -1

#ifdef SYS_BBP
#define BL_SysGetLanguage BBP_SysGetLanguage
#define BL_SysCheckSleep BBP_SysCheckSleep
#define BL_SysResetMBBusy BBP_SysResetMBBusy
#define BL_SysResetEnable BBP_SysResetEnable
#define BL_SysResetDisable BBP_SysResetDisable
#endif

#ifdef SYS_BBP
BBPSysBootParam *BBP_SysGetBootParam(void);
#endif

int BL_SysGetLanguage(void);
BOOL BL_SysCheckSleep(void);
void BL_SysResetMBBusy(void);
void BL_SysResetEnable(void);
void BL_SysResetDisable(void);

#endif
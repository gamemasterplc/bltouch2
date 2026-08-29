#ifndef SYS_H
#define SYS_H

#include <nitro/types.h>

#define BL_SYS_LANGUAGE_NONE -1

#ifdef SYS_BBP
#define BL_SysGetLanguage BBP_SysGetLanguage
#define BL_SysCheckSleep BBP_SysCheckSleep
#endif

int BL_SysGetLanguage(void);
BOOL BL_SysCheckSleep(void);

#endif
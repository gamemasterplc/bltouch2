#ifndef SYS_H
#define SYS_H

#define BL_SYS_LANGUAGE_NONE -1

#ifdef SYS_BBP
#define BL_SysGetLanguage BBP_SysGetLanguage
#endif

int BL_SysGetLanguage(void);

#endif
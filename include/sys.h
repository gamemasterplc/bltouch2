#ifndef SYS_H
#define SYS_H

#define SYS_LANGUAGE_NONE -1

#ifdef SYS_BBP
#define SysGetLanguage BBP_SysGetLanguage
#else
#define SysGetLanguage BL_SysGetLanguage
#endif

int SysGetLanguage(void);

#endif
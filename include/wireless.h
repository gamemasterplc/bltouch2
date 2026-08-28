#ifndef WIRELESS_H
#define WIRELESS_H

#include "file.h"

#ifdef SYS_BBP
#define BL_WLGetFilePtr BBP_WLGetFilePtr
#endif

BLFile *BL_WLGetFilePtr(void);

#endif
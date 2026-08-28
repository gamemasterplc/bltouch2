#ifndef FILETBL_H
#define FILETBL_H

#include <nitro/types.h>

#ifdef SYS_BBP
#define BL_GetFilePath BBP_GetFilePath
#endif

char *BL_GetFilePath(u32 fileID);


#endif
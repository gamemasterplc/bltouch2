#ifndef FILETBL_H
#define FILETBL_H

#include <nitro/types.h>

#ifdef SYS_BBP
#include "bbp/dirid.h"
#else
#include "bl/dirid.h"
#endif

#define BL_FILEID(dir, file) (((dir) << 16)|(file))

#ifdef SYS_BBP
#define BL_GetFilePath BBP_GetFilePath
#endif

char *BL_GetFilePath(u32 fileID);


#endif
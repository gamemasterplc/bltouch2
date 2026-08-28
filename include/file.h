#ifndef FILE_H
#define FILE_H

#include <nitro/types.h>


#ifdef SYS_BBP
#define BL_FileLoad BBP_FileLoad
#define BL_FileUnload BBP_FileUnload
#define BL_FileInit BBP_FileInit
#define BL_FileClose BBP_FileClose
#endif

typedef struct BLFile_s BLFile;

struct BLFile_s {
    u32 fileID;
    s16 refCnt;
    u32 size;
    BLFile *next;
    BLFile *prev;
    u8 data[];
};

BLFile *BL_FileLoad(u32 fileID, BOOL temp);
void BL_FileUnload(BLFile *file);
void BL_FileInit(void);
void BL_FileClose(void);

#endif
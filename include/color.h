#ifndef COLOR_H
#define COLOR_H

#include <nitro/types.h>
#include "display.h"
#include "gfxheap.h"
#include "file.h"

enum BLColorPltt_e {
    BL_COLOR_OBJ_PLTT,
    BL_COLOR_BG_PLTT,
    BL_COLOR_OBJ_EXT_PLTT,
    BL_COLOR_BG_EXT_PLTT_1,
    BL_COLOR_BG_EXT_PLTT_2,
    BL_COLOR_BG_EXT_PLTT_3,
    BL_COLOR_BG_EXT_PLTT_4,
    BL_COLOR_MAX_PLTT
};

typedef u32 BLColorKey;

#ifdef SYS_BBP
#define BL_ColorInit BBP_ColorInit
#define BL_ColorClose BBP_ColorClose
#define BL_ColorAlloc BBP_ColorAlloc
#define BL_ColorFree BBP_ColorFree
#define BL_ColorGetOfs BBP_ColorGetOfs
#define BL_ColorWrite BBP_ColorWrite
#define BL_ColorWriteEx BBP_ColorWriteEx
#endif

void BL_ColorInit(void);
void BL_ColorClose(void);
BLColorKey BL_ColorAlloc(u16 *data, BLFile *file, u32 display, u32 pltt, u32 idx, u8 num, int isTex);
void BL_ColorFree(BLColorKey key);
u8 BL_ColorGetOfs(BLColorKey key);
#ifndef SYS_BBP
u32 BL_ColorGetTexOfs(BLColorKey key);
#endif
void BL_ColorWrite(BLColorKey key);
void BL_ColorWriteEx(BLColorKey key, u32 idx, u8 ofs, u32 size);

#endif
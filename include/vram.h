#ifndef VRAM_H
#define VRAM_H

#include <nitro/types.h>
#include "display.h"
#include "gfxheap.h"

#ifdef SYS_BBP
#define BL_VramInit BBP_VramInit
#define BL_VramClear BBP_VramClear
#define BL_VramKillMain BBP_VramKillMain
#define BL_VramKillSub BBP_VramKillSub
#define BL_VramImageInit BBP_VramImageInit
#define BL_VramImageAlloc BBP_VramImageAlloc
#define BL_VramImageFree BBP_VramImageFree
#define BL_VramImageGetOfs BBP_VramImageGetOfs
#endif

typedef struct {
    u32 display : 1;
    u32 useCnt : 16;
    BLGfxAlloc *alloc;
} BLVramImageKey;

typedef struct {
    u16 useCnt;
    BLGfxAlloc *alloc;
} BLVramTexKey;

void BL_VramInit(void);
void BL_VramClear(void);
void BL_VramKillMain(void);
void BL_VramKillSub(void);
void BL_VramImageInit(void);
BOOL BL_VramImageAlloc(BLVramImageKey *key, int screen, u32 size);
void BL_VramImageFree(BLVramImageKey key);
#ifndef SYS_BBP
BOOL BL_VramTexAlloc(BLVramTexKey *key, u32 size);
void BL_VramTexFree(BLVramTexKey key);
#endif
u32 BL_VramImageGetOfs(BLVramImageKey key);


static inline u32 BL_VramTexGetOfs(BLVramTexKey key)
{
    return key.alloc->ofs;
}

#endif
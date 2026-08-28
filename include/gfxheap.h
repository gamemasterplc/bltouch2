#ifndef GFXHEAP_H
#define GFXHEAP_H

#include <nitro/types.h>

#ifdef SYS_BBP
#define BL_GfxHeapCreate BBP_GfxHeapCreate
#define BL_GfxHeapAlloc BBP_GfxHeapAlloc
#define BL_GfxHeapFree BBP_GfxHeapFree
#define BL_GfxHeapKill BBP_GfxHeapKill
#endif

typedef struct BLGfxAlloc_s BLGfxAlloc;

struct BLGfxAlloc_s {
    u32 ofs;
    u32 size;
    BLGfxAlloc *prev;
    BLGfxAlloc *next;
};

typedef struct BLGfxHeap_s {
    u32 size;
    u16 mask;
    BLGfxAlloc *head;
    BLGfxAlloc *tail;
} BLGfxHeap;

void BL_GfxHeapCreate(BLGfxHeap *heap, u32 size, u32 numBit);
BLGfxAlloc *BL_GfxHeapAlloc(BLGfxHeap *heap, u32 size);
void BL_GfxHeapFree(BLGfxHeap *heap, BLGfxAlloc *alloc);
void BL_GfxHeapKill(BLGfxHeap *heap);

#endif
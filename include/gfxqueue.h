#ifndef GFXQUEUE_H
#define GFXQUEUE_H

#include <nitro/types.h>

typedef void (*BLGfxQueueFunc)(void *ctx);

typedef enum {
    BL_GFX_QUEUE_VBLANK,
    BL_GFX_QUEUE_MAIN,
    BL_GFX_QUEUE_MAX
} BLGfxQueueId;

typedef struct BLGfxQueueEntry_s BLGfxQueueEntry;
struct BLGfxQueueEntry_s {
    BLGfxQueueFunc func;
    BLGfxQueueEntry *prev;
    BLGfxQueueEntry *next;
    void *ctx;
    u16 prio;
    u16 queueId;
};

#ifdef SYS_BBP
#define BL_GfxQueueInit BBP_GfxQueueInit
#define BL_GfxQueueCreate BBP_GfxQueueCreate
#define BL_GfxQueueExec BBP_GfxQueueExec
#define BL_GfxQueueClose BBP_GfxQueueClose
#define BL_GfxQueueAdd BBP_GfxQueueAdd
#define BL_GfxQueueRemove BBP_GfxQueueRemove
#define BL_GfxQueueRemoveCur BBP_GfxQueueRemoveCur
#endif

void BL_GfxQueueInit(void);
void BL_GfxQueueCreate(void);
void BL_GfxQueueExec(BLGfxQueueId queueId);
void BL_GfxQueueClose(void);
BLGfxQueueEntry *BL_GfxQueueAdd(BLGfxQueueFunc func, BLGfxQueueId queueId, u16 prio, void *ctx);
void BL_GfxQueueRemove(BLGfxQueueEntry *entry);
void BL_GfxQueueRemoveCur(void);

#endif
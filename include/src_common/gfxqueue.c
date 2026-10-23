#include "gfxqueue.h"
#include <nitro.h>
#include "memory.h"


static BLGfxQueueEntry *CurGfxEntry;

static struct {
    BLGfxQueueEntry *head;
    BLGfxQueueEntry *tail;
} GfxQueue[BL_GFX_QUEUE_MAX];

void BL_GfxQueueInit(void)
{
    for(int i=0; i<BL_GFX_QUEUE_MAX; i++) {
        MI_CpuFill8(&GfxQueue[i], 0, sizeof(GfxQueue[0]));
    }
}

void BL_GfxQueueCreate(void)
{
    BOOL irqPrev;
    
    CurGfxEntry = NULL;
    #ifdef SYS_BBP
    irqPrev = OS_DisableIrq();
    #endif
    for(int i=0; i<BL_GFX_QUEUE_MAX; i++) {
        BLGfxQueueEntry *head = BL_MemCalloc(BL_MEM_TAG_SCENE_GFX_QUEUE, sizeof(BLGfxQueueEntry));
        BLGfxQueueEntry *tail = BL_MemCalloc(BL_MEM_TAG_SCENE_GFX_QUEUE, sizeof(BLGfxQueueEntry));
        #ifndef SYS_BBP
        OS_DisableIrq();
        #endif
        head->prio = 0;
        head->prev = NULL;
        head->next = tail;
        head->func = NULL;
        tail->prio = 0xFFFF;
        tail->prev = head;
        tail->next = NULL;
        tail->func = NULL;
        GfxQueue[i].head = head;
        GfxQueue[i].tail = tail;
        #ifndef SYS_BBP
        OS_EnableIrq();
        #endif
    }
    #ifdef SYS_BBP
    OS_RestoreIrq(irqPrev);
    #endif
}

void BL_GfxQueueExec(BLGfxQueueId queueId)
{
    BLGfxQueueEntry *head = GfxQueue[queueId].head;
    BLGfxQueueEntry *tail = GfxQueue[queueId].tail;
    if(!head || !tail) {
        return;
    } else {
        BLGfxQueueEntry *cur;
        BLGfxQueueEntry *next;
        
        BLGfxQueueEntry *old = CurGfxEntry;
        for(cur=head->next; cur != tail; cur=next) {
            next = cur->next;
            CurGfxEntry = cur;
            if(cur->func) {
                cur->func(cur->ctx);
            }
        }
        CurGfxEntry = old;
    }
}

void BL_GfxQueueClose(void)
{
    BOOL irqPrev;
    
    BLGfxQueueEntry *cur;
    BLGfxQueueEntry *head;
    BLGfxQueueEntry *tail;
    BLGfxQueueEntry *next;
    int i;
    #ifdef SYS_BBP
    irqPrev = OS_DisableIrq();
    #endif
    for(i=0; i<BL_GFX_QUEUE_MAX; i++) {
        
        head = GfxQueue[i].head;
        tail = GfxQueue[i].tail;
        
        
        #ifndef SYS_BBP
        OS_DisableIrq();
        #endif
        for(cur=head->next; cur!=tail; cur=next) {
            next = cur->next;
            BL_GfxQueueRemove(cur);
        }
        BL_MemFree(head);
        BL_MemFree(tail);
        
        #ifndef SYS_BBP
        OS_EnableIrq();
        #endif
        MI_CpuFill8(&GfxQueue[i], 0, sizeof(GfxQueue[0]));
    }
    #ifdef SYS_BBP
    OS_RestoreIrq(irqPrev);
    #endif
}

static BLGfxQueueEntry *GetQueueInsertPos(u16 prio, BLGfxQueueId queueId);

BLGfxQueueEntry *BL_GfxQueueAdd(BLGfxQueueFunc func, BLGfxQueueId queueId, u16 prio, void *ctx)
{
    
    BLGfxQueueEntry *entry;
    BLGfxQueueEntry *next;
    BOOL irqPrev;
    #ifdef SYS_BBP
    irqPrev = OS_DisableIrq();
    #endif
    entry = BL_MemCalloc(BL_MEM_TAG_SCENE_GFX_QUEUE, sizeof(BLGfxQueueEntry));
    #ifndef SYS_BBP
    OS_DisableIrq();
    #endif
    next = GetQueueInsertPos(prio, queueId);
    entry->func = func;
    entry->queueId = queueId;
    entry->prio = prio;
    
    entry->ctx = ctx;
    entry->prev = next->prev;
    entry->next = next;
    next->prev->next = entry;
    next->prev = entry;
    
    #ifndef SYS_BBP
    OS_EnableIrq();
    #else
    OS_RestoreIrq(irqPrev);
    #endif
    return entry;
}

void BL_GfxQueueRemove(BLGfxQueueEntry *entry)
{
    BOOL irqPrev;
    #ifndef SYS_BBP
    OS_DisableIrq();
    #else
    irqPrev = OS_DisableIrq();
    #endif
    BLGfxQueueEntry *prev = entry->prev;
    BLGfxQueueEntry *next = entry->next;
    prev->next = next;
    entry->next->prev = entry->prev;
    #ifndef SYS_BBP
    OS_EnableIrq();
    #endif
    BL_MemFree(entry);
    #ifdef SYS_BBP
    OS_RestoreIrq(irqPrev);
    #endif
}

void BL_GfxQueueRemoveCur(void)
{
    BL_GfxQueueRemove(CurGfxEntry);
}

static BLGfxQueueEntry *GetQueueInsertPos(u16 prio, BLGfxQueueId queueId)
{
    BLGfxQueueEntry *head = GfxQueue[queueId].head;
    BLGfxQueueEntry *tail = GfxQueue[queueId].tail;
    for(BLGfxQueueEntry *cur=head->next; cur!=tail; cur=cur->next) {
        if(cur->prio > prio) {
            return cur;
        }
    }
    return tail;
}
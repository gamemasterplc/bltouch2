#include "gfxheap.h"
#include "memory.h"

static inline BLGfxAlloc *GfxAlloc()
{
    BLGfxAlloc *node = BL_MemCalloc(BL_MEM_TAG_SCENE_GFX_HEAP, sizeof(BLGfxAlloc));
    node->ofs = 0;
    node->size = 0;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void BL_GfxHeapCreate(BLGfxHeap *heap, u32 size, u32 numBit)
{
    BLGfxAlloc *head;
    heap->size = size;
    heap->mask = (1 << numBit)-1;
    heap->tail = NULL;
    head = GfxAlloc();
    head->size = size;
    heap->head = head;
    
}

BLGfxAlloc *BL_GfxHeapAlloc(BLGfxHeap *heap, u32 size)
{
    BLGfxAlloc *cur;
    BLGfxAlloc *prev;
    
    u32 maxSize;
    u32 ofs;
    u32 allocSize;
    BLGfxAlloc *ret;
    
    allocSize = (size+heap->mask) & ~heap->mask;
    prev = NULL;
    
    for(cur=heap->head; cur; cur=cur->next) {
        if(cur->size < allocSize) {
            continue;
        } else if(cur->size == allocSize) {
            prev = cur;
            break;
        } else if(prev == NULL || maxSize > (cur->size-allocSize)) {
            prev = cur;
            maxSize = cur->size-allocSize;
        }
    }
    if(!prev) {
        return NULL;
    }
    ofs = prev->ofs;
    prev->ofs += allocSize;
    prev->size -= allocSize;
    if(prev->size == 0) {
        if(prev->prev) {
            prev->prev->next = prev->next;
        }
        if(prev->next) {
            prev->next->prev = prev->prev;
        }
        if(heap->head == prev) {
            heap->head = prev->next;
        }
        BL_MemFree(prev);
    }
    ret = GfxAlloc();
    ret->ofs = ofs;
    ret->size = allocSize;
    if(heap->tail) {
        heap->tail->prev = ret;
    }
    ret->next = heap->tail;
    heap->tail = ret;
    return ret;
}

void BL_GfxHeapFree(BLGfxHeap *heap, BLGfxAlloc *alloc)
{
    BLGfxAlloc *cur;
    BLGfxAlloc *prev;
    u32 ofs;
    u32 size;
    
    if(alloc->prev) {
        alloc->prev->next = alloc->next;
    }
    if(alloc->next) {
        alloc->next->prev = alloc->prev;
    }
    if(alloc == heap->tail) {
        heap->tail = alloc->next;
    }

    prev = NULL;
    
    for(cur=heap->head; cur; cur=cur->next) {
        if(cur->ofs >= alloc->ofs) {
            break;
        }
        prev = cur;
    }
    if(prev && alloc->ofs == prev->ofs+prev->size) {
        prev->size += alloc->size;
        if(alloc->prev) {
            alloc->prev->next = alloc->next;
        }
        if(alloc->next) {
            alloc->next->prev = alloc->prev;
        }
        if(heap->head == alloc) {
            heap->head = alloc->next;
        }

        BL_MemFree(alloc);
        if(cur && cur->ofs == prev->ofs+prev->size) {
            prev->size += cur->size;
            if(cur->prev) {
                cur->prev->next = cur->next;
            }
            if(cur->next) {
                cur->next->prev = cur->prev;
            }
            if(heap->head == cur) {
                heap->head = cur->next;
            }
            BL_MemFree(cur);
        }
    } else if(cur && cur->ofs == alloc->ofs+alloc->size) {
        cur->ofs = alloc->ofs;
        cur->size += alloc->size;
        if(alloc->prev) {
            alloc->prev->next = alloc->next;
        }
        if(alloc->next) {
            alloc->next->prev = alloc->prev;
        }
        if(heap->head == alloc) {
            heap->head = alloc->next;
        }

        BL_MemFree(alloc);
    } else {
        alloc->prev = prev;
        alloc->next = cur;
        if(cur) {
            cur->prev = alloc;
        }
        if(prev) {
            prev->next = alloc;
        } else {
            heap->head = alloc;
        }
    }
    
}

void BL_GfxHeapKill(BLGfxHeap *heap)
{
    BLGfxAlloc *cur;
    BLGfxAlloc *next;
    for(cur=heap->head; cur;) {
        next = cur->next;
        BL_MemFree(cur);
        cur = next;
    }
    heap->head = NULL;
    for(cur=heap->tail; cur;) {
        next = cur->next;
        BL_MemFree(cur);
        cur = next;
    }
    heap->tail = NULL;
}
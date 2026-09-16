#include "object.h"
#include <nitro.h>
#include "memory.h"

BLObj *BL_ObjCur;

static struct {
    BLObj *head;
    BLObj *tail;
    u32 pauseFlag;
} ObjList;

static BLObj *NewObj(void);
static void FreeObj(BLObj *obj);
static void InsertObj(BLObj *obj);
static BLObj *KillObj(BLObj *obj);
static BLObj *SortObj(BLObj *obj);

void BL_ObjInit(void)
{
    BLObj *head;
    BLObj *tail;
    MI_CpuFill16(0, &ObjList, sizeof(ObjList));
    head = NewObj();
    tail = NewObj();
    head->prev = NULL;
    head->next = tail;
    tail->prev = head;
    tail->next = NULL;
    ObjList.head = head;
    ObjList.tail = tail;
}

void BL_ObjExec(void)
{
    BLObj *obj;
    BLObj *head;
    BLObj *tail;
    BLObj *next;
    
    head = ObjList.head;
    tail = ObjList.tail;
    #ifdef SYS_BBP
    for(obj=head->next; obj != tail; obj=next) {
        next = obj->next;
        BL_ObjCur = obj;
        if(obj->kill) {
            obj = KillObj(obj);
        } else if(obj->needSort) {
            obj = SortObj(obj);
        }
    }
    #else
    for(obj=head->next; obj != tail; obj=obj->next) {
        BL_ObjCur = obj;
        if(obj->kill) {
            obj = KillObj(obj);
        } else if(obj->needSort) {
            obj = SortObj(obj);
        }
    }
    #endif
    #ifdef SYS_BBP
    for(obj=head->next; obj != tail; obj=next) {
        next = obj->next;
        if(obj->lock != TRUE &&  (obj->pauseFlag & ObjList.pauseFlag) == 0) {
            if(obj->delay != 0) {
                if(obj->delay != 0xFFFF) {
                    obj->delay--;
                }
            } else {
                BL_ObjCur = obj;
                obj->func();
            }
        }
    }
    #else
    for(obj=head->next; obj != tail; obj=obj->next) {
        if(obj->lock != TRUE &&  (obj->pauseFlag & ObjList.pauseFlag) == 0) {
            if(obj->delay != 0) {
                if(obj->delay != 0xFFFF) {
                    obj->delay--;
                }
            } else {
                BL_ObjCur = obj;
                obj->func();
            }
        }
    }
    #endif
}

BLObj *BL_ObjCreate(BLObjFunc func, u16 prio, u32 pauseFlag, void *data, BLObjFunc destroy)
{
    BLObj *obj;
    if(func == NULL) {
        func = BL_ObjNullExec;
    }
    obj = NewObj();
    if(obj) {
        obj->valid = TRUE;
        obj->prio = prio;
        obj->pauseFlag = pauseFlag;
        obj->func = func;
        obj->data = data;
        obj->destroy = destroy;
        InsertObj(obj);
    }
    return obj;
}

void BL_ObjCurKill(void)
{
    BL_ObjKill(BL_ObjCur);
}

void BL_ObjKill(BLObj *obj)
{
    BLObj *iter;
    BLObj *next;
    if(!obj) {
        return;
    }
    obj->lock = TRUE;
    obj->kill = TRUE;
    obj->delay = 0;
    obj->pauseFlag = 0;
    #ifdef SYS_BBP
    for(iter=obj->childFirst; iter; iter=next) {
        next = iter->childNext;
        BL_ObjKill(iter);
    }
    #else
    for(iter=obj->childFirst; iter; iter=iter->childNext) {
        BL_ObjKill(iter);
    }
    #endif
}

#ifndef SYS_BBP
BLObj *BL_ObjCurCreateChild(BLObjFunc func, u16 prio, u32 pauseFlag, void *data, BLObjFunc destroy)
{
    return BL_ObjCreateChild(BL_ObjCur, func, prio, pauseFlag, data, destroy);
}
#endif

BLObj *BL_ObjCreateChild(BLObj *parent, BLObjFunc func, u16 prio, u32 pauseFlag, void *data, BLObjFunc destroy)
{
    BLObj *obj;
    BLObj *prev;
    BLObj *iter;
    obj = NULL;
    if(parent && !parent->lock) {
        obj = BL_ObjCreate(func, prio, pauseFlag, data, destroy);
        if(!obj) {
            return obj;
        }
        if(!parent->childFirst) {
            parent->childFirst = obj;
        } else {
            prev = parent->childFirst;
            for(iter=prev->childNext; iter; iter=iter->childNext) {
                prev = iter;
            }
            prev->childNext = obj;
            obj->childPrev = prev;
            obj->childNext = NULL;
        }
        obj->parent = parent;
    }
    return obj;
}

#ifdef SYS_BBP
void BL_ObjKillChild(BLObj *obj)
{
    BLObj *iter;
    if(!obj) {
        return;
    }
    if(obj->lock) {
        return;
    }
    
    for(iter=obj->childFirst; iter; iter=iter->childNext) {
        BL_ObjKill(iter);
    }
}
#endif

void BL_ObjCurSetFunc(BLObjFunc func)
{
    BL_ObjSetFunc(BL_ObjCur, func);
}

void BL_ObjSetFunc(BLObj *obj, BLObjFunc func)
{
    if(func == NULL) {
        func = BL_ObjNullExec;
    }
    if(!obj) {
        return;
    }
    if(!obj->lock) {
        obj->func = func;
    }
}

#ifndef SYS_BBP

void BL_ObjCurSetPrio(u16 prio)
{
    BL_ObjSetPrio(BL_ObjCur, prio);
}

void BL_ObjSetPrio(BLObj *obj, u16 prio)
{
    if(!obj) {
        return;
    }
    if(obj->lock) {
        return;
    }
    if(obj->prio == prio) {
        return;
    }
    obj->prioNew = prio;
    obj->needSort = TRUE;
}

void BL_ObjSetPauseFlag(u32 flag)
{
    ObjList.pauseFlag |= flag;
}
#endif

void BL_ObjResetPauseFlag(u32 flag)
{
    ObjList.pauseFlag &= ~flag;
}

static BLObj *NewObj(void)
{
    return BL_MemCalloc(BL_MEM_TAG_OBJECT, sizeof(BLObj));
}

static void FreeObj(BLObj *obj)
{
    BL_MemFree(obj);
}

static void InsertObj(BLObj *obj)
{
    BLObj *head;
    BLObj *tail;
    BLObj *iter;
    BLObj *prev;
    head = ObjList.head;
    tail = ObjList.tail;
    for(iter=head->next; iter != tail; iter=iter->next) {
        if(iter->prio > obj->prio) {
            break;
        }
    }
    obj->next = iter;
    prev = iter->prev;
    obj->prev = prev;
    prev->next = obj;
    obj->next->prev = obj;
}

static BLObj *KillObj(BLObj *obj)
{
    BLObj *prev;
    BLObj *iter;
    prev = obj->prev;
    prev->next = obj->next;
    obj->next->prev = prev;
    if(obj->parent) {
        
        if(!obj->childPrev) {
            obj->parent->childFirst = obj->childNext;
            if(obj->parent->childFirst) {
                obj->childNext->childPrev = NULL;
            }
        } else {
            obj->childPrev->childNext = obj->childNext;
            if(obj->childPrev->childNext) {
                obj->childNext->childPrev = obj->childPrev;
            }
        }
    } else {
        if(obj->childPrev) {
            obj->childPrev->childNext = obj->childNext;
            if(obj->childPrev->childNext) {
                obj->childNext->childPrev = obj->childPrev;
            }
        } else {
            if(obj->childNext) {
                obj->childNext->childPrev = obj->childPrev;
            }
        }
    }
    for(iter=obj->childFirst; iter; iter=iter->childNext) {
        iter->parent = NULL;
    }

    if(obj->destroy) {
        obj->destroy();
    }
    FreeObj(obj);
    return prev;
}

static BLObj *SortObj(BLObj *obj)
{
    BLObj *next = obj->next;
    if(obj->prio != obj->prioNew) {
        obj->prev->next = next;
        next->prev = obj->prev;
        obj->prio = obj->prioNew;
        InsertObj(obj);
    }
    return next->prev;
}

void BL_ObjNullExec(void)
{
    
}
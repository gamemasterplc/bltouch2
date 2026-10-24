#include "tone.h"
#include "memory.h"
#include <nitro.h>

static struct {
    BLTone *head;
    BLTone *tail;
    u32 pauseFlag;
    BLToneId nextId;
} ToneList;

void BL_ToneInit(void)
{
    BLTone *head;
    BLTone *tail;
    MI_CpuFill32(0, &ToneList, sizeof(ToneList));
    head = BL_MemCalloc(BL_MEM_TAG_SCENE_TONE, sizeof(BLTone));
    tail = BL_MemCalloc(BL_MEM_TAG_SCENE_TONE, sizeof(BLTone));
    head->prev = NULL;
    head->next = tail;
    head->prio = 0;
    tail->prev = head;
    tail->next = NULL;
    tail->prio = 0xFFFF;
    ToneList.nextId = 1;
    ToneList.head = head;
    ToneList.tail = tail;
    ToneList.pauseFlag = 0;
}

static void RemoveTone(BLTone *tone);

void BL_ToneClose(void)
{
    BLTone *head = ToneList.head;
    BLTone *tail = ToneList.tail;
    BLTone *next;
    BLTone *cur;
    for(cur=head->next; cur != ToneList.tail; cur=next) {
        next = cur->next;
        RemoveTone(cur);
    }
    BL_MemFree(ToneList.head);
    BL_MemFree(ToneList.tail);
    MI_CpuFill32(0, &ToneList, sizeof(ToneList));
}

#ifndef SYS_BBP
void BL_ToneSetPauseFlag(u32 flag)
{
    ToneList.pauseFlag |= flag;
}

void BL_ToneResetPauseFlag(u32 flag)
{
    ToneList.pauseFlag &= ~flag;
}
#endif

static inline BLResToneAnim *GetToneAnim(BLResTone *res, int anim)
{
    u32 *animOfsTbl;
    if(anim < res->numAnim) {
        animOfsTbl = (u32 *)((u8 *)res+res->animOfsTblOfs);            
        return (BLResToneAnim *)(((u8 *)res+res->animBaseOfs)+animOfsTbl[anim]);
    } else {
        return NULL;
    }
}

static inline BLResToneFrame *GetToneFrame(BLResToneAnim *anim, int frame)
{
    if(frame >= anim->numFrame) {
        return NULL;
    } else {
        return (BLResToneFrame *)(anim->data+(anim->dataPitch*frame));
    }
}


BLToneId BL_ToneCreate(BLResTone *res, BLTonePlttType plttType, u32 plttOfs, u16 prio)
{
    return BL_ToneCreateFlag(res, plttType, plttOfs, prio, 0);
}

static void SetPlttTransfer(BLToneTransfer *transfer, BLTonePlttType plttType, u32 plttOfs, u32 colorIdx);
static void AddTransferQueue(BLToneTransfer *transfer);
static void InsertTone(BLTone *tone, u16 prio);
static void RemoveTone(BLTone *tone);

BLToneId BL_ToneCreateFlag(BLResTone *res, BLTonePlttType plttType, u32 plttOfs, u16 prio, u32 pauseFlag)
{
    BLTone *tone; //r4
    BLToneTransfer *transfer; //r5
    BLToneId nextId; //r7
    BLToneId firstId; //r5
    int numAnim; //r6
    int i; //r7
    BLResToneAnim *anim; //r2
    BLResToneFrame *frame; //r0
    
    if(!res) {
        return BL_TONEID_INVALID;
    }
    numAnim = res->numAnim;
    if(numAnim == 0) {
        return BL_TONEID_INVALID;
    }
    tone = BL_MemCalloc(BL_MEM_TAG_SCENE_TONE, sizeof(BLTone));
    nextId = firstId = ToneList.nextId;
    
    while(BL_ToneGetData(nextId)) {
        nextId++;
        if(nextId == BL_TONEID_INVALID) {
            nextId = 1;
        }
        if(nextId == firstId) {
            return BL_TONEID_INVALID;
        }
    }
    tone->id = nextId;
    nextId++;
    if(nextId == BL_TONEID_INVALID) {
        nextId = 1;
    }
    ToneList.nextId = nextId;
    tone->transfer = transfer = BL_MemCalloc(BL_MEM_TAG_SCENE_TONE, sizeof(BLToneTransfer));
    for(i=0; i<numAnim; i++) {
        anim = GetToneAnim(res, i);
        frame = GetToneFrame(anim, 0);
        transfer->curAnim = anim;
        transfer->frame = 0;
        
        transfer->time = frame->delay << 8;
        transfer->src = frame->data;
        transfer->srcSize = anim->dataSize;
        SetPlttTransfer(transfer, plttType, plttOfs, anim->colorIdx);
        AddTransferQueue(transfer);
        if(i+1 < numAnim) {
            BLToneTransfer *newTransfer = BL_MemCalloc(BL_MEM_TAG_SCENE_TONE, sizeof(BLToneTransfer));
            transfer->next = newTransfer;
            transfer = newTransfer;
        }
    }
    
    transfer->next = NULL;
    InsertTone(tone, prio);
    
    tone->numAnim = numAnim;
    tone->prio = prio;
    tone->pauseFlag = pauseFlag;
    tone->speed = 1 << 8;
    tone->firstF = TRUE;
    return tone->id;
}

void BL_ToneKill(BLToneId id)
{
    BLTone *tone = BL_ToneGetData(id);
    if(!tone) {
        return;
    }
    RemoveTone(tone);
}

BLTone *BL_ToneGetData(BLToneId id)
{
    BLTone *head = ToneList.head;
    BLTone *tail = ToneList.tail;
    BLTone *cur;
    for(cur=head->next; cur != tail; cur=cur->next) {
        if(cur->id == id) {
            return cur;
        }
    }
    return NULL;
}

#ifndef SYS_BBP

void BL_ToneSetSpeed(BLToneId id, u16 speed)
{
    BLTone *tone = BL_ToneGetData(id);
    if(!tone) {
        return;
    }
    tone->speed = speed;
}
#endif

static void ExecTone(BLTone *tone);

void BL_ToneExec(void)
{
    BLTone *head = ToneList.head;
    BLTone *tail = ToneList.tail;
    BLTone *next;
    BLTone *cur;
    for(cur=head->next; cur != tail; cur=next) {
        next = cur->next;
        if(!cur->pause && !(cur->pauseFlag & ToneList.pauseFlag)) {
            ExecTone(cur);
        }
    }
}

static void InsertTone(BLTone *tone, u16 prio)
{
    BLTone *head = ToneList.head;
    BLTone *tail = ToneList.tail;
    BLTone *cur;
    for(cur=head->next; cur != tail; cur=cur->next) {
        if(prio > cur->prio) {
            break;
        }
    }
    tone->prev = cur->prev;
    tone->next = cur;
    cur->prev->next = tone;
    cur->prev = tone;
}

static void UnlinkTone(BLTone *tone)
{
    BLTone *prev = tone->prev;
    BLTone *next = tone->next;
    prev->next = next;
    tone->next->prev = tone->prev;
}

static void RemoveTone(BLTone *tone)
{
    BLToneTransfer *transfer;
    BLToneTransfer *transferNext;
    for(transfer=tone->transfer; transfer; transfer=transferNext) {
        transferNext = transfer->next;
        if(transfer->queueEntry) {
            BL_GfxQueueRemove(transfer->queueEntry);
        }
        BL_MemFree(transfer);
    }
    UnlinkTone(tone);
    BL_MemFree(tone);
}

static void ExecTone(BLTone *tone)
{
    BLToneTransfer *transfer; //r8
    int numDone; //r9
    BLResToneAnim *curAnim; //r1
    int frameNo; //r2
    BLResToneFrame *frame; //r3
    numDone = 0;
    for(transfer=tone->transfer; transfer; transfer=transfer->next) {
        if(!transfer->endF) {
            transfer->time -= tone->speed;
            if(transfer->time <= 0) {
                curAnim = transfer->curAnim;
                frameNo = transfer->frame;
                frame = GetToneFrame(curAnim, frameNo+1);
                if(!frame ||  frame->delayRaw == 0 || frame->delayRaw == 0xFFFE) {
                    if(frameNo == 0 || tone->loopMode == 2) {
                        numDone++;
                        transfer->endF = TRUE;
                    }
                    transfer->frame = 0;
                } else if(frame->delayRaw == 0xFFFF) {
                    if(tone->loopMode == 1) {
                        transfer->frame = 0;
                    } else {
                        numDone++;
                        transfer->endF = TRUE;
                    }
                } else {
                    transfer->frame++;
                }
                if(!transfer->endF) {
                    
                    frame = GetToneFrame(curAnim, transfer->frame);
                    
                    transfer->time = frame->delay << 8;
                    transfer->src = frame->data;
                    transfer->srcSize = curAnim->dataSize;
                    
                    AddTransferQueue(transfer);
                }
            }
        } else {
            numDone++;
        }
    }
    if(tone->firstF) {
        tone->firstF = FALSE;
        return;
    }
    if(tone->numAnim != numDone && tone->autoRemove != TRUE) {
        return;
    }
    RemoveTone(tone);
}

static void TransferPlttOBJ(void *ctx);
static void TransferPlttOBJSub(void *ctx);
static void TransferPlttOBJExt(void *ctx);
static void TransferPlttOBJExtSub(void *ctx);
static void TransferPlttBG(void *ctx);
static void TransferPlttBGSub(void *ctx);
static void TransferPlttBGExt(void *ctx);
static void TransferPlttBGExtSub(void *ctx);
static void TransferPlttTex(void *ctx);

static void SetPlttTransfer(BLToneTransfer *transfer, BLTonePlttType plttType, u32 plttIdx, u32 colorIdx)
{
    switch(plttType) {
        case BL_TONE_PLTT_OBJ:
            transfer->transferFunc = TransferPlttOBJ;
            transfer->destOfs = (plttIdx*32)+(colorIdx*2);
            break;
        
        case BL_TONE_PLTT_OBJ_EXT:
            transfer->transferFunc = TransferPlttOBJExt;
            transfer->destOfs = (plttIdx*512)+(colorIdx*2);
            break;
        
        case BL_TONE_PLTT_OBJ_SUB:
            transfer->transferFunc = TransferPlttOBJSub;
            transfer->destOfs = (plttIdx*32)+(colorIdx*2);
            break;
        
        case BL_TONE_PLTT_OBJ_EXT_SUB:
            transfer->transferFunc = TransferPlttOBJExtSub;
            transfer->destOfs = (plttIdx*512)+(colorIdx*2);
            break;
           
        case BL_TONE_PLTT_BG:
            transfer->transferFunc = TransferPlttBG;
            transfer->destOfs = (plttIdx*32)+(colorIdx*2);
            break;
        
        case BL_TONE_PLTT_BG0_EXT:
            transfer->transferFunc = TransferPlttBGExt;
            transfer->destOfs = (plttIdx*512)+(colorIdx*2)+0;
            break;
        
        case BL_TONE_PLTT_BG1_EXT:
            transfer->transferFunc = TransferPlttBGExt;
            transfer->destOfs = (plttIdx*512)+(colorIdx*2)+0x2000;
            break;
        
        case BL_TONE_PLTT_BG2_EXT:
            transfer->transferFunc = TransferPlttBGExt;
            transfer->destOfs = (plttIdx*512)+(colorIdx*2)+0x4000;
            break;
        
        case BL_TONE_PLTT_BG3_EXT:
            transfer->transferFunc = TransferPlttBGExt;
            transfer->destOfs = (plttIdx*512)+(colorIdx*2)+0x6000;
            break;
        
        case BL_TONE_PLTT_BG_SUB:
            transfer->transferFunc = TransferPlttBGSub;
            transfer->destOfs = (plttIdx*32)+(colorIdx*2);
            break;
        
        case BL_TONE_PLTT_BG0_EXT_SUB:
            transfer->transferFunc = TransferPlttBGExtSub;
            transfer->destOfs = (plttIdx*32)+(colorIdx*2)+0;
            break;
        
        case BL_TONE_PLTT_BG1_EXT_SUB:
            transfer->transferFunc = TransferPlttBGExtSub;
            transfer->destOfs = (plttIdx*32)+(colorIdx*2)+0x2000;
            break;
        
        case BL_TONE_PLTT_BG2_EXT_SUB:
            transfer->transferFunc = TransferPlttBGExtSub;
            transfer->destOfs = (plttIdx*32)+(colorIdx*2)+0x4000;
            break;
        
        case BL_TONE_PLTT_BG3_EXT_SUB:
            transfer->transferFunc = TransferPlttBGExtSub;
            transfer->destOfs = (plttIdx*32)+(colorIdx*2)+0x6000;
            break;
        
        case BL_TONE_PLTT_TEX:
            transfer->transferFunc = TransferPlttTex;
            transfer->destOfs = plttIdx+(colorIdx*2);
            break;
    }
}

static void AddTransferQueue(BLToneTransfer *transfer)
{
    transfer->queueEntry = BL_GfxQueueAdd(transfer->transferFunc, BL_GFX_QUEUE_MAIN, -1, transfer);
}

static void TransferPlttOBJ(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    MI_CpuCopy16(transfer->src, HW_PLTT+0x200+transfer->destOfs, transfer->srcSize);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}

static void TransferPlttOBJSub(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    MI_CpuCopy16(transfer->src, HW_DB_PLTT+0x200+transfer->destOfs, transfer->srcSize);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}

static void TransferPlttOBJExt(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    GXVRam vram;
    void *dst;
    dst = NULL;
    vram = GX_ResetBankForOBJExtPltt();
    if(vram != GX_VRAM_F) {
        if(vram == GX_VRAM_G) {
            dst = HW_LCDC_VRAM_G;
        }
    } else {
        dst = HW_LCDC_VRAM_F;
    }
    if(dst) {
        MI_CpuCopy16(transfer->src, dst+transfer->destOfs, transfer->srcSize);
    }
    GX_SetBankForOBJExtPltt(vram);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}

static void TransferPlttOBJExtSub(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    GXVRam vram;
    vram = GX_ResetBankForSubOBJExtPltt();
    if(vram) {
        MI_CpuCopy16(transfer->src, HW_LCDC_VRAM_I+transfer->destOfs, transfer->srcSize);
    }
    GX_SetBankForSubOBJExtPltt(vram);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}

static void TransferPlttBG(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    MI_CpuCopy16(transfer->src, HW_PLTT+transfer->destOfs, transfer->srcSize);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}

static void TransferPlttBGSub(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    MI_CpuCopy16(transfer->src, HW_DB_PLTT+transfer->destOfs, transfer->srcSize);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}

static void TransferPlttBGExt(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    GXVRam vram;
    
    void *dst;
    u32 ofs;
    dst = NULL;
    ofs = 0;
    vram = GX_ResetBankForBGExtPltt();
    switch(vram) {
        case GX_VRAM_E:
            dst = HW_LCDC_VRAM_E;
            break;

        case GX_VRAM_G:
            dst = HW_LCDC_VRAM_G;
            ofs = 0x4000;
            break;
            
        case GX_VRAM_F:
        case (GX_VRAM_F|GX_VRAM_G):
            dst = HW_LCDC_VRAM_F;
            ofs = 0;
            break;
    }
    if(dst) {
        MI_CpuCopy16(transfer->src, (dst+transfer->destOfs)-ofs, transfer->srcSize);
    }
    GX_SetBankForBGExtPltt(vram);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}

static void TransferPlttBGExtSub(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    GXVRam vram;

    vram = GX_ResetBankForSubBGExtPltt();
    if(vram) {
        MI_CpuCopy16(transfer->src, HW_LCDC_VRAM_H+transfer->destOfs, transfer->srcSize);
    }
    GX_SetBankForSubBGExtPltt(vram);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}

static void TransferPlttTex(void *ctx)
{
    BLToneTransfer *transfer = (BLToneTransfer *)ctx;
    GXVRam vram;
    void *dst;
    dst = NULL;
    vram = GX_ResetBankForTexPltt();
    if(vram & GX_VRAM_E) {
        dst = transfer->destOfs+HW_LCDC_VRAM_E;
    } else if(vram & GX_VRAM_F) {
        dst = transfer->destOfs+HW_LCDC_VRAM_F;
    } else if(vram & GX_VRAM_G) {
        dst = transfer->destOfs+HW_LCDC_VRAM_G;
    }
    if(dst) {
        MI_CpuCopy16(transfer->src, dst, transfer->srcSize);
    }
    GX_SetBankForTexPltt(vram);
    BL_GfxQueueRemoveCur();
    transfer->queueEntry = NULL;
}
#include "vram.h"
#include "gfxheap.h"
#include "memory.h"
#include <nnsys/gfx_transfer.h>
#include <nitro.h>


#define GFX_TRANSFER_MAX 256

typedef struct VramWork_s {
    BLGfxHeap heap[BL_DISPLAY_MAX]; //0x00
    BLGfxHeap heapTex; //0x20
    u16 useCnt[BL_DISPLAY_MAX]; //0x30
    u16 useCntTex; //0x34
    NNSGfxTransfer transferBuf[GFX_TRANSFER_MAX]; //0x38
} VramWork_t;

static VramWork_t *VramWork;

void BL_VramInit(void)
{
    VramWork = BL_MemCalloc(BL_MEM_TAG_SYSTEM, sizeof(VramWork_t));
    if(!VramWork) {
        return;
    }
    NNS_GfxTransferInit(&VramWork->transferBuf[0], GFX_TRANSFER_MAX);
}

void BL_VramClear(void)
{
    GX_DisableBankForBG();
    GX_DisableBankForOBJ();
    GX_DisableBankForBGExtPltt();
    GX_DisableBankForOBJExtPltt();
    GX_DisableBankForTex();
    GX_DisableBankForTexPltt();
    GX_DisableBankForClearImage();
    GX_DisableBankForSubBG();
    GX_DisableBankForSubOBJ();
    GX_DisableBankForSubBGExtPltt();
    GX_DisableBankForSubOBJExtPltt();
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MI_CpuClearFast((void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);	// Clear all LCDC space
    GX_DisableBankForLCDC();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);   		// clear OAM
	MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);     		// clear the standard palette

	MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);     // clear OAM
	MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);       // clear the standard palette
}

void BL_VramKillMain(void)
{
    GX_ResetBankForBG();
    GX_ResetBankForOBJ();
    GX_ResetBankForBGExtPltt();
    GX_ResetBankForOBJExtPltt();
    GX_ResetBankForTex();
    GX_ResetBankForTexPltt();
    GX_ResetBankForClearImage();
    MI_CpuFillFast((void *)HW_OAM, 192, HW_OAM_SIZE);   		// clear OAM
	MI_CpuClearFast((void *)HW_PLTT, HW_PLTT_SIZE);     		// clear the standard palette
    if(VramWork->useCnt[BL_DISPLAY_MAIN] != 0) {
        BL_GfxHeapKill(&VramWork->heap[BL_DISPLAY_MAIN]);
        VramWork->useCnt[BL_DISPLAY_MAIN] = 0;
    }
    if(VramWork->useCntTex != 0) {
        BL_GfxHeapKill(&VramWork->heapTex);
        VramWork->useCntTex = 0;
    }
    
}

void BL_VramKillSub(void)
{
    GX_ResetBankForSubBG();
    GX_ResetBankForSubOBJ();
    GX_ResetBankForSubBGExtPltt();
    GX_ResetBankForSubOBJExtPltt();
    MI_CpuFillFast((void *)HW_DB_OAM, 192, HW_DB_OAM_SIZE);     // clear OAM
	MI_CpuClearFast((void *)HW_DB_PLTT, HW_DB_PLTT_SIZE);       // clear the standard palette
    if(VramWork->useCnt[BL_DISPLAY_SUB] != 0) {
        BL_GfxHeapKill(&VramWork->heap[BL_DISPLAY_SUB]);
        VramWork->useCnt[BL_DISPLAY_SUB] = 0;
    }
}

void BL_VramImageInit(void)
{
    u32 bankSize;
    bankSize = GX_GetBankSizeForOBJ();
    if(bankSize != 0) {
        BL_GfxHeapCreate(&VramWork->heap[BL_DISPLAY_MAIN], bankSize, GX_GetOBJVRamCharShift()+5);
        VramWork->useCnt[BL_DISPLAY_MAIN]++;
        if(VramWork->useCnt[BL_DISPLAY_MAIN] == 0) {
            VramWork->useCnt[BL_DISPLAY_MAIN]++;
        }
    }
    bankSize = GX_GetBankSizeForSubOBJ();
    if(bankSize != 0) {
        BL_GfxHeapCreate(&VramWork->heap[BL_DISPLAY_SUB], bankSize, GXS_GetOBJVRamCharShift()+5);
        VramWork->useCnt[BL_DISPLAY_SUB]++;
        if(VramWork->useCnt[BL_DISPLAY_SUB] == 0) {
            VramWork->useCnt[BL_DISPLAY_SUB]++;
        }
    }
    bankSize = GX_GetBankSizeForTex();
    if(bankSize != 0) {
        BL_GfxHeapCreate(&VramWork->heapTex, bankSize, 5);
        VramWork->useCntTex++;
        if(VramWork->useCntTex == 0) {
            VramWork->useCntTex++;
        }
    }
}

BOOL BL_VramImageAlloc(BLVramImageKey *key, int display, u32 size)
{
    key->alloc = BL_GfxHeapAlloc(&VramWork->heap[display], size);
    if(!key->alloc) {
        return FALSE;
    }
    key->display = display;
    key->useCnt = VramWork->useCnt[display];
    return TRUE;
}

void BL_VramImageFree(BLVramImageKey key)
{
    if(VramWork->useCnt[key.display] != key.useCnt) {
        return;
    }
    if(key.alloc == NULL) {
        return;
    }
    BL_GfxHeapFree(&VramWork->heap[(u8)key.display], key.alloc);
}

#ifndef SYS_BBP

BOOL BL_VramTexAlloc(BLVramTexKey *key, u32 size)
{
    key->alloc = BL_GfxHeapAlloc(&VramWork->heapTex, size);
    if(!key->alloc) {
        return FALSE;
    }
    key->useCnt = VramWork->useCntTex;
    return TRUE;
}

void BL_VramTexFree(BLVramTexKey key)
{
    if(key.useCnt != VramWork->useCntTex) {
        return;
    }
    if(key.alloc == NULL) {
        return;
    }
    BL_GfxHeapFree(&VramWork->heapTex, key.alloc);
}

#endif

u32 BL_VramImageGetOfs(BLVramImageKey key)
{
    return key.alloc->ofs;
}

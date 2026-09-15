#include "color.h"
#include "memory.h"
#include "gfxheap.h"
#include <nitro/types.h>
#include "res_color.h"
#include <nitro.h>
#include <nnsys/gfx_transfer.h>

typedef struct ColorAlloc_s {
    u8 idx; //0x00
    u8 numRows; //0x01
    u16 useCnt; //0x02
    u32 fileID; //0x04
    u32 ofs; //0x08
    BLGfxAlloc *gfxAlloc; //0x0C
    u16 *buf; //0x10
} ColorAlloc;

#ifdef SYS_BBP
#define OBJ_PLTT_SIZE 0x1E0
#else
#define OBJ_PLTT_SIZE 0x200
#endif
#define OBJ_PLTT_SHIFT 5
#define OBJ_EXT_PLTT_SIZE 0x2000
#define OBJ_EXT_PLTT_SHIFT 9
#define BG_PLTT_SIZE 0x200
#define BG_PLTT_SHIFT 5
#define BG_EXT_PLTT_SIZE 0x2000
#define BG_EXT_PLTT_SHIFT 9

#define PLTT_MAX 16
#define TEX_PLTT_SIZE 0x4000
#define TEX_PLTT_SHIFT 5

#define TEX_PLTT_MAX 128

typedef struct ColorWork_s {
    ColorAlloc alloc[BL_DISPLAY_MAX][BL_COLOR_MAX_PLTT][PLTT_MAX]; //0x00
    BLGfxHeap heap[BL_DISPLAY_MAX][BL_COLOR_MAX_PLTT]; //0x1180
    ColorAlloc allocTex[TEX_PLTT_MAX]; //0x1260
    BLGfxHeap heapTex; //0x1C60
} ColorWork_t;

static ColorWork_t *ColorWork;

static const int ColorTransferTypeTbl[BL_DISPLAY_MAX][BL_COLOR_MAX_PLTT] = {
    {
        NNS_GFX_TRANSFER_OBJ_PLTT,
        NNS_GFX_TRANSFER_BG_PLTT,
        NNS_GFX_TRANSFER_OBJ_EXT_PLTT,
        NNS_GFX_TRANSFER_BG_EXT_PLTT,
        NNS_GFX_TRANSFER_BG_EXT_PLTT,
        NNS_GFX_TRANSFER_BG_EXT_PLTT,
        NNS_GFX_TRANSFER_BG_EXT_PLTT
    },
    {
        NNS_GFX_TRANSFER_SUB_OBJ_PLTT,
        NNS_GFX_TRANSFER_SUB_BG_PLTT,
        NNS_GFX_TRANSFER_SUB_OBJ_EXT_PLTT,
        NNS_GFX_TRANSFER_SUB_BG_EXT_PLTT,
        NNS_GFX_TRANSFER_SUB_BG_EXT_PLTT,
        NNS_GFX_TRANSFER_SUB_BG_EXT_PLTT,
        NNS_GFX_TRANSFER_SUB_BG_EXT_PLTT
    },
};

void BL_ColorInit(void)
{
    int i;
    
    ColorWork = BL_MemCalloc(BL_MEM_TAG_SCENE_GENERIC, sizeof(ColorWork_t));

    if(!ColorWork) {
        return;
    }
    for(i=0; i<BL_DISPLAY_MAX; i++) {
        BL_GfxHeapCreate(&ColorWork->heap[i][BL_COLOR_OBJ_PLTT], OBJ_PLTT_SIZE, OBJ_PLTT_SHIFT);
        BL_GfxHeapCreate(&ColorWork->heap[i][BL_COLOR_BG_PLTT], BG_PLTT_SIZE, BG_PLTT_SHIFT);
        BL_GfxHeapCreate(&ColorWork->heap[i][BL_COLOR_OBJ_EXT_PLTT], OBJ_EXT_PLTT_SIZE, OBJ_EXT_PLTT_SHIFT);
        BL_GfxHeapCreate(&ColorWork->heap[i][BL_COLOR_BG_EXT_PLTT_1], BG_EXT_PLTT_SIZE, BG_EXT_PLTT_SHIFT);
        BL_GfxHeapCreate(&ColorWork->heap[i][BL_COLOR_BG_EXT_PLTT_2], BG_EXT_PLTT_SIZE, BG_EXT_PLTT_SHIFT);
        BL_GfxHeapCreate(&ColorWork->heap[i][BL_COLOR_BG_EXT_PLTT_3], BG_EXT_PLTT_SIZE, BG_EXT_PLTT_SHIFT);
        BL_GfxHeapCreate(&ColorWork->heap[i][BL_COLOR_BG_EXT_PLTT_4], BG_EXT_PLTT_SIZE, BG_EXT_PLTT_SHIFT);
    }
    BL_GfxHeapCreate(&ColorWork->heapTex, TEX_PLTT_SIZE, TEX_PLTT_SHIFT);
}

void BL_ColorClose(void)
{
    int i, j;
    for(i=0; i<BL_DISPLAY_MAX; i++) {
        for(j=0; j<BL_COLOR_MAX_PLTT; j++) {
            BL_GfxHeapKill(&ColorWork->heap[i][j]);
        }
    }
    BL_GfxHeapKill(&ColorWork->heapTex);
    BL_MemFree(ColorWork);
    ColorWork = NULL;
}


typedef union {
    struct {
        u32 valid : 1;
        u32 isTex : 2;
        u32 display : 1;
        u32 pltt : 3;
        u32 idx : 13;
    };
    u32 raw;
} ColorKey;

BLColorKey BL_ColorAlloc(u16 *data, BLFile *file, u32 display, u32 pltt, u32 idx, u8 num, int isTex)
{
    ColorAlloc *newAlloc;
    int i;
    ColorAlloc *alloc;
    
    ColorKey key;
    
    int shift;
    s8 numRows;
    int allocMax;
    
    BLResColor *resColor;
    BLGfxAlloc *gfxAlloc;
    u16 *buf;
    u32 allocSize;
    u32 transferOfs;
    NNSGfxTransferType transferType;
    s32 curColors;

    resColor = BL_FILE_GET_DATA_AS(file, BLResColor);
    
    if(!isTex) {
        key.isTex = FALSE;
        key.display = display;
        key.pltt = pltt;
        alloc = &ColorWork->alloc[display][pltt][0];
        allocMax = PLTT_MAX;
    } else {
        key.isTex = TRUE;
        alloc = &ColorWork->allocTex[0];
        allocMax = TEX_PLTT_MAX;
    }
    key.valid = TRUE;
    newAlloc = NULL;
    
    for(i=0; i<allocMax; i++, alloc++) {
        if(alloc->useCnt == 0) {
            if(!newAlloc) {
                newAlloc = alloc;
                key.idx = i;
                continue;
            }
        } else if(file) {
            if(alloc->fileID == file->fileID && alloc->idx == idx) {
                if(alloc->numRows >= num-1) {
                    alloc->useCnt++;
                    key.idx = i;
                    return key.raw;
                }
            }
        }
    }
    if(!newAlloc) {
        return 0;
    }
    shift = 5;
    if(!isTex) {
        if(pltt >= BL_COLOR_OBJ_EXT_PLTT) {
            shift += 4;
        }
    } else if(isTex == 2) {
        shift += 4;
    }
    numRows = num;
    if(file) {
        curColors = (((resColor->size-1) >> shift)+1)-idx;
        if(resColor->defColors != 0) {
            return 0;
        }
        
        if(resColor->size <= (idx << shift)) {
            return 0;
        }
        if(curColors <= 0) {
            return 0;
        }
        if(numRows == 0) {
            numRows = curColors;
        }
        
    }
    if(numRows > 16) {
        numRows = 16;
    }
    allocSize = numRows << shift;
    if(isTex) {
        gfxAlloc = BL_GfxHeapAlloc(&ColorWork->heapTex, allocSize);
    } else {
        gfxAlloc = BL_GfxHeapAlloc(&ColorWork->heap[display][pltt], allocSize);
    }
    if(!gfxAlloc) {
        return 0;
    }
    newAlloc->gfxAlloc = gfxAlloc;
    if(file) {
        newAlloc->fileID = file->fileID;
    } else {
        newAlloc->fileID = -1;
    }
    newAlloc->idx = idx;
    newAlloc->numRows = numRows-1;
    newAlloc->useCnt = 1;
    if(isTex) {
        newAlloc->ofs = gfxAlloc->ofs;
    } else {
        newAlloc->ofs = gfxAlloc->ofs >> shift;
    }
    buf = BL_MemCalloc(BL_MEM_TAG_SCENE_GENERIC, gfxAlloc->size);
    newAlloc->buf = buf;
    if(!buf) {
        if(!isTex) {
            BL_GfxHeapFree(&ColorWork->heap[display][pltt], gfxAlloc);
        } else {
            BL_GfxHeapFree(&ColorWork->heapTex, gfxAlloc);
        }
        return 0;
    }
    
    MI_CpuCopy16((u8 *)data+(newAlloc->idx << shift), buf, gfxAlloc->size);
    transferOfs = gfxAlloc->ofs;
    if(!isTex) {
        transferType = ColorTransferTypeTbl[display][pltt];
        
        if(pltt >= BL_COLOR_BG_EXT_PLTT_1) {
            if(pltt <= BL_COLOR_BG_EXT_PLTT_4) {
                transferOfs += (BG_EXT_PLTT_SIZE*(pltt-BL_COLOR_BG_EXT_PLTT_1));
            }
            
        }
    } else {
        transferType = NNS_GFX_TRANSFER_TEX_PLTT;
    }
    NNS_GfxTransferAdd(transferType, transferOfs, buf, gfxAlloc->size);
    return key.raw;
}

void BL_ColorFree(BLColorKey key)
{
    ColorKey keyVal;
    ColorAlloc *alloc;
    BOOL isTex;
    keyVal.raw = key;
    if(key == 0) {
        return;
    }
    if(!keyVal.valid) {
        return;
    }
    isTex = keyVal.isTex;
    if(isTex) {
        alloc = &ColorWork->allocTex[keyVal.idx];
    } else {
        if(keyVal.pltt >= BL_COLOR_MAX_PLTT) {
            return;
        }
        alloc = &ColorWork->alloc[keyVal.display][keyVal.pltt][keyVal.idx];
    }
    if(alloc->useCnt == 0) {
        return;
    }
    alloc->useCnt--;
    if(alloc->useCnt != 0) {
        return;
    }
    BL_MemFree(alloc->buf);
    if(!isTex) {
        BL_GfxHeapFree(&ColorWork->heap[keyVal.display][keyVal.pltt], alloc->gfxAlloc);
    } else {
        BL_GfxHeapFree(&ColorWork->heapTex, alloc->gfxAlloc);
    }
}

u8 BL_ColorGetOfs(BLColorKey key)
{
    ColorAlloc *alloc;
    ColorKey keyVal;
    keyVal.raw = key;
    
    alloc = &ColorWork->alloc[keyVal.display][keyVal.pltt][0];
    
    return alloc[keyVal.idx].ofs;
}

u32 BL_ColorGetTexOfs(BLColorKey key)
{
    ColorAlloc *alloc;
    ColorKey keyVal;
    keyVal.raw = key;
    
    alloc = &ColorWork->allocTex[keyVal.idx];
    return alloc->ofs;
}

void BL_ColorWrite(BLColorKey key)
{
    BL_ColorWriteEx(key, 0, 0, 0);
}

void BL_ColorWriteEx(BLColorKey key, u32 idx, u8 ofs, u32 size)
{
    ColorKey keyVal;
    ColorAlloc *alloc;
    int shift;
    u32 transferOfs;
    NNSGfxTransferType transferType;
    void *buf;
    
    
    keyVal.raw = key;
    
    if(key == 0) {
        return;
    }
    if(!keyVal.valid) {
        return;
    }
    if(keyVal.isTex) {
        alloc = &ColorWork->allocTex[keyVal.idx];
    } else {
        if(keyVal.pltt >= BL_COLOR_MAX_PLTT) {
            return;
        }
        alloc = &ColorWork->alloc[keyVal.display][keyVal.pltt][keyVal.idx];
    }
    if(alloc->useCnt == 0) {
        return;
    }
    shift = 5;
    if(!keyVal.isTex) {
        if(keyVal.pltt >= BL_COLOR_OBJ_EXT_PLTT) {
            shift += 4;
        }
    } else if(keyVal.isTex == 2) {
        shift += 4;
    }
    if(ofs == 0) {
        ofs = alloc->numRows+1;
    }
    
    if((ofs << shift)+(idx << shift) > alloc->gfxAlloc->size) {
        return;
    }
    
    transferOfs = alloc->gfxAlloc->ofs+(idx << shift);
    if(!keyVal.isTex) {
        transferType = ColorTransferTypeTbl[keyVal.display][keyVal.pltt];
        
        if(keyVal.pltt >= BL_COLOR_BG_EXT_PLTT_1) {
            if(keyVal.pltt <= BL_COLOR_BG_EXT_PLTT_4) {
                transferOfs += (BG_EXT_PLTT_SIZE*(keyVal.pltt-BL_COLOR_BG_EXT_PLTT_1));
            }
            
        }
    } else {
        transferType = NNS_GFX_TRANSFER_TEX_PLTT;
    }
    if(size == 0) {
        buf = alloc->buf;
    }
    NNS_GfxTransferAdd(transferType, transferOfs, buf, ofs << shift);
}

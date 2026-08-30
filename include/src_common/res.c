#include "res.h"
#include "res_anim.h"
#include "res_background.h"
#include "res_image.h"

#include "file.h"
#include "display.h"
#include "memory.h"

#include <nitro.h>
#include <nnsys/gfx_transfer.h>

u32 BL_ResGetAnimVRamSize(void *res, int animNo)
{
    BLResAnim *resAnim = res;
    return resAnim->anim[animNo].numTiles*BL_RES_ANIM_TILE_SIZE;
}

u32 BL_ResGetAnimMaxVRamSize(void *res)
{
    BLResAnim *resAnim = res;
    u32 size = 0;
    BLResAnimData *iter;
    for(iter=&resAnim->anim[0]; iter<BL_RES_ANIM_GET_END(resAnim); iter++) {
        if(size < iter->numTiles*BL_RES_ANIM_TILE_SIZE) {
            size = iter->numTiles*BL_RES_ANIM_TILE_SIZE;
        }
    }
    return size;
}

void BL_ResLoadBackground(BLFile *file, int display, int scrBase, int tileOfs, u8 palOfs)
{
    int i;
    int j;
    
    u16 *rowPtrDst;
    u16 *tempBuf;
    u16 *rowPtrSrc;
    
    u16 baseValue;
    
    u16 writeH;
    u16 writeW;
    
    u16 *srcPtr;
    u32 dstPtr;
    int h;
    int w;
    
    BLResBackground *res;
    u16 *tempIn;
    u16 *tempOut;

    res = BL_FILE_GET_DATA_AS(file, BLResBackground);
    
    srcPtr = &res->data[0];
    if(display != BL_DISPLAY_MAIN) {
        dstPtr = 0x06200000;
    } else {
        dstPtr = 0x06000000+GX_GetBGScrOffset();
    }
    dstPtr += scrBase << 11;
    tempBuf = BL_MemCalloc(BL_MEM_TAG_SYSTEM, 0x800);
    h = res->h;
    baseValue = ((u32)(palOfs << 28) >> 16)|(tileOfs & 0x3FF);
    
    while(1) {
        if(h < 32) {
            writeH = h;
        } else {
            writeH = 32;
        }
        rowPtrSrc = srcPtr;
        w=res->w;
        while(1) {
            if(w < 32) {
                writeW = w;
            } else {
                writeW = 32;
            }
            MI_CpuFill16(0, tempBuf, 0x800);
            for(i=0; i<writeH; i++) {
                
                tempIn = &rowPtrSrc[res->w*i];
                tempOut = &tempBuf[(i*32)];
                for(j=0; j<writeW; j++) {
                    *tempOut++ = (*tempIn++)+baseValue;
                }
            }
            MI_CpuCopy16(tempBuf, (u16 *)dstPtr, 0x800);
            w -= 32;
            dstPtr += 0x800;
            if(w > 0) {
                rowPtrSrc += 0x20;
            } else {
                break;
            }
            
        }
        h -= 32;
        if(h <= 0) {
            break;
        } else {
            srcPtr += res->w*32;
        }
        
    }
    BL_MemFree(tempBuf);
}

void BL_ResLoadBackgroundAffine(BLFile *file, int display, int scrBase, int tileOfs, u8 palOfs)
{
    u16 *tempBuf;
    u16 *srcPtr;
    u32 dstPtr;
    int bufSize;
    u32 dstW;
    u16 baseValue;
    int i;
    int j;

    u16 *tempOut;
    u16 *tempIn;
    
    
    
    BLResBackground *res;
    res = BL_FILE_GET_DATA_AS(file, BLResBackground);
    
    srcPtr = &res->data[0];
    if(display != BL_DISPLAY_MAIN) {
        dstPtr = 0x06200000;
    } else {
        dstPtr = 0x06000000+GX_GetBGScrOffset();
    }
    dstPtr += scrBase << 11;
    dstW = res->w;
    baseValue = ((u32)(palOfs << 28) >> 16)|(tileOfs & 0x3FF);
    if(dstW > 64) {
        dstW = 128;
    } else if(dstW > 32) {
        dstW = 64;
    } else if(dstW > 16) {
        dstW = 32;
    }
    bufSize = dstW*res->h*2;
    tempBuf = BL_MemCalloc(BL_MEM_TAG_SYSTEM, bufSize);
    tempIn = srcPtr;
    for(i=0; i<res->h; i++) {
        tempOut = &tempBuf[dstW*i];
        for(j=0; j<res->w; j++) {
            *tempOut++ = (*tempIn++)+baseValue;
        }
    }
    MI_CpuCopy16(tempBuf, (u16 *)dstPtr, bufSize);
    BL_MemFree(tempBuf);
}

void BL_ResLoadImage(BLFile *file, int type, u32 ofs)
{
    BLResImage *res;
    res = BL_FILE_GET_DATA_AS(file, BLResImage);
    DC_FlushRange(&res->data[0], res->size);
    switch(type) {
        case 0:
            GX_LoadBG0Char(&res->data[0], ofs, res->size);
            break;
        
        case 1:
            GX_LoadBG1Char(&res->data[0], ofs, res->size);
            break;
        
        case 2:
            GX_LoadBG2Char(&res->data[0], ofs, res->size);
            break;
        
        case 3:
            GX_LoadBG3Char(&res->data[0], ofs, res->size);
            break;
        
        case 4:
            GX_LoadOBJ(&res->data[0], ofs, res->size);
            break;
            
        case 5:
            GXS_LoadBG0Char(&res->data[0], ofs, res->size);
            break;
        
        case 6:
            GXS_LoadBG1Char(&res->data[0], ofs, res->size);
            break;
        
        case 7:
            GXS_LoadBG2Char(&res->data[0], ofs, res->size);
            break;
        
        case 8:
            GXS_LoadBG3Char(&res->data[0], ofs, res->size);
            break;
        
        case 9:
            GXS_LoadOBJ(&res->data[0], ofs, res->size);
            break;
    }
}

#ifndef SYS_BBP
void BL_ResLoadImage3D(BLFile *file, u32 ofs)
{
    BLResImage *res;
    res = BL_FILE_GET_DATA_AS(file, BLResImage);
    void *temp = BL_MemAlloc(BL_MEM_TAG_SCENE_IMAGE_3D, res->size, 0);
    MI_CpuCopyFast(&res->data[0], temp, res->size);
    NNS_GfxTransferAdd(NNS_GFX_TRANSFER_TEX, ofs, temp, res->size);
}

#endif
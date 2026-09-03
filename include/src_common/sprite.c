#include <nitro.h>
#include <nnsys/g3d.h>

#include "sprite.h"
#include "sortlist.h"
#include "memory.h"

#define SPR_TYPE_NORMAL 0
#define SPR_TYPE_3D 1
#define SPR_TYPE_RAW 2

typedef struct SpriteData_s {
    BLSortListNode node;
    union {
        BLSprite sprite;
        BLSpr3D spr3D;
        BLSprRaw sprRaw;
        BLSprAnim sprAnim;
    };
    union {
        struct {
            u16 prio : 13;
            u16 layer : 2;
            u16 exists : 1;
        };
        u16 order;
    };
    u8 type : 2;
    u8 screen : 1;
} SpriteData;

typedef struct SprTileData_s {
    u8 numTiles;
    u16 tileNo[128];
} SprTileData;

#define GET_SPRDATA(spr) (SpriteData *)((u8 *)(spr)-sizeof(BLSortListNode))

#ifndef SYS_BBP
static s16 bssUnk2;
#endif
static s16 CurSprZ;
static BLSortList SprList[BL_DISPLAY_MAX];
static SprTileData CurSprTile;

static SpriteData *NewSpr(u16 order, u8 screen);

static void FreeSpr(SpriteData *spr);

void BL_SprInit(void)
{
    MI_CpuFill32(0, &SprList[BL_DISPLAY_MAIN], sizeof(SprList));
    BL_SortListCreate(&SprList[BL_DISPLAY_MAIN], BL_SORTLIST_CMP_GREATER, TRUE);
    BL_SortListCreate(&SprList[BL_DISPLAY_SUB], BL_SORTLIST_CMP_GREATER, TRUE);
    G3X_Init();
    G3X_AntiAlias(FALSE);
    G3X_AlphaBlend(TRUE);
    G3X_SetClearColor(0, 0, 0x7FFF, 0, 0);
    NNS_G3dViewportInit();
    G3_MaterialColorDiffAmb(0x7FFF, 0x7FFF, TRUE);
    
}

static void SprDelete(BLSortListNode *node)
{
    FreeSpr((SpriteData *)node);
}

void BL_SprClose(void)
{
    BL_SortListForEach(&SprList[BL_DISPLAY_MAIN], SprDelete);
    BL_SortListForEach(&SprList[BL_DISPLAY_SUB], SprDelete);
}

BLSprite *BL_SprCreate(int screen, u8 layer, u16 prio, u32 imageAddr, u8 palette, BLResAnim *animRes, BLResImage *imageRes)
{
    SpriteData *sprData;
    u16 prioValue;
    BLSprite *spr;
    prioValue = 0x8000|(layer << 13)|(prio & 0x1FFF);
    sprData = NewSpr(prioValue, screen & 0x1);
    if(!sprData) {
        return NULL;
    }
    sprData->type = SPR_TYPE_NORMAL;
    spr = &sprData->sprite;
    MI_CpuFill8(spr, 0, sizeof(BLSprite));
    spr->base.animSpeed = 1 << 6;
    spr->base.imageAddr = imageAddr;
    spr->base.animRes = animRes;
    spr->base.nextAnimNo = BL_SPR_ANIM_NULL;
    spr->base.scaleY = spr->base.scaleX = FX32_ONE;
    spr->base.angle = 0;
    spr->base.animLayer = 0;
    spr->base.palette = palette;
    spr->imageRes = imageRes;
    if(spr->imageRes) {
        spr->imageDirty = TRUE;
    }
    spr->animSpr = spr;
    return spr;
}

void BL_SprKill(BLSprite *spr)
{
    BLSprite *iter;
    if(!spr) {
        return;
    }
    if(spr->animSprNext) {
        if(spr->animSpr == spr) {
            spr->animSprNext->base.animNo = spr->base.animNo;
            spr->animSprNext->base.nextAnimNo = spr->base.nextAnimNo;
            
            spr->animSprNext->base.animFrame = spr->base.animFrame;
            spr->animSprNext->base.frameTime = spr->base.frameTime;
            spr->animSprNext->base.animSpeed = spr->base.animSpeed;
            spr->animSprNext->base.animPlayMax = spr->base.animPlayMax;
            spr->animSprNext->base.animPlayNo = spr->base.animPlayNo;
            spr->animSprNext->base.animPause = spr->base.animPause;
            spr->animSprNext->base.animDone = spr->base.animDone;
            spr->animSprNext->base.frameDone = spr->base.frameDone;
            spr->animSprNext->base.animNextStart = spr->base.animNextStart;
            spr->animSprNext->base.imageAddr = spr->base.imageAddr;
            spr->animSprNext->base.animRes = spr->base.animRes;
            spr->animSprNext->imageDirty = spr->imageDirty;
            spr->animSprNext->imageRes = spr->imageRes;
            for(iter=spr->inheritList; iter !=spr; iter=iter->inheritList) {
                iter->animSpr = spr->animSprNext;
            }
        }
        if(spr->inheritList == spr->animSprNext) {
            spr->inheritList->animSprNext = NULL;
            spr->animSprNext->inheritList = NULL;
        } else {
            spr->animSprNext->inheritList = spr->inheritList;
            spr->inheritList->animSprNext = spr->animSprNext;
        }
    }
    FreeSpr(GET_SPRDATA(spr));
}

void BL_SprSetAnim(BLSprite *spr, u16 animNo, u8 animFrame)
{
    BLSprite *anim = spr->animSpr;
    anim->base.animNo = animNo;
    anim->base.animFrame = animFrame;
    anim->base.animPlayNo = 0;
    anim->base.frameTime = 0;
    anim->base.nextAnimNo = BL_SPR_ANIM_NULL;
    anim->base.animNextStart = FALSE;
    anim->base.animDone = FALSE;
    
    if(anim->base.animRes->anim[anim->base.animNo].numFrames == 1) {
        anim->base.animPlayMax = 1;
    } else {
        anim->base.animPlayMax = 0;
    }
    if(spr->animSpr->imageRes) {
        spr->animSpr->imageDirty = TRUE;
    }
}

static inline void ResortSprite(SpriteData *sprData)
{
    BL_SortListNodeSetOrder(&SprList[sprData->screen], (BLSortListNode *)sprData, sprData->order);
}

#ifndef SYS_BBP
void BL_SprSetLayer(BLSprite *spr, u8 layer)
{
    SpriteData *sprData = GET_SPRDATA(spr);
    sprData->layer = layer;
    ResortSprite(sprData);
}
#endif

void BL_SprSetPrio(BLSprite *spr, u16 prio)
{
    SpriteData *sprData = GET_SPRDATA(spr);
    sprData->prio = prio;
    ResortSprite(sprData);
}

void BL_SprSetLayerPrio(BLSprite *spr, u8 layer, u16 prio)
{
    SpriteData *sprData = GET_SPRDATA(spr);
    sprData->layer = layer;
    sprData->prio = prio;
    ResortSprite(sprData);
}

u16 BL_SprGetPrio(BLSprite *spr)
{
    SpriteData *sprData = GET_SPRDATA(spr);
    return sprData->prio;
}

#ifndef SYS_BBP

BLSpr3D *BL_Spr3DCreate(u16 prio, u32 imageAddr, u32 colorAddr, BLResAnim *animRes)
{
    SpriteData *sprData;
    u16 prioValue;
    BLSpr3D *spr;
    prioValue = prio & 0x1FFF;
    sprData = NewSpr(prioValue, 0);
    if(!sprData) {
        return NULL;
    }
    sprData->type = SPR_TYPE_3D;
    spr = &sprData->spr3D;
    MI_CpuFill8(spr, 0, sizeof(BLSpr3D));
    spr->base.animSpeed = 1 << 6;
    spr->base.imageAddr = imageAddr;
    spr->base.animRes = animRes;
    spr->base.nextAnimNo = BL_SPR_ANIM_NULL;
    spr->base.scaleX = FX32_ONE;
    spr->base.scaleY = FX32_ONE;
    spr->base.angle = 0;
    spr->base.animLayer = 0;
    spr->alpha = 31;
    spr->colorAddr = colorAddr;
    
    return spr;
}

void BL_Spr3DKill(BLSpr3D *spr)
{
    if(!spr) {
        return;
    }
    FreeSpr(GET_SPRDATA(spr));
}

void BL_Spr3DSetAnim(BLSpr3D *spr, u16 animNo, u8 animFrame)
{
    spr->base.animNo = animNo;
    spr->base.animFrame = animFrame;
    spr->base.animPlayNo = 0;
    spr->base.frameTime = 0;
    spr->base.nextAnimNo = BL_SPR_ANIM_NULL;
    spr->base.animNextStart = FALSE;
    spr->base.animDone = FALSE;
    
    if(spr->base.animRes->anim[spr->base.animNo].numFrames == 1) {
        spr->base.animPlayMax = 1;
    } else {
        spr->base.animPlayMax = 0;
    }
}

void BL_Spr3DSetPrio(BLSpr3D *spr, u16 prio)
{
    SpriteData *sprData = GET_SPRDATA(spr);
    sprData->prio = prio;
    ResortSprite(sprData);
}

BLSprRaw *BL_SprRawCreate(u16 prio, u32 imageAddr, u32 colorAddr, u8 width, u8 height, u8 is8BPP)
{
    SpriteData *sprData;
    u16 prioValue;
    BLSprRaw *spr;
    prioValue = prio & 0x1FFF;
    sprData = NewSpr(prioValue, 0);
    if(!sprData) {
        return NULL;
    }
    sprData->type = SPR_TYPE_RAW;
    spr = &sprData->sprRaw;
    MI_CpuFill8(spr, 0, sizeof(BLSprRaw));
    spr->imageAddr = imageAddr;
    spr->scaleX = FX32_ONE;
    spr->scaleY = FX32_ONE;
    spr->angle = 0;
    spr->width = width;
    spr->height = height;
    spr->is8BPP = is8BPP;
    spr->alpha = 31;
    spr->colorAddr = colorAddr;
    return spr;
}

void BL_SprRawKill(BLSprRaw *spr)
{
    if(!spr) {
        return;
    }
    FreeSpr(GET_SPRDATA(spr));
}

#endif

static SpriteData *NewSpr(u16 order, u8 screen)
{
    SpriteData *spr = BL_MemAlloc(BL_MEM_TAG_SCENE_SPRITE, sizeof(SpriteData), 0);
    if(!spr) {
        return NULL;
    }
    spr->screen = screen;
    spr->order = order;
    BL_SortListNodeInsert(&SprList[screen], (BLSortListNode *)spr, order);
    return spr;
}

static void FreeSpr(SpriteData *spr)
{
    BL_SortListNodeRemove(&SprList[spr->screen], (BLSortListNode *)spr);
    BL_MemFree(spr);
}

static void SprLoopAnim(BLSortListNode *node);
static void SprUpdateAnim(BLSortListNode *node);
static void SprDraw(BLSortListNode *node);

void BL_SprExec(void)
{
    CurSprZ = 1024;
    #ifndef SYS_BBP
    bssUnk2 = 0;
    #endif
    BL_SortListForEach(&SprList[BL_DISPLAY_MAIN], SprLoopAnim);
    BL_SortListForEach(&SprList[BL_DISPLAY_SUB], SprLoopAnim);
    BL_SortListForEach(&SprList[BL_DISPLAY_MAIN], SprDraw);
    BL_SortListForEach(&SprList[BL_DISPLAY_SUB], SprDraw);
    BL_SortListForEach(&SprList[BL_DISPLAY_MAIN], SprUpdateAnim);
    BL_SortListForEach(&SprList[BL_DISPLAY_SUB], SprUpdateAnim);
}

static void SprDrawAnim(SpriteData *sprData);
static void SprDrawRaw(SpriteData *sprData);

typedef void (*SprDrawFunc)(SpriteData *sprData);

static SprDrawFunc DrawFunc[3] = {
    SprDrawAnim,
    SprDrawAnim,
    SprDrawRaw
};

static void SprDraw(BLSortListNode *node)
{
    SpriteData *spr = (SpriteData *)node;
    DrawFunc[spr->type](spr);
}

static inline BLSprite *GetBLSprite(SpriteData *sprData)
{
    if(sprData->type == SPR_TYPE_NORMAL) {
        return &sprData->sprite;
    } else {
        return NULL;
    }
}

static inline BLSpr3D *GetBLSpr3D(SpriteData *sprData)
{
    if(sprData->type == SPR_TYPE_3D) {
        return &sprData->spr3D;
    } else {
        return NULL;
    }
}

static void SprLoopAnim(BLSortListNode *node)
{
    SpriteData *sprData = (SpriteData *)node;
    BLSprAnim *sprAnim;
    BLSprite *spr;
    if(sprData->type != SPR_TYPE_NORMAL && sprData->type != SPR_TYPE_3D) {
        return;
    }
    sprAnim = &sprData->sprAnim;
    spr = GetBLSprite(sprData);
    if(spr && spr != spr->animSpr) {
        return;
    }
    if(sprAnim->animPause) {
        return;
    }
    if(!sprAnim->animNextStart) {
        return;
    }
    sprAnim->animNo = sprAnim->nextAnimNo;
    sprAnim->animFrame = 0;
    sprAnim->animPlayNo = 0;
    sprAnim->frameTime = 0;
    sprAnim->nextAnimNo = BL_SPR_ANIM_NULL;
    sprAnim->animNextStart = FALSE;
    sprAnim->animDone = FALSE;
    if(sprAnim->animRes->anim[sprAnim->animNo].numFrames == 1) {
        sprAnim->animPlayMax = 1;
    } else {
        sprAnim->animPlayMax = 0;
    }
    if(spr && spr->imageRes) {
        spr->imageDirty = TRUE;
    }
}

static void SprUpdateAnim(BLSortListNode *node)
{
    SpriteData *sprData = (SpriteData *)node;
    BLSprAnim *sprAnim;
    BLSprite *spr;
    BLResAnim *res;
    BLResAnimData *animData;
    BLResAnimFrame *animFrame;
    
    u16 animMax;
    if(sprData->type != SPR_TYPE_NORMAL && sprData->type != SPR_TYPE_3D) {
        return;
    }
    sprAnim = &sprData->sprAnim;
    spr = GetBLSprite(sprData);
    res = sprAnim->animRes;
    animMax = BL_RES_ANIM_GET_ANIM_MAX(res);
    if(sprAnim->animNo > animMax) {
        //Its definitely doing a signed modulo here
        sprAnim->animNo %= (int)animMax;
        return;
    }
    if(spr && spr->animSpr != spr) {
        return;
    }
    if(sprAnim->animPause) {
        return;
    }
    animData = &res->anim[sprAnim->animNo];
    animFrame = BL_RES_ANIM_GET_FRAME(res)+animData->baseFrame+sprAnim->animFrame;
    sprAnim->frameTime += sprAnim->animSpeed;
    
    sprAnim->animDone = FALSE;
    sprAnim->frameDone = FALSE;
    
    if((sprAnim->frameTime >> 6) >= animFrame->delay) {
        if(++sprAnim->animFrame >= animData->numFrames) {
            if(sprAnim->animPlayMax && ++sprAnim->animPlayNo >= sprAnim->animPlayMax) {
                sprAnim->animDone = TRUE;
                sprAnim->animFrame--;
                sprAnim->frameTime -= sprAnim->animSpeed;
                sprAnim->animPlayNo--;
            } else {
                if(sprAnim->animPlayMax == 0) {
                    sprAnim->animDone = TRUE;
                }
                sprAnim->frameTime = 0;
                sprAnim->animFrame = 0;
                sprAnim->frameDone = TRUE;
            }
        } else {
            sprAnim->frameTime = 0;
            sprAnim->frameDone = TRUE;
        }
    }
    if(sprAnim->nextAnimNo != BL_SPR_ANIM_NULL) {
        if(sprAnim->animDone) {
            
            sprAnim->animDone = FALSE;
            sprAnim->animNextStart = TRUE;
        }
    }
    if(spr) {
        spr->imageDirty = sprAnim->frameDone;
    }
}

static void TransformCellPos(s16 *x, s16 *y, fx32 scaleX, fx32 scaleY, u16 rot, s16 centerX, s16 centerY);


static void SprDrawAnim(SpriteData *sprData)
{
    TransformCellPos(NULL, NULL, 1, 1, 0, 0, 0);
}

static void SprDrawRaw(SpriteData *sprData)
{
    
}

static void TransformCellPos(s16 *x, s16 *y, fx32 scaleX, fx32 scaleY, u16 rot, s16 centerX, s16 centerY)
{
    fx32 cx = FX32_CONVERT(centerX);
    fx32 cy = FX32_CONVERT(centerY);
    fx32 ofsX = FX32_CONVERT(*x)-cx;
    fx32 ofsY = FX32_CONVERT(*y)-cy;
    
    fx32 scaleOfsX = FX_Mul(ofsX, scaleX);
    fx32 scaleOfsY = FX_Mul(ofsY, scaleY);
    fx32 s = FX_SinIdx(rot);
    fx32 c = FX_CosIdx(rot);
    
    fx32 outX = FX_Mul(scaleOfsX, c)-FX_Mul(scaleOfsY, s);
    fx32 outY = FX_Mul(scaleOfsX, s)+FX_Mul(scaleOfsY, c);
    
    outX += cx;
    outY += cy;
    *x = outX/4096.0f;
    *y = outY/4096.0f;
}
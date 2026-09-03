#ifndef RES_ANIM_H
#define RES_ANIM_H

#include <nitro/types.h>

//Must be done this way to prevent signedness issues
#define BL_RES_ANIM_GET_ANIM_MAX(res) ((((res)->animFrameOfs+((u8 *)(res)))-(u8 *)(&(res)->anim[0]))/sizeof(BLResAnimData))

#define BL_RES_ANIM_GET_ANIM_END(res) ((BLResAnimData *)((res)->animFrameOfs+((u8 *)(res))))
#define BL_RES_ANIM_GET_XFORM(res) ((BLResAnimXForm *)((res)->xformOfs+((u8 *)(res))))
#define BL_RES_ANIM_GET_FRAME(res) ((BLResAnimFrame *)((res)->animFrameOfs+((u8 *)(res))))
#define BL_RES_ANIM_GET_CELL(res) ((BLResAnimCell *)((res)->cellOfs+((u8 *)(res))))
#define BL_RES_ANIM_GET_OBJ(res) ((BLResAnimObj *)((res)->objOfs+((u8 *)(res))))
#define BL_RES_ANIM_GET_OBJ_XFORM(res) ((BLResAnimObjXForm *)((res)->objXFormOfs+((u8 *)(res))))

#define BL_RES_ANIM_TILE_SIZE 32
#define BL_RES_ANIM_IDX_NULL 0xFFFF

typedef struct BLResAnimData_s {
    u16 baseFrame;
    u16 numTiles;
    u8 numFrames;
    u16 pad;
} BLResAnimData;

typedef struct BLResAnimFrame_s {
    u16 cell;
    u16 delay;
    u16 xformIdx;
    u16 pad;
} BLResAnimFrame;

typedef struct BLResAnimXForm_s {
    s16 ofsX;
    s16 ofsY;
    s32 scaleX;
    s32 scaleY;
    u16 rot;
} BLResAnimXForm;

typedef struct BLResAnimCell_s {
    u16 objBase;
    u16 numObjs;
} BLResAnimCell;

typedef struct BLResAnimObj_s {
    s16 posX;
    s16 posY;
    u16 tileNo;
    u16 shape : 2;
    u16 size : 2;
    u16 pal : 4;
    u16 extPalette : 1;
    u16 flipX : 1;
    u16 flipY : 1;
    u16 layerFlag : 2;
    u16 doubleSize : 1;
    u16 tileObjIdx;
    u16 objXFormIdx;
} BLResAnimObj;

typedef struct BLResAnimObjXForm_s {
    s32 scaleX;
    s32 scaleY;
    u16 rot;
} BLResAnimObjXForm;

typedef struct BLResAnim_s {
    vu32 animFrameOfs;
    u32 xformOfs;
    u32 cellOfs;
    u32 objOfs;
    u32 objXFormOfs;
    BLResAnimData anim[];
} BLResAnim;


#endif
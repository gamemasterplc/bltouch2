#ifndef RES_ANIM_H
#define RES_ANIM_H

#include <nitro/types.h>

#define BL_RES_ANIM_GET_END(anim) ((BLResAnimData *)((anim)->animFrameOfs+((u8 *)(anim))))
#define BL_RES_ANIM_GET_XFORM(anim) ((BLResAnimXForm *)((anim)->xformOfs+((u8 *)(anim))))
#define BL_RES_ANIM_GET_FRAME(anim) ((BLResAnimFrame *)((anim)->animFrameOfs+((u8 *)(anim))))
#define BL_RES_ANIM_GET_CELL(anim) ((BLResAnimCell *)((anim)->cellOfs+((u8 *)(anim))))
#define BL_RES_ANIM_GET_OBJ(anim) ((BLResAnimObj *)((anim)->objOfs+((u8 *)(anim))))

#define BL_RES_ANIM_TILE_SIZE 32

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
    u16 nextObj;
} BLResAnimObj;

typedef struct BLResAnim_s {
    u32 animFrameOfs;
    u32 xformOfs;
    u32 cellOfs;
    u32 objOfs;
    u32 endOfs;
    BLResAnimData anim[];
} BLResAnim;

#endif
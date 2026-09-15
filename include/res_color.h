#ifndef RES_COLOR_H
#define RES_COLOR_H

#include <nitro/types.h>

#define BL_RES_IMAGE_FMT_4BPP 0
#define BL_RES_IMAGE_FMT_8BPP 1
#define BL_RES_IMAGE_FMT_TEX_4BPP 2
#define BL_RES_IMAGE_FMT_TEX_8BPP 3

typedef struct BLResColor_s {
    u32 size;
    u8 defColors;
    u32 unk8;
    u32 unkC;
    u8 data[];
} BLResColor;

#endif
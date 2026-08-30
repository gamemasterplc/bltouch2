#ifndef RES_IMAGE_H
#define RES_IMAGE_H

#include <nitro/types.h>

#define BL_RES_IMAGE_FMT_4BPP 0
#define BL_RES_IMAGE_FMT_8BPP 1
#define BL_RES_IMAGE_FMT_3D 2

typedef struct BLResImage_s {
    u32 size;
    u8 w;
    u8 h;
    u8 fmt;
    u8 pad;
    u8 data[];
} BLResImage;

#endif
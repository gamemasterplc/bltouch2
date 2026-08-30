#ifndef RES_BACKGROUND_H
#define RES_BACKGROUND_H

#include <nitro/types.h>

typedef struct BLResBackground_s {
    u32 size;
    u32 unk4;
    u16 w;
    u16 h;
    u32 unk;
    u16 data[];
} BLResBackground;

#endif
#ifndef RES_TONE_H
#define RES_TONE_H

#include <nitro/types.h>

typedef struct BLResToneFrame_s {
    union {
        u32 delay : 23;
        u32 delayRaw;
    };
    u16 data[]; //0x04
} BLResToneFrame;

typedef struct BLResToneAnim_s {
    u16 numFrame; //0x00
    u16 dataSize; //0x02
    u16 colorIdx; //0x04
    u16 dataPitch; //0x06
    u8 data[]; //0x08 (array of BLResToneFrame)
} BLResToneAnim;

typedef struct BLResTone_s {
    u16 numAnim; //0x00
    u16 pad;
    u32 animOfsTblOfs; //0x04
    u32 animBaseOfs; //0x08
} BLResTone;

#endif
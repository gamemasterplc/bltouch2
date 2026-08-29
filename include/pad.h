#ifndef PAD_H
#define PAD_H

#include <nitro.h>
#include "wireless_defs.h"


#ifdef SYS_BBP
#define BL_PadInit BBP_PadInit
#define BL_PadRead BBP_PadRead
#define BL_PadExec BBP_PadExec
#define BL_PadGetPlayer BBP_PadGetPlayer
#define BL_PadWork BBP_PadWork
#endif


typedef struct BLPadPlayer_s {
    u16 btnDown;
    u16 btn;
    u16 btnUp;
    u16 btnRepeat;
    u8 repeatTimer[12];
} BLPadPlayer;

typedef struct BLPadWork_s {
    BLPadPlayer local;
    BLPadPlayer remote[BL_WIRELESS_MAX_PLAYERS];
    #if SYS_BBP
    BLPadPlayer unkB4;
    #endif
} BLPadWork;

extern BLPadWork *BL_PadWork;

void BL_PadInit(void);
u16 BL_PadRead(void);
void BL_PadExec(void);
BLPadPlayer *BL_PadGetPlayer(int no);

#endif
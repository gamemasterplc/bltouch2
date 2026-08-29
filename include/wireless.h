#ifndef WIRELESS_H
#define WIRELESS_H

#include <nitro.h>
#include "file.h"
#include "wireless_defs.h"

#ifdef SYS_BBP
#define BL_WLGetFilePtr BBP_WLGetFilePtr
#define BL_WLCheckInputSync BBP_WLCheckInputSync
#define BL_WLGetPadButton BBP_WLGetPadButton
#define BL_WLGetPlayerData BBP_WLGetPlayerData
#endif

#define BL_WLGetTPData(playerNo) (&BL_WLGetPlayerData(playerNo)->tp)

typedef struct BLWirelessPlayer_s {
    TPData tp;
    u16 button;
    u8 flag;
    u8 flag2;
    u32 seed;
    u8 buf[BL_WIRELESS_PLAYER_BUF_SIZE];
} BLWirelessPlayer;


BLFile *BL_WLGetFilePtr(void);
BOOL BL_WLCheckInputSync(void);
u16 BL_WLGetPadButton(int playerNo);
BLWirelessPlayer *BL_WLGetPlayerData(int playerNo);

#endif
#include <nitro.h>
#include "pad.h"
#include "sys.h"
#include "memory.h"
#include "wireless.h"

BLPadWork *BL_PadWork;

static void SetPlayerData(BLPadPlayer *player, u16 input);

void BL_PadInit(void)
{
    BL_PadWork = BL_MemCalloc(BL_MEM_TAG_SYSTEM, sizeof(BLPadWork));
    if(BL_PadWork) {
        return;
    }
}

u16 BL_PadRead(void)
{
    if(BL_SysCheckSleep()) {
        return 0;
    }
    return PAD_Read();
}

void BL_PadExec(void)
{
    int i;
    SetPlayerData(&BL_PadWork->local, BL_PadRead());
    if(!BL_WLCheckInputSync()) {
        return;
    }
    for(i=0; i<BL_WIRELESS_MAX_PLAYERS; i++) {
        SetPlayerData(&BL_PadWork->remote[i], BL_WLGetPadButton(i));
    }
}

static void SetPlayerData(BLPadPlayer *player, u16 input)
{
    int i;
    if((input & (PAD_KEY_RIGHT|PAD_KEY_LEFT)) == (PAD_KEY_RIGHT|PAD_KEY_LEFT)) {
        input &= ~(PAD_KEY_RIGHT|PAD_KEY_LEFT);
    }
    if((input & (PAD_KEY_UP|PAD_KEY_DOWN)) == (PAD_KEY_UP|PAD_KEY_DOWN)) {
        input &= ~(PAD_KEY_UP|PAD_KEY_DOWN);
    }
    player->btnUp = player->btn&(input^player->btn);
    player->btnDown = input&(input^player->btn);
    player->btn = input;
    player->btnRepeat = 0;
    
    for(i=0; i<12; i++) {
        u16 mask = 1 << i;
        if(input & mask) {
            if(player->btnDown & mask) {
                player->repeatTimer[i] = 18;
                player->btnRepeat |= mask;
            } else {
                if(player->repeatTimer[i] == 0) {
                    player->repeatTimer[i] = 6;
                    player->btnRepeat |= mask;
                } else {
                    player->repeatTimer[i]--;
                }
            }
        }
    }
}

BLPadPlayer *BL_PadGetPlayer(int no)
{
    if(!BL_WLCheckInputSync()) {
        return &BL_PadWork->local;
    } else {
        return &BL_PadWork->remote[no];
    }
}

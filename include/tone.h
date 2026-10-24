#ifndef TONE_H
#define TONE_H

#include "gfxqueue.h"
#include "res_tone.h"

typedef enum {
    BL_TONE_PLTT_OBJ, //0
    BL_TONE_PLTT_OBJ_EXT, //1
    BL_TONE_PLTT_OBJ_SUB, //2
    BL_TONE_PLTT_OBJ_EXT_SUB, //3
    BL_TONE_PLTT_BG, //4
    BL_TONE_PLTT_BG0_EXT, //5
    BL_TONE_PLTT_BG1_EXT, //6
    BL_TONE_PLTT_BG2_EXT, //7
    BL_TONE_PLTT_BG3_EXT, //8
    BL_TONE_PLTT_BG_SUB, //9
    BL_TONE_PLTT_BG0_EXT_SUB, //10
    BL_TONE_PLTT_BG1_EXT_SUB, //11
    BL_TONE_PLTT_BG2_EXT_SUB, //12
    BL_TONE_PLTT_BG3_EXT_SUB, //13
    BL_TONE_PLTT_TEX, //14
} BLTonePlttType;

#ifdef SYS_BBP
#define BL_ToneInit BBP_ToneInit
#define BL_ToneClose BBP_ToneClose
#define BL_ToneCreate BBP_ToneCreate
#define BL_ToneCreateFlag BBP_ToneCreateFlag
#define BL_ToneKill BBP_ToneKill
#define BL_ToneGetData BBP_ToneGetData
#define BL_ToneExec BBP_ToneExec

#endif

typedef struct BLTone_s BLTone;

typedef struct BLToneTransfer_s BLToneTransfer;

typedef u16 BLToneId;

#define BL_TONEID_INVALID 0

struct BLToneTransfer_s {
    BLToneTransfer *next; //0x00
    BLGfxQueueEntry *queueEntry; //0x04
    BLResToneAnim *curAnim; //0x08
    int time; //0x0C
    u16 frame; //0x10
    u8 endF : 1; //0x12
    BLGfxQueueFunc transferFunc; //0x14
    u32 destOfs; //0x18
    u16 *src; //0x1C
    u32 srcSize; //0x20
    
};

struct BLTone_s {
    BLTone *prev; //0x00
    BLTone *next; //0x04
    BLToneTransfer *transfer; //0x08
    u32 unkC; //0x0C
    s32 loopMode; //0x10
    u32 pauseFlag; //0x14
    u16 prio; //0x18
    u16 numAnim; //0x1A
    BLToneId id; //0x1C
    u16 speed; //0x1E
    //Flags at 0x20
    u32 autoRemove : 1; //Bit 0
    u32 pause : 1; //Bit 1
    u32 firstF : 1; //Bit 2
};

void BL_ToneInit(void);
void BL_ToneClose(void);
BLToneId BL_ToneCreate(BLResTone *res, BLTonePlttType plttType, u32 plttOfs, u16 prio);
BLToneId BL_ToneCreateFlag(BLResTone *res, BLTonePlttType plttType, u32 plttOfs, u16 prio, u32 pauseFlag);
void BL_ToneKill(BLToneId id);

BLTone *BL_ToneGetData(BLToneId id);
void BL_ToneExec(void);

#ifndef SYS_BBP
void BL_ToneSetPauseFlag(u32 pauseFlag);
void BL_ToneResetPauseFlag(u32 pauseFlag);
void BL_ToneSetSpeed(BLToneId id, u16 speed);

#endif

#endif
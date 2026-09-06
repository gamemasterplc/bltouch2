#ifndef SOUND_H
#define SOUND_H

#include "nnsys/snd.h"

#ifdef SYS_BBP
#include "SD_BBP2.sadl"
#else
#include "SD_BLDS2.sadl"
#endif

#ifdef SYS_BBP
#define BL_SndInit BBP_SndInit
#define BL_SndExec BBP_SndExec
#define BL_SndClose BBP_SndClose
#define BL_SndSeqInit BBP_SndSeqInit
#define BL_SndPlayMusic BBP_SndPlayMusic
#define BL_SndStopMusic BBP_SndStopMusic
#define BL_SndFadeInMusic BBP_SndFadeInMusic
#define BL_SndFadeOutMusic BBP_SndFadeOutMusic
#define BL_SndSetMusicVolume BBP_SndSetMusicVolume
#define BL_SndGetMusicHandle BBP_SndGetMusicHandle
#define BL_SndPlayJingle BBP_SndPlayJingle
#define BL_SndPlaySE BBP_SndPlaySE
#define BL_SndPlaySELoop BBP_SndPlaySELoop
#define BL_SndStopSELoop BBP_SndStopSELoop
#define BL_SndPlaySELoopMulti BBP_SndPlaySELoopMulti
#define BL_SndStopSELoopMulti BBP_SndStopSELoopMulti
#endif

#ifndef SYS_BBP
void BL_SndLoadGroup(int grp);
void BL_SndLoadZoneGroup(u32 zone);
void BL_SndLoadZoneEvilGroup(u32 zone);
void BL_SndLoadMgGroup(u32 mgNo);
s32 BL_SndGetHeapFreeSize(void);
void BL_SndPause(BOOL pause);
BOOL BL_SndTempoFadeMusic(s32 tempo, s32 speed);
void BL_SndPlayMgMusic(int mgNo);
#else
void BBP_SndArcRestore(void);
void BBP_SndSetMusicVolume(int volume);
#endif

void BL_SndInit(void);
void BL_SndExec(void);
void BL_SndClose(void);
void BL_SndSeqInit(void);
void BL_SndPlayMusic(s32 seqNo, s32 volume);
void BL_SndStopMusic(s32 speed);
BOOL BL_SndFadeInMusic(s32 speed);
BOOL BL_SndFadeOutMusic(s32 speed);
NNSSndHandle *BL_SndGetMusicHandle(void);
void BL_SndPlayJingle(s32 seqNo);
void BL_SndPlaySE(s32 seqArcNo, s32 seNo);
void BL_SndPlaySELoop(s32 seqArcNo, s32 seNo);
void BL_SndStopSELoop(s32 seqArcNo, s32 seNo);
NNSSndHandle *BL_SndPlaySELoopMulti(s32 seqArcNo, s32 seNo);
void BL_SndStopSELoopMulti(NNSSndHandle *handle);

#endif
#include "sound.h"
#include "memory.h"

#ifdef SYS_BBP
#include "bbp/dwc.h"
#endif

#include <nitro.h>

#define MUS_HANDLE_MUSIC 0
#define MUS_HANDLE_JINGLE 1
#define MUS_HANDLE_MAX 2

#define SE_LOOP_MAX 16

#ifndef SYS_BBP
#define HAS_GROUP
#define SDAT_PATH "sound/SD_BLDS2.sdat"
#define HEAP_SIZE 0x80000
#else
#define SDAT_PATH "BBP2data/sound/SD_BBP2.sdat"
#define HEAP_SIZE 0xC0000
#endif

typedef struct SndLoopSE_s {
    NNSSndHandle handle;
    u16 seqArcNo;
    u16 seNo;
    u16 numPlay;
    u16 playMulti;
} SndLoopSE;

typedef struct MusicWork_s {
    u8 mode;
    u8 fadeIn;
    u8 fadeTempo;
    u8 curMove;
    u16 speed;
    u16 tempoTarget;
    s16 time;
    s32 unkC;
    
} MusicWork;

typedef struct SndWork_s {
    void *sndMem;
    #ifndef SYS_BBP
    NNSSndHeapHandle sndHeap;
    #endif
    NNSSndArc sndArc;
    #ifndef SYS_BBP
    NNSSndArc sndArc2;
    #endif
    NNSSndHandle musHandle[MUS_HANDLE_MAX];
    NNSSndHandle seHandle;
    SndLoopSE loopSE[SE_LOOP_MAX];
    MusicWork musWork;
    #ifdef HAS_GROUP
    s16 lastGrp;
    #endif
} SndWork_t;

static SndWork_t *SndWork;

void BL_SndInit(void)
{
    FSFile file;
    u32 length;
    void *heap;
    int i;
    
    SndWork = BL_MemCalloc(BL_MEM_TAG_SYSTEM, sizeof(SndWork_t));
    if(!SndWork) {
        return;
    }
    NNS_SndInit();
    #ifndef SYS_BBP
    SndWork->sndMem = BL_MemCalloc(BL_MEM_TAG_SYSTEM, HEAP_SIZE);
    if(!SndWork->sndMem) {
        BL_MemFree(SndWork);
        return;
    }
    SndWork->sndHeap = NNS_SndHeapCreate(SndWork->sndMem, HEAP_SIZE);
    NNS_SndArcInit(&SndWork->sndArc, SDAT_PATH, SndWork->sndHeap, FALSE);
    NNS_SndArcPlayerSetup(SndWork->sndHeap);
    NNS_SndHeapSaveState(SndWork->sndHeap);
    #else
    FS_InitFile(&file);
    if(!FS_OpenFile(&file, SDAT_PATH)) {
        return;
    }
    length = FS_GetLength(&file);
    if(length < HEAP_SIZE) {
        length = HEAP_SIZE;
    }
    heap = BL_MemCalloc(BL_MEM_TAG_SYSTEM, length);
    if(!heap) {
        return;
    }
    FS_ReadFile(&file, heap, length);
    FS_CloseFile(&file);
    SndWork->sndMem = heap;
    NNS_SndArcInitOnMemory(&SndWork->sndArc, heap);
    NNS_SndArcPlayerSetup(NULL);
    #endif
    for(i=0; i<MUS_HANDLE_MAX; i++) {
        NNS_SndHandleInit(&SndWork->musHandle[i]);
    }
    NNS_SndHandleInit(&SndWork->seHandle);
    for(i=0; i<SE_LOOP_MAX; i++) {
        NNS_SndHandleInit(&SndWork->loopSE[i].handle);
    }
    #ifdef HAS_GROUP
    SndWork->lastGrp = -1;
    #endif
}

static void ExecMusic(void);
static void ExecSE(void);

void BL_SndExec(void)
{
    ExecMusic();
    ExecSE();
    NNS_SndMain();
}

void BL_SndClose(void)
{
    int i;
    BL_SndSeqInit();
    NNS_SndMain();
    for(i=0; i<MUS_HANDLE_MAX; i++) {
        NNS_SndHandleReleaseSeq(&SndWork->musHandle[i]);
    }
    NNS_SndHandleReleaseSeq(&SndWork->seHandle);
    for(i=0; i<SE_LOOP_MAX; i++) {
        NNS_SndHandleReleaseSeq(&SndWork->loopSE[i].handle);
    }
    #ifndef SYS_BBP
    NNS_SndHeapDestroy(SndWork->sndHeap);
    #endif
    BL_MemFree(SndWork->sndMem);
    BL_MemFree(SndWork);
}

#ifdef HAS_GROUP
void BL_SndLoadGroup(int grp)
{
    if(SndWork->lastGrp == grp) {
        return;
    }
    NNS_SndHeapLoadState(SndWork->sndHeap, TRUE);
    if(grp < 0) {
        return;
    }
    NNS_SndArcLoadGroup(grp, SndWork->sndHeap);
    
    SndWork->lastGrp = grp;
}

#endif

#ifndef SYS_BBP

void BL_SndLoadZoneGroup(u32 zone)
{
    static const s8 grpTbl[34] = {
        10, 10, 10, 10,
        11, 11, 11, 11, 11,
        14, 14, 14, 14, 14,
        15, 15, 15,
        19, 19,
        21, 21,
        20, 20, 20, 20,
        23, 23, 23, 23,
        24, 24, 24, 24,
        26
    };
    int grp = -1;
    if(zone < 34) {
        grp = grpTbl[zone];
    }
    BL_SndLoadGroup(grp);
}

void BL_SndLoadZoneEvilGroup(u32 zone)
{
    static const s8 grpTbl[34] = {
        27, 27, 27, 27,
        28, 28, 28, 28, 28,
        29, 29, 29, 29, 29,
        30, 30, 30,
        31, 31,
        21, 21,
        32, 32, 32, 32,
        33, 33, 33, 33,
        34, 34, 34, 34,
        26
    };
    int grp = -1;
    if(zone < 34) {
        grp = grpTbl[zone];
    }
    BL_SndLoadGroup(grp);
}

void BL_SndLoadMgGroup(u32 mgNo)
{
    static const s8 grpTbl[45] = {
        35, 36, 37, 38, 39,
        40, 41, 42, 43, 44,
        45, 46, 47, 48, 49,
        50, 51, 52, 53, 54,
        55, 56, 57, 58, 59,
        60, 61, 62, 63, 64,
        65, 66, 67, 68, 69,
        70, 71, 72, 73, 74,
        45, 46, 66, 48, 63
    };
    int grp = -1;
    if(mgNo < 45) {
        grp = grpTbl[mgNo];
    }
    BL_SndLoadGroup(grp);
}

void BL_SndPause(BOOL pause)
{
    int i;
    
    NNS_SndPlayerPause(&SndWork->musHandle[MUS_HANDLE_MUSIC], pause);
    NNS_SndPlayerPause(&SndWork->musHandle[MUS_HANDLE_JINGLE], pause);
    NNS_SndPlayerPause(&SndWork->seHandle, pause);
    for(i=0; i<SE_LOOP_MAX; i++) {
        SndLoopSE *se = &SndWork->loopSE[i];
        if(se->numPlay != 0) {
            NNS_SndPlayerPause(&se->handle, pause);
        }
    }
}
#else
void BBP_SndArcRestore(void)
{
    NNS_SndArcSetCurrent(&SndWork->sndArc);
}
    
#endif

void BL_SndSeqInit(void)
{
    int i;
    NNS_SndPlayerStopSeqAll(FALSE);
    for(i=0; i<SE_LOOP_MAX; i++) {
        SndWork->loopSE[i].numPlay = 0;
    }
    MI_CpuFill8(&SndWork->musWork, 0, sizeof(MusicWork));
}

#ifndef SYS_BBP
s32 BL_SndGetHeapFreeSize(void)
{
    if(!MB_IsMultiBootChild()) {
        return 0;
    }
    return HEAP_SIZE-NNS_SndHeapGetFreeSize(SndWork->sndHeap);
}

#endif

void BL_SndPlayMusic(s32 seqNo, s32 volume)
{
    NNSSndHandle *handle = &SndWork->musHandle[MUS_HANDLE_MUSIC];
    if(seqNo != NNS_SndPlayerGetSeqNo(handle)) {
        NNS_SndArcPlayerStartSeq(handle, seqNo);
    }
    NNS_SndPlayerMoveVolume(handle, 0, 0);
    NNS_SndPlayerMoveVolume(handle, 127, volume);
    NNS_SndPlayerPause(handle, FALSE);
    NNS_SndPlayerSetTempoRatio(handle, 0x100);
    MI_CpuFill8(&SndWork->musWork, 0, sizeof(MusicWork));
}

void BL_SndStopMusic(s32 speed)
{
    NNSSndHandle *handle = &SndWork->musHandle[MUS_HANDLE_MUSIC];
    NNS_SndPlayerPause(handle, FALSE);
    NNS_SndPlayerStopSeq(handle, speed);
}

BOOL BL_SndFadeInMusic(s32 speed)
{
    NNSSndHandle *handle = &SndWork->musHandle[MUS_HANDLE_MUSIC];
    MusicWork *work = &SndWork->musWork;
    if(!NNS_SndPlayerIsPlaying(handle)) {
        return FALSE;
    }
    if(work->curMove != 0) {
        return FALSE;
    }
    work->fadeIn = TRUE;
    work->speed = speed;
    return TRUE;
}

BOOL BL_SndFadeOutMusic(s32 speed)
{
    NNSSndHandle *handle = &SndWork->musHandle[MUS_HANDLE_MUSIC];
    MusicWork *work = &SndWork->musWork;
    if(!NNS_SndPlayerIsPlaying(handle)) {
        return FALSE;
    }
    if(work->curMove != 2) {
        return FALSE;
    }
    work->fadeIn = FALSE;
    work->speed = speed;
    return TRUE;
}

#ifndef SYS_BBP

BOOL BL_SndTempoFadeMusic(s32 tempo, s32 speed)
{
    NNSSndHandle *handle = &SndWork->musHandle[MUS_HANDLE_MUSIC];
    MusicWork *work = &SndWork->musWork;
    if(!NNS_SndPlayerIsPlaying(handle)) {
        return FALSE;
    }
    if(work->curMove != 0) {
        return FALSE;
    }
    work->tempoTarget = tempo;
    work->speed = speed;
    work->fadeTempo = TRUE;
    return TRUE;
}

void BL_SndPlayMgMusic(int mgNo)
{
    static const u8 musTbl[45] = {
        21, 23, 22, 22, 27,
        23, 0, 21, 23, 27,
        26, 24, 21, 25, 21,
        26, 22, 25, 27, 25,
        25, 26, 24, 22, 26,
        25, 28, 21, 24, 27,
        23, 0, 23, 22, 22,
        23, 21, 24, 24, 24,
        26, 24, 0, 25, 24
    };
    if(musTbl[mgNo] == 0) {
        return;
    }
    BL_SndPlayMusic(musTbl[mgNo], 0);
}

#else
    
void BBP_SndSetMusicVolume(int volume)
{
    NNSSndHandle *handle = &SndWork->musHandle[MUS_HANDLE_MUSIC];
    if(volume > 127) {
        volume = 127;
    } else if(volume < 0) {
        volume = 0;
    }
    NNS_SndPlayerSetVolume(handle, volume);
}

#endif

NNSSndHandle *BL_SndGetMusicHandle(void)
{
    #ifndef SYS_BBP
    if(!SndWork) {
        return NULL;
    }
    #endif
    return &SndWork->musHandle[MUS_HANDLE_MUSIC];
}

void BL_SndPlayJingle(s32 seqNo)
{
    NNSSndHandle *handle = &SndWork->musHandle[MUS_HANDLE_JINGLE];
    NNS_SndArcPlayerStartSeq(handle, seqNo);
    NNS_SndPlayerMoveVolume(handle, 127, 0);
    
}

void BL_SndPlaySE(s32 seqArcNo, s32 seNo)
{
    NNS_SndArcPlayerStartSeqArc(&SndWork->seHandle, seqArcNo, seNo);
    #ifdef SYS_BBP
    NNS_SndPlayerSetVolume(&SndWork->seHandle, 127);
    if(BBP_DWCGetState() == 5 && BBP_DWCCheckAIDBitmap(BBP_DWCGetMyAID())) {
        NNS_SndPlayerSetVolume(&SndWork->seHandle, 32);
    }
    #endif
}

static NNSSndHandle *StartSELoop(s32 seqArcNo, s32 seNo, BOOL playMulti);
#ifndef SYS_BBP
static void StopSELoop(s32 seqArcNo, s32 seNo, NNSSndHandle *handle);
#else
static void StopSELoop(s32 seqArcNo, s32 seNo);

#endif

void BL_SndPlaySELoop(s32 seqArcNo, s32 seNo)
{
    StartSELoop(seqArcNo, seNo, TRUE);
}

void BL_SndStopSELoop(s32 seqArcNo, s32 seNo)
{
    #ifndef SYS_BBP
    StopSELoop(seqArcNo, seNo, NULL);
    #else
    StopSELoop(seqArcNo, seNo);
    #endif
}

NNSSndHandle *BL_SndPlaySELoopMulti(s32 seqArcNo, s32 seNo)
{
    StartSELoop(seqArcNo, seNo, FALSE);
}

void BL_SndStopSELoopMulti(NNSSndHandle *handle)
{
    int seqArcNo;
    int seqArcIdx;
    
    #ifndef SYS_BBP
    if(!handle) {
        return;
    }
    #endif
    
    #ifndef SYS_BBP
    seqArcNo = NNS_SndPlayerGetSeqArcNo(handle);
    if(seqArcNo < 0) {
        return;
    }
    seqArcIdx = NNS_SndPlayerGetSeqArcIdx(handle);
    if(seqArcIdx < 0) {
        return;
    }
    StopSELoop(seqArcNo, seqArcIdx, handle);
    #else
    seqArcNo = NNS_SndPlayerGetSeqArcNo(handle);
    if(seqArcNo < 0) {
        return;
    }
    seqArcIdx = NNS_SndPlayerGetSeqArcIdx(handle);
    if(seqArcIdx < 0) {
        return;
    }
    StopSELoop(seqArcNo, seqArcIdx);
    #endif
}

#ifndef SYS_BBP
static NNSSndHandle *StartSELoop(s32 seqArcNo, s32 seNo, BOOL playMulti)
{
    SndLoopSE *se;
    int i;
    se = NULL;
    for(i=0; i<SE_LOOP_MAX; i++) {
        SndLoopSE *curSE = &SndWork->loopSE[i];
        if(curSE->numPlay == 0) {
            if(se == NULL) {
                se = curSE;
            }
            if(!playMulti) {
                break;
            }
        } else if(curSE->playMulti == FALSE || !playMulti) {
            continue;
        } else {
            if(curSE->seqArcNo == seqArcNo && curSE->seNo == seNo) {
                curSE->numPlay++;
                return &curSE->handle;
            }
        }
    }
    if(!se) {
        return NULL;
    }
    se->seqArcNo = seqArcNo;
    se->seNo = seNo;
    se->numPlay = 1;
    se->playMulti = playMulti;
    NNS_SndArcPlayerStartSeqArc(&se->handle, seqArcNo, seNo);
    return &se->handle;
}
#else
static NNSSndHandle *StartSELoop(s32 seqArcNo, s32 seNo, BOOL playMulti)
{
    SndLoopSE *se;
    int i;
    se = NULL;
    for(i=0; i<SE_LOOP_MAX; i++) {
        SndLoopSE *curSE = &SndWork->loopSE[i];
        if(se == NULL && curSE->numPlay == 0) {
            se = curSE;
            if(!se->playMulti) {
                break;
            }
        } else if(curSE->playMulti == FALSE || !playMulti) {
            continue;
        } else {
            if(curSE->seqArcNo == seqArcNo && curSE->seNo == seNo) {
                curSE->numPlay++;
                return &curSE->handle;
            }
        }
    }
    if(!se) {
        return NULL;
    }
    se->seqArcNo = seqArcNo;
    se->seNo = seNo;
    se->numPlay = 1;
    se->playMulti = playMulti;
    NNS_SndArcPlayerStartSeqArc(&se->handle, seqArcNo, seNo);
    return &se->handle;
} 
#endif

#ifndef SYS_BBP
static void StopSELoop(s32 seqArcNo, s32 seNo, NNSSndHandle *handle)
{
    int i;
    for(i=0; i<SE_LOOP_MAX; i++) {
        SndLoopSE *se = &SndWork->loopSE[i];
        if(se->seqArcNo == seqArcNo && se->seNo == seNo) {
            if(handle) {
                if(handle != &se->handle) {
                    continue;
                }
                if(se->numPlay != 0) {
                    se->numPlay = 0;
                    se->playMulti = FALSE;
                    NNS_SndPlayerStopSeq(&se->handle, 0);
                    return;
                }
            }
            if(se->numPlay == 0) {
                return;
            }
            se->numPlay--;
            if(se->numPlay == 0) {
                se->playMulti = FALSE;
                NNS_SndPlayerStopSeqBySeqArcIdx(se->seqArcNo, se->seNo, FALSE);
            }
            return;
        }
        
    }
}
#else
static void StopSELoop(s32 seqArcNo, s32 seNo)
{
    int i;
    for(i=0; i<SE_LOOP_MAX; i++) {
        SndLoopSE *se = &SndWork->loopSE[i];
        if(se->seqArcNo == seqArcNo && se->seNo == seNo) {
            if(se->numPlay == 0) {
                return;
            }
            se->numPlay--;
            if(se->numPlay == 0) {
                NNS_SndPlayerStopSeqBySeqArcIdx(se->seqArcNo, se->seNo, FALSE);
            }
            return;
        }
    }
}
#endif

static void ExecMusic(void)
{
    MusicWork *work = &SndWork->musWork;
    NNSSndHandle *handle = &SndWork->musHandle[MUS_HANDLE_MUSIC];
    switch(work->mode) {
        case 0:
            work->curMove = 0;
            if(work->fadeIn) {
                NNS_SndPlayerMoveVolume(handle, 0, work->speed);
                work->curMove = 1;
                work->time = work->speed;
                work->mode = 1;
                
            } else {
                if(work->fadeTempo) {
                    work->time = 0;
                    work->mode = 4;
                    work->curMove = 4;
                }
                return;
            }
        
        case 1:
            if(work->time != 0) {
                work->time--;
                return;
            }
            NNS_SndPlayerPause(handle, TRUE);
            work->mode = 2;
            work->curMove = 2;
            break;
           
        case 2:
            if(work->fadeIn) {
                return;
            }
            NNS_SndPlayerPause(handle, FALSE);
            NNS_SndPlayerMoveVolume(handle, 127, work->speed);
            work->time = work->speed;
            work->mode = 3;
            work->curMove = 3;
        case 3:
            if(work->time) {
                work->time--;
                return;
            }
            work->mode = 0;
            work->curMove = 0;
            break;
           
        case 4:
            work->time++;
            NNS_SndPlayerSetTempoRatio(handle, (((work->tempoTarget-256)*work->time)/work->speed)+256);
            if(work->time < work->speed) {
                return;
            }
            work->time = 0;
            work->mode = 0;
            work->fadeTempo = FALSE;
            work->curMove = 0;
            break;
        default:
            MI_CpuFill8(work, 0, sizeof(MusicWork));
            break;
    }
}

static void ExecSE(void)
{
    int i;
    for(i=0; i<SE_LOOP_MAX; i++) {
        SndLoopSE *se = &SndWork->loopSE[i];
        if(!NNS_SndPlayerIsPlaying(&se->handle)) {
            se->numPlay = 0;
        }
    }
}
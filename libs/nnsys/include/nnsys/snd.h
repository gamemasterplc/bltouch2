#ifndef _NNS_SND_H
#define _NNS_SND_H

#include <nitro.h>

#define NNS_SND_ARC_SECTION_SYMBOL 0
#define NNS_SND_ARC_SECTION_INFO 1
#define NNS_SND_ARC_SECTION_FAT 2
#define NNS_SND_ARC_SECTION_FILE 3

typedef struct NNSSndHeap *NNSSndHeapHandle;
typedef struct NNSSndPlayer_s NNSSndPlayer;

typedef struct {
    NNSSndPlayer *player;
} NNSSndHandle;

typedef struct {
    u32 ofs; //0x00
    u32 size; //0x04
} NNSSndArcSection;

typedef struct {
    u32 magic; //0x00
    u16 byteOrder; //0x04
    u16 version; //0x06
    u32 size; //0x08
    u16 headerSize; //0x0C
    u16 numBlocks; //0x0E
    NNSSndArcSection sections[4]; //0x10
} NNSSndArcHeader;

typedef struct {
    u32 magic; //0x00
    u32 size; //0x04
    u32 seqSymbolOfs; //0x08
    u32 seqArcSymbolOfs; //0x0C
    u32 bankSymbolOfs; //0x10
    u32 waveArcSymbolOfs; //0x14
    u32 playerSymbolOfs; //0x18
    u32 groupSymbolOfs; //0x1C
    u32 strmPlayerSymbolOfs; //0x20
    u32 strmSymbolOfs; //0x24
} NNSSndArcSymbolSection;

typedef struct {
    u32 magic; //0x00
    u32 size; //0x04
    u32 seqInfoOfs; //0x08
    u32 seqArcInfoOfs; //0x0C
    u32 bankInfoOfs; //0x10
    u32 waveArcInfoOfs; //0x14
    u32 playerInfoOfs; //0x18
    u32 groupInfoOfs; //0x1C
    u32 strmPlayerInfoOfs; //0x20
    u32 strmInfoOfs; //0x24
} NNSSndArcInfoSection;

typedef struct {
    u32 fileOfs;
    u32 fileSize;
    void *fileAddr;
    u32 unkC;
} NNSSndArcFile;

typedef struct {
    u32 magic;
    u32 size;
    u32 numFiles;
    NNSSndArcFile files[];
} NNSSndArcFATSection;

typedef struct {
    NNSSndArcHeader header; //0x00
    BOOL isFile; //0x30
    FSFile file; //0x34
    FSFileID fileID; //0x7C
    NNSSndArcFATSection *fat; //0x84
    NNSSndArcSymbolSection *symbol; //0x88
    NNSSndArcInfoSection *info; //0x8C
} NNSSndArc;

void NNS_SndInit(void);
void NNS_SndMain(void);

void NNS_SndHandleInit(NNSSndHandle *handle);
void NNS_SndHandleReleaseSeq(NNSSndHandle *handle);

void NNS_SndPlayerPause(NNSSndHandle *handle, BOOL pause);
BOOL NNS_SndArcPlayerStartSeq(NNSSndHandle *handle, s32 seqIndex);
BOOL NNS_SndArcPlayerStartSeqArc(NNSSndHandle *handle, s32 seqArc, s32 seIndex);
BOOL NNS_SndPlayerStopSeqBySeqArcIdx(s32 seqArc, s32 seIndex, BOOL pause);
int NNS_SndPlayerGetSeqArcNo(NNSSndHandle *handle);
int NNS_SndPlayerGetSeqArcIdx(NNSSndHandle *handle);

void NNS_SndPlayerStopSeq(NNSSndHandle *handle, BOOL stop);
void NNS_SndPlayerStopSeqAll(BOOL stop);
void NNS_SndPlayerMoveVolume(NNSSndHandle *handle, s32 to, s32 speed);
void NNS_SndPlayerSetTempoRatio(NNSSndHandle *handle, s32 ratio);
void NNS_SndPlayerSetVolume(NNSSndHandle *handle, int volume);

s32 NNS_SndPlayerGetSeqNo(NNSSndHandle *handle);

NNSSndHeapHandle NNS_SndHeapCreate(void *ptr, u32 size);
void NNS_SndHeapDestroy(NNSSndHeapHandle heap);
void NNS_SndHeapLoadState(NNSSndHeapHandle heap, s32 maxLoad);
BOOL NNS_SndHeapSaveState(NNSSndHeapHandle heap);
s32 NNS_SndHeapGetFreeSize(NNSSndHeapHandle heap);

BOOL NNS_SndArcLoadGroup(s32 id, NNSSndHeapHandle heap);

BOOL NNS_SndArcPlayerSetup(NNSSndHeapHandle heap);

void NNS_SndArcInit(NNSSndArc *sndArc, char *path, NNSSndHeapHandle heap, BOOL flag);
void NNS_SndArcInitOnMemory(NNSSndArc *sndArc, void *ptr);

NNSSndArc *NNS_SndArcSetCurrent(NNSSndArc *sndArc);

static inline BOOL NNS_SndPlayerIsPlaying(NNSSndHandle *handle)
{
    return handle->player != NULL;
}

#endif
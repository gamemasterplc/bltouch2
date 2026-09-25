#ifndef MAP_SAVE_H
#define MAP_SAVE_H

#include "bl_types.h"

#define MAP_SAVE_WORK 0
#define MAP_SAVE_BACKUP 1

typedef struct {
	u32 existFlag : 8; //Bits 0-7
	u32 beatFlag : 8; //Bits 8-15
	u32 unk1 : 1; //Bit 16
	u32 unk2 : 1; //Bit 17
	u32 escort : 3; //Bits 18-20
	u32 waiting : 1; //Bit 21
	u32 unk3 : 1; //Bit 22
	u32 unk4 : 3; //Bits 23-25
} MapSaveMgRival;

typedef struct {
	u8 complete : 1; //Bit 0
	u8 defeat : 1; //Bit 1
	u8 completeToken : 1; //Bit 2
	u8 playNum : 4; //Bits 3-6
	u8 unk1 : 1; //Bit 7
} MapSaveAtrcFlag;

typedef struct {
	s8 index; //0x00
	s8 param; //0x01
} MapSaveUranai;

typedef struct {
	u32 mg01Record : 14;
	u32 mg02Record : 14;
	u32 mg03RecordHi : 4;
    u32 mg03RecordLo : 10;
	u32 mg04Record : 14;
    u32 mg05RecordHi : 8;
    u32 mg05RecordLo : 6;
	u32 mg06Record : 14;
    u32 mg07RecordHi : 12;
    u32 mg07RecordLo : 2;
	u32 mg08Record : 14;
	u32 mg09Record : 14;
    u32 mg10RecordHi : 2;
    u32 mg10RecordLo : 12;
	u32 mg11Record : 14;
    u32 mg12RecordHi : 6;
    u32 mg12RecordLo : 8;
	u32 mg13Record : 14;
    u32 mg14RecordHi : 10;
    u32 mg14RecordLo : 4;
	u32 mg15Record : 14;
	u32 mg16Record : 14;
	u32 mg17Record : 14;
	u32 mg18Record : 14;
	u32 mg19RecordHi : 4;
    u32 mg19RecordLo : 10;
	u32 mg20Record : 14;
    u32 mg21RecordHi : 8;
    u32 mg21RecordLo : 6;
	u32 mg22Record : 14;
    u32 mg23RecordHi : 12;
    u32 mg23RecordLo : 2;
	u32 mg24Record : 14;
	u32 mg25Record : 14;
    u32 mg26RecordHi : 2;
    u32 mg26RecordLo : 12;
	u32 mg27Record : 14;
    u32 mg28RecordHi : 6;
    u32 mg28RecordLo : 8;
	u32 mg29Record : 14;
    u32 mg30RecordHi : 10;
    u32 mg30RecordLo : 4;
	u32 mg31Record : 14;
	u32 mg32Record : 14;
	u32 mg33Record : 14;
	u32 mg34Record : 14;
	u32 mg35RecordHi : 4;
    u32 mg35RecordLo : 10;
	u32 mg36Record : 14;
    u32 mg37RecordHi : 8;
    u32 mg37RecordLo : 6;
	u32 mg38Record : 14;
    u32 mg39RecordHi : 12;
    u32 mg39RecordLo : 2;
	u32 mg40Record : 14;
	u32 mg41Record : 14;
    u32 mg42RecordHi : 2;
    u32 mg42RecordLo : 12;
	u32 mg43Record : 14;
    u32 mg44RecordHi : 6;
    u32 mg44RecordLo : 8;
	u32 mg45Record : 14;
} MapSaveAtrcRecord;

typedef struct {
	u8 star : 1;
	u8 moon : 1;
	u8 sun : 1;
	u8 earth : 1;
} MapSaveZoneVisit;

typedef struct {
	u16 playerFlag; //0x00
	u32 starPieceFlag; //0x04
	u32 moonPieceFlag; //0x08
	u32 sunPieceFlag; //0x0C
	u32 earthPieceFlag; //0x10
	u16 lightningPieceFlag; //0x14
	u16 rainbowPieceFlag; //0x16
	u16 heartPieceFlag; //0x18
	u8 keyPieceFlag; //0x1A
	s16 numTokens; //0x1C
	u16 unk1E;
	u16 padFlag; //0x20
	u16 mailKuro; //0x22
	u16 mailPretty; //0x24
	u16 mailGold; //0x26
	u16 mailAo; //0x28
	u16 mailMidori; //0x2A
	u16 mailAqua; //0x2C
	u32 mailStar; //0x30
	u16 mailThief; //0x34
	u16 mailZako; //0x36
	u16 mailReadFlag[6]; //0x38
	u16 stampFlag; //0x44
	u16 starA1P1Flag; //0x46
	u16 starA1P2Flag; //0x48
	u16 starA1P3Flag; //0x4A
	u16 starA1P4Flag; //0x4C
	u16 starA2P1Flag; //0x4E
	u16 starA2P2Flag; //0x50
	u16 starA2P3Flag; //0x52
	u16 starA2P4Flag; //0x54
	u16 starA2P5Flag; //0x56
	u16 moonA1P1Flag; //0x58
	u16 moonA1P2Flag; //0x5A
	u16 moonA1P3Flag; //0x5C
	u16 moonA1P4Flag; //0x5E
	u16 moonA1P5Flag; //0x60
	u16 moonA2P1Flag; //0x62
	u16 moonA2P2Flag; //0x64
	u16 moonA2P3Flag; //0x66
	u16 sunA1P1Flag; //0x68
	u16 sunA1P1AltFlag; //0x6A
	u16 sunA1P2Flag; //0x6C
	u16 sunA1P3Flag; //0x6E
	u16 sunRideFlag[5]; //0x70
	u16 sunA1P4Flag; //0x7A
	u16 sunA2P1Flag; //0x7C
	u16 sunA2P2Flag; //0x7E
	u16 sunA2P3Flag; //0x80
	u16 sunA2P4Flag; //0x82
	u32 unk84;
	u32 earthA1P1Flag; //0x88
	u16 earthA1P2Flag; //0x8C
	u16 earthA1P3Flag; //0x8E
	u16 earthA1P4Flag; //0x90
	u16 earthA2P1Flag; //0x92
	u16 earthA2P2Flag; //0x94
	u16 earthA2P3Flag; //0x96
	u16 earthA2P4Flag; //0x98
	u16 earthA2P5Flag; //0x9A
	u16 chrShiroFlag; //0x9C
	u16 chrKuroFlag; //0x9E
	u32 chrPrettyFlag; //0xA0
	u32 chrGoldFlag; //0xA4
	u16 chrEvilFlag; //0xA8
	u16 chrAoFlag; //0xAA
	u16 chrMidoriFlag; //0xAC
	u16 miniFlag; //0xAE
	u16 magicFlag; //0xB0
	u16 bossJackFlag; //0xB2
	u16 bossQueenFlag; //0xB4
	u16 bossAceFlag; //0xB6
	u16 bossEvilFlag; //0xB8
	u16 thiefFlag; //0xBA
	u16 ninjaFlag; //0xBC
	u16 sageFlag; //0xBE
	u16 mummyFlag; //0xC0
	u16 uranaiFlag; //0xC2
	u16 mgHypnotizeFlag; //0xC4
	u16 starFlag; //0xC6
	u16 moonFlag; //0xC8
	u16 sunFlag; //0xCA
	u16 earthFlag; //0xCC
	u32 areaVisitFlag; //0xD0
	MapSaveMgRival rival[7]; //0xD4
	u16 nextStampFlag; //0xF0
	u8 menuPage; //0xF2
	u16 zoneEvilFlag; //0xF4
	u16 zoneEnterFlag; //0xF6
	u16 zoneExitFlag; //0xF8
	u16 eventFlag; //0xFA
	u16 eventTouchFlag; //0xFC
	u16 unkFE;
	u16 bonusFlag; //0x100
	u16 unk102;
	u16 bonusFlag2; //0x104
	u16 unk106;
	MapSaveAtrcFlag atrcFlag[45]; //0x108
	MapSaveAtrcFlag bhouseFlag[4]; //0x135
	//Flags at 0x13C
	u32 itemHeadFlag : 6; //Bits 0-5
	u32 itemBodyFlag : 8; //Bits 6-13
	u32 itemBombFlag : 8; //Bits 14-21
	u32 itemTicketFlag : 8; //Bits 22-29
	u32 : 2;
	//Flags at 0x140
	u32 itemHeadCur : 6; //Bits 0-5
	u32 itemBodyCur : 8; //Bits 6-13
	u32 itemBombCur : 8; //Bits 14-21
	u32 itemTicketCur : 8; //Bits 22-29
	u32 : 2;
	//Flags at 0x144
	u32 messSpeed : 2; //Bits 0-1
	u32 shortcut : 1; //Bit 2
	MapSaveUranai uranai[3]; //0x148
	MapSaveZoneVisit visit; //0x14E
	MapSaveZoneVisit visitInforme; //0x14F
	//Quest progress fields at 0x150
	u32 jackBossProgress : 2; //Bits 0-1
	u32 queenBossProgress : 1; //Bit 2
	u32 charKeyProgress : 2; //Bits 3-4
	u32 evilProgress : 3; //Bits 5-7
	u32 greenKeyProgress : 2; //Bits 8-9
	u32 aoFluteProgress : 2; //Bits 10-11
	u32 aoBasketballProgress : 3; //Bits 12-14
	u32 aoAceSwimProgress : 3; //Bits 15-17
	u32 midoriFluteProgress : 2; //Bits 18-19
	u32 midoriPuzzleProgress : 3; //Bits 20-22
	u32 uranaiAquaProgress : 1; //Bit 23
	u32 uranaiPrettyProgress : 1; //Bit 24
	u32 prettyQuestProgress : 3; //Bits 25-27
	u32 aquaQuestProgress : 3; //Bits 28-30
	MapSaveAtrcRecord atrcRecord; //0x154
} MapSave; //size=0x1A4

extern u8 MapSave_ItemHeadBitTbl[6];
extern u8 MapSave_ItemHeadOrderTbl[6];

extern u8 MapSave_ItemBodyBitTbl[8];
extern u8 MapSave_ItemBodyOrderTbl[8];

extern u8 MapSave_ItemBombBitTbl[8];
extern u8 MapSave_ItemBombOrderTbl[8];

extern u8 MapSave_ItemTicketBitTbl[8];

MapSaveAtrcRecord *MapSave_GetAtrcRecord_(int saveType);

u8 MapSave_GetFlagArea(int flag);

BLBool MapSave_GetFlag_(int flag, int saveType);
void MapSave_SetFlag_(int flag, int saveType);
void MapSave_ResetFlag_(int flag, int saveType);
void MapSave_SetPieceFlag_(int type, int piece, int saveType);
u32 MapSave_GetPieceFlag_(int type, int saveType);
int MapSave_GetPieceNum_(int type, int saveType);
void MapSave_CollectToken_(int type, int token, int saveType);
void MapSave_AddToken_(s16 num, int saveType);
s16 MapSave_GetNumTokens_(int saveType);
int MapSave_GetNumStamps_(int saveType);
void MapSave_SetAtrcFlag_(int atrcNo, int type, BLBool on, int saveType);
BLBool MapSave_GetAtrcFlag_(int atrcNo, int type, int saveType);
u8 MapSave_GetNumAtrcFlag_(int type, int saveType);
void MapSave_AddAtrcPlayNum_(int atrcNo, u8 num, int saveType);
u8 MapSave_GetAtrcPlayNum_(int atrcNo, int saveType);
void MapSave_SetBHouseFlag_(int bhouseNo, int type, BLBool on, int saveType);
BLBool MapSave_GetBHouseFlag_(int bhouseNo, int type, int saveType);
void MapSave_AddBHousePlayNum_(int bhouseNo, u8 num, int saveType);
u8 MapSave_GetBHousePlayNum_(int bhouseNo, int saveType);
void MapSave_SetItemFlag_(int type, int bit, BLBool reset, int saveType);
BLBool MapSave_GetItemFlag_(int type, int bit, int saveType);
u8 MapSave_GetNumItem_(int type, int saveType);
void MapSave_SetCurItem_(int type, u8 no, int saveType);
u8 MapSave_GetCurItem_(int type, int saveType);
void MapSave_SetSunRideJetFlag_(u16 flag, int saveType);
BLBool MapSave_GetSunRideJetFlag_(u16 flag, int saveType);
u16 MapSave_GetNumSunRideStars_(int saveType);
void MapSave_SetMenuPage_(u8 page, int saveType);
u8 MapSave_GetMenuPage_(int saveType);
void MapSave_SetMessSpeed_(u8 speed, int saveType);
u8 MapSave_GetMessSpeed_(int saveType);
void MapSave_SetMenuShortcut_(u8 shortcut, int saveType);
u8 MapSave_GetMenuShortcut_(int saveType);
void MapSave_SetMailReadFlag_(u16 flag, int saveType);
BLBool MapSave_GetMailReadFlag_(u16 flag, int saveType);
void MapSave_SetZoneVisitFlag_(int type, int zoneType, int saveType);
BLBool MapSave_GetZoneVisitFlag_(int type, int zoneType, int saveType);
void MapSave_IncQuestProgress_(int questType, int saveType);
u8 MapSave_GetQuestProgress_(int questType, int saveType);
 
static inline MapSaveAtrcRecord *MapSave_GetAtrcRecordBackup(void)
{
	return MapSave_GetAtrcRecord_(MAP_SAVE_BACKUP);
}

static inline BLBool MapSave_GetFlag(int flag)
{
	return MapSave_GetFlag_(flag, MAP_SAVE_WORK);
}

static inline BLBool MapSave_GetFlagBackup(int flag)
{
	return MapSave_GetFlag_(flag, MAP_SAVE_BACKUP);
}

static inline void MapSave_SetFlag(int flag)
{
	MapSave_SetFlag_(flag, MAP_SAVE_WORK);
}

static inline void MapSave_SetFlagBackup(int flag)
{
	MapSave_SetFlag_(flag, MAP_SAVE_BACKUP);
}

static inline void MapSave_ResetFlag(int flag)
{
	MapSave_SetFlag_(flag, MAP_SAVE_WORK);
}

static inline void MapSave_ResetFlagBackup(int flag)
{
	MapSave_SetFlag_(flag, MAP_SAVE_BACKUP);
}


#endif
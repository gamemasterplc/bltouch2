#ifndef MAP_H
#define MAP_H

#include "map_save.h"
#include "map_defs.h"
#include "file.h"
#include "sprite.h"
#include "vram.h"
#include "color.h"
#include "object.h"

#include "bl_types.h"

typedef struct {
	u8 id; //0x00
	u8 moveType; //0x01
	BLBool enable; //0x02
	u8 unk3;
	s8 dir; //0x04
	BLVec2 pos; //0x08
	u8 speed; //0x10
} MapSpawnChr;

typedef struct {
	u8 area; //0x00
	u8 prevArea; //0x01
	u8 playerChr; //0x02
	u8 unk3;
	u8 playerDir; //0x04
	BLIVec2 playerPos; //0x06
	MapSpawnChr chr[MAP_CHR_MAX]; //0x0C
	MapSave save; //0x110
	u8 mail[MAP_MAIL_MAX]; //0x2B4
	u8 escortAvail; //0x308
	u8 escortChr; //0x309
	u32 unk30C;
	u32 unk310;
	u32 sceneCall; //0x314
	u32 sceneParam; //0x318
	u8 mgCall; //0x31C
	u16 unk31E;
	u16 atrcWinNum; //0x320
} MapBackup; //size=0x324

typedef struct {
	u8 animDir; //0x0
	u16 nextAnim; //0x2
	s16 turnDelay; //0x4
	BLFile *colorFile; //0x8
	BLFile *imageFile; //0xC
	BLFile *animFile; //0x10
	u32 unk14;
	BLSprite *mainSpr; //0x18
	BLVramImageKey imageKey; //0x1C
	BLColorKey colorKey; //0x24
	//TODO: Fix type once tone.c decompiled
	u16 tone; //0x28
} MapSprite;

typedef struct {
	BLVec2 unk0;
	u8 id; //0x08
	u8 moveType; //0x09
	BLBool enable; //0x0A
	u8 unkB;
	s8 dir; //0x0C
	BLVec2 pos; //0x10
	BLVec2 posScreen; //0x18
	BLVec2 posNext; //0x20
	u8 speed; //0x28
	u8 prevSpeed; //0x29
	fx32 curSpeed; //0x2C
	BLVec2 vel; //0x30
	u8 colType; //0x38
	u16 unk3A; 
	BLIVec2 tilePosX; //0x3C
	u32 tileCol1[6][8]; //0x40
	fx32 targetAngle; //0x100
	fx32 targetSpeed; //0x104
	MapSprite spr; //0x108
	MapSprite sprOth; //0x134
} MapChr;

typedef struct {
	s8 curPath; //0x00
	s8 unk1;
	u8 speed; //0x02
	BLBool pause; //0x03
	u8 movePath; //0x04
	BLIVec2 movePos; //0x06
	u8 nextPath; //0x0A
	BLIVec2 nextPos; //0x0C
	s16 delay; //0x10
	s16 pathDelay; //0x12
} MapChrPath;

typedef struct {
	MapSprite spr; //0x00
} MapObj;

typedef struct {
	MapSprite spr[2]; //0x00
} MapGate;

typedef struct {
	BLFile *file[4]; //0x00
	BLSprTex *spr; //0x10
	BLVramTexKey texKey; //0x14
	BLColorKey colorKeyTone; //0x1C
	BLColorKey colorKey; //0x20
	u16 toneId; //0x24
} MapTouchSpr;

typedef struct {
	s8 type; //0x00
	u8 id; //0x01
	s8 subType; //0x02
	u16 index; //0x04
	BLVec2 pos; //0x08
	BLVec2 screenPos; //0x10
	MapTouchSpr spr[MAP_TOUCH_SPR_MAX]; //0x18
} MapTouch;

typedef struct {
	MapSprite spr;
} MapEvSpr;

typedef struct {
	u8 type; //0x00
	BLVec2 pos; //0x04
} MapEvSprInfo;

typedef struct {
	u8 type; //0x00
	BLVec2 pos; //0x04
} MapEvItemInfo;

typedef struct {
	u8 animDir; //0x0
	u16 nextAnim; //0x2
	s16 turnDelay; //0x4
	BLFile *animFile; //0x8
	u32 unkC;
	BLSprTex *spr; //0x10
	BLVramImageKey imageKey; //0x14
	BLColorKey colorKey; //0x1C
	u32 unk20; //0x20
	//TODO: Fix type once tone.c decompiled
	u16 tone; //0x24
} MapEvItem;

typedef struct {
	u16 sceneState; //0x00
	u16 unk2;
	u32 unk4;
	u16 unk8;
	u16 unkA;
	u32 unkC;
	u8 area; //0x10
	u8 prevArea; //0x11
	u8 unk12;
	u8 playerChr; //0x13
	u8 prevArea2; //0x14
	u8 playerDir; //0x15
	BLIVec2 playerPos; //0x16
	BLFile *colFile; //0x1C
	BLIVec2 mapSize; //0x20
	u8 cameraLock; //0x24
	u8 unk25_0 : 1;
	u8 unk25_1 : 1;
	BLVec2 cameraPos; //0x28
	BLVec2 unk30;
	
	u8 touchOn : 1; //0x38 Bit 1
	u8 touchEnable : 1; //0x38 Bit 2
	u16 unk3A;
	u16 touchX; //0x3C
	u16 touchY; //0x3E
	MapChr chr[MAP_CHR_MAX]; //0x40
	MapChrPath chrPath[MAP_CHR_MAX]; //0x1220
	MapSave save; //0x1324
	u8 exitArea; //0x14C8
	u8 exitDir; //0x14C9
	BLIVec2 exitPos; //0x14CA
	s8 nextExitDir; //0x14CE
	u8 unk14CF;
	u8 mail[MAP_MAIL_MAX]; //0x14D0
	u8 escortAvail; //0x1524
	u8 escortChr; //0x1525
	s16 escortStopTimer; //0x1526
	u8 origBgPrio[4]; //0x1528
	BLVec2 escortPosHis[MAP_ESCORT_HIS_MAX]; //0x152C
	fx32 escortAngleHis[MAP_ESCORT_HIS_MAX]; //0x15CC
	u8 exitType; //0x161C
	u8 exitMg; //0x161D
	u32 sceneCall; //0x1620
	u32 sceneParam; //0x1624
	s32 fadeBrightness; //0x1628
	union { //0x162C
		u8 sceneWorkU8[28];
		s8 sceneWorkS8[28];
		u32 sceneWorkU32[7];
		s32 sceneWorkS32[7];
	};
	u8 evState; //0x1648
	u8 evNext; //0x1649
	u8 evNextChrState; //0x164A
	s8 evObj; //0x164B
	s8 evArg; //0x164C
	s8 evBonus; //0x164D
	s16 evTimer; //0x164E
	u32 unk1650;
	BLIVec2 evPos; //0x1654
	BLIVec2 evEscapePos; //0x1658
	BLIVec2 evEscortEscapePos; //0x165C
	u32 *curScript; //0x1660
	BLObj *sceneObj; //0x1664
	BLObj *subObj; //0x1668
	u32 unk166C;
	u32 unk1670;
	BLVec2 bgOfs[4]; //0x1674
	fx32 farBGOfs; //0x1694
	fx32 farBGSpeed; //0x1698
	BLColorKey bgToneColorKey; //0x169C
	BLFile *bgToneFile[3]; //0x16A0
	u16 bgToneId[3]; //0x16AC
	BLVec2 earthRideBGOfs; //0x16B4
	BLColorKey bgColorKey; //0x16BC
	BLFile *bgToneFile2[3]; //0x16C0
	u16 bgToneId2[3]; //0x16CC
	u8 unk16D2[2];
	s8 objIdx[MAP_OBJ_MAX]; //0x16D4
	MapObj obj[MAP_OBJ_MAX]; //0x1708
	BLVec2 dynObjPos[MAP_DYNOBJ_MAX]; //0x1FF8
	MapObj dynObj[MAP_DYNOBJ_MAX]; //0x2158
	MapObj bgObj[MAP_BGOBJ_MAX]; //0x28E8
	MapGate gate[MAP_GATE_MAX]; //0x2998
	MapTouch touch[MAP_TOUCH_MAX]; //0x2C58
	MapEvSpr evSpr[MAP_EVSPR_MAX]; //0x3718
	MapEvItem evItem[MAP_EVITEM_MAX]; //0x3878
	MapEvSprInfo evSprInfo[MAP_EVSPR_MAX]; //0x39B8
	MapEvItemInfo evItemInfo[MAP_EVITEM_MAX]; //0x3A18
	u16 atrcWinNum; //0x3A78
} MapWork; //size=0x3A7C

extern MapChrPath Map_ChrPath[MAP_CHR_MAX]; //0x020AD058
extern MapBackup Map_Backup; //0x020AD15C
extern MapWork Map_Work; //0x020AD480
extern MapBackup Map_BackupCopy; //0x020B0EFC

void Map_SaveInit(void);
void Map_SaveFlagInit(void);
void Map_SaveBackup(void);
void Map_BackupPlayerPos(void);
void Map_MovePlayerMgBonus(int mgNo);
void Map_LoadBackup(void);
void Map_WriteBackup(int slot);
void Map_ReadBackup(int slot);
int Map_GetZoneType(u8 zone);

int Map_GetCurZoneType_(int saveType);
int Map_GetPrevZoneType_(int saveType);
BOOL Map_GetChrPosInt(int chrNo, s16 *outX, s16 *outY);
u8 Map_GetSubWakuType_(int saveType);
u8 Map_GetZoneArea_(int saveType);
u8 Map_GetCurZone_(int saveType);
u8 Map_GetZone(int saveType);
void Map_AddAtrcWinNum(int num);
int Map_GetAtrcWinNum(void);
BLBool Map_IsPieceFull(int flagType);

static inline int Map_GetCurZoneType(void)
{
	return Map_GetCurZoneType_(MAP_SAVE_WORK);
}

static inline int Map_GetCurZoneTypeBackup(void)
{
	return Map_GetCurZoneType_(MAP_SAVE_BACKUP);
}

static inline int Map_GetPrevZoneType(void)
{
	return Map_GetPrevZoneType_(MAP_SAVE_WORK);
}

static inline int Map_GetPrevZoneTypeBackup(void)
{
	return Map_GetPrevZoneType_(MAP_SAVE_BACKUP);
}

#endif
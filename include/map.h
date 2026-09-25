#ifndef MAP_H
#define MAP_H

#include "map_save.h"
#include "map_defs.h"
#include "file.h"
#include "sprite.h"
#include "vram.h"
#include "color.h"
#include "bl_types.h"


typedef struct {
	u8 id; //0x00
	BLBool active; //0x01
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
	s16 playerPosX; //0x06
	s16 playerPosY; //0x08
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
	u16 atrcWinNum;
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
	//TODO: Fix type once coloranim.c decompiled
	u16 colorAnim; //0x28
} MapSprite;

typedef struct {
	BLVec2 unk0;
	u8 curPath; //0x08
	BLBool active; //0x09
	BLBool disable; //0x0A
	u8 unkB;
	s8 dir; //0x0C
	BLVec2 pos; //0x10
	BLVec2 posScreen; //0x18
	BLVec2 posNext; //0x20
	u8 speed; //0x28
	u8 prevSpeed; //0x29
	fx32 speedVal; //0x2C
	BLVec2 vel; //0x30
	u8 colType; //0x38
	u16 unk3A; 
	s16 tilePosX; //0x3C
	s16 tilePosY; //0x3E
	u32 tileCol1[6][8]; //0x40
	BLVec2 pathVel; //0x100
	MapSprite spr; //0x108
	MapSprite sprOth; //0x134
} MapChr;

typedef struct {
	s8 curPath; //0x00
	s8 unk1;
	u8 speed; //0x02
	BLBool pause; //0x03
	u8 movePath; //0x04
	s16 movePosX; //0x06
	s16 movePosY; //0x08
	u8 nextPath; //0x0A
	s16 nextPosX; //0x0C
	s16 nextPosY; //0x0E
	s16 delay; //0x10
	s16 pathDelay; //0x12
} MapChrPath;

typedef struct {
	u16 sceneState; //0x00
	u16 unk2;
	u32 unk4;
	u16 unk8;
	u16 unkA;
	u32 unkC;
	u8 area; //0x10
	u8 prevArea; //0x11
	u16 unk12;
	u8 prevArea2; //0x14
	u8 dir; //0x15
	s16 playerPosX; //0x16
	s16 playerPosY; //0x18
	BLFile *colFile; //0x1C
	s16 mapWidth; //0x20
	s16 mapHeight; //0x22
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
	
} MapWork; //size=0x3A7C

extern MapBackup Map_Backup; //0x020AD15C
extern MapWork Map_Work; //0x020AD480

int Map_GetCurZoneType(BOOL isBackup);

#endif
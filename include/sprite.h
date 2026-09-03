#ifndef SPRITE_H
#define SPRITE_H

#include "res_anim.h"
#include "res_image.h"
#include "display.h"
#include <nitro/fx.h>

#ifdef SYS_BBP
#define BL_SprInit BBP_SprInit
#define BL_SprClose BBP_SprClose
#define BL_SprCreate BBP_SprCreate
#define BL_SprKill BBP_SprKill
#define BL_SprSetAnim BBP_SprSetAnim
#define BL_SprSetPrio BBP_SprSetPrio
#define BL_SprSetLayerPrio BBP_SprSetLayerPrio
#define BL_SprExec BBP_SprExec
#endif

#define BL_SPR_ANIM_NULL 0xFF

typedef struct BLSprite_s BLSprite;

typedef struct BLSprAnim_s {
	s16 x; //0x0
	s16 y; //0x2
	s16 centerX; //0x4
	s16 centerY; //0x6
	s32 scaleX; //0x8
	s32 scaleY; //0xC
	u16 angle; //0x10
	u16 animNo; //0x12
	u16 nextAnimNo; //0x14
	u16 frameTime; //0x16
	u8 animFrame; //0x18
	u8 animSpeed; //0x19
	u8 animPlayMax; //0x1A
	u8 animPlayNo; //0x1B
	//Flags at 0x1C
	unsigned palette : 4; //Bits 0-3
	unsigned palOfs2 : 4; //Bits 4-7
	unsigned dispOff : 1; //Bit 8
	unsigned animPause : 1; //Bit 9
	unsigned flipX : 1; //Bit 10
	unsigned flipY : 1; //Bit 11
	unsigned animDone : 1; //Bit 12
	unsigned frameDone : 1; //Bit 13
	unsigned animNextStart : 1; //Bit 14
	unsigned animLayer : 4; //Bits 15-18
	u32 imageAddr; //0x20
	BLResAnim *animRes; //0x24
} BLSprAnim;

struct BLSprite_s {
	BLSprAnim base; //Must be at 0x00
	BLResImage *imageRes; //0x28
	//Flags at 0x2C
	u8 useAlpha : 1;
	u8 isObjWindow : 1;
	u8 useMosaic : 1;
	u8 imageDirty : 1;
	BLSprite *animSpr; //0x30
	BLSprite *animSprNext; //0x34
	BLSprite *inheritList; //0x38
	
};

typedef struct BLSpr3D_s {
	BLSprAnim base; //Must be at 0x00
	u32 colorAddr; //0x28
	u8 alpha : 5; //0x2C
} BLSpr3D;

typedef struct BLSprRaw_s {
	s16 x; //0x0
	s16 y; //0x2
	s16 centerX; //0x4
	s16 centerY; //0x6
	s32 scaleX; //0x8
	s32 scaleY; //0xC
	s16 angle; //0x10
	u32 imageAddr; //0x14
	u32 colorAddr; //0x18
	//Flags at 0x1C
	u16 width : 3;
	u16 height : 3;
	u16 : 1;
	u16 flipX : 1;
	u16 flipY : 1;
	u16 alpha : 5;
	u16 is8BPP : 1;
} BLSprRaw;

void BL_SprInit(void);
void BL_SprClose(void);
BLSprite *BL_SprCreate(int screen, u8 layer, u16 prio, u32 imageAddr, u8 palette, BLResAnim *animRes, BLResImage *imageRes);
void BL_SprKill(BLSprite *spr);
void BL_SprSetAnim(BLSprite *spr, u16 animNo, u8 animFrame);

void BL_SprSetPrio(BLSprite *spr, u16 prio);
void BL_SprSetLayerPrio(BLSprite *spr, u8 layer, u16 prio);
u16 BL_SprGetPrio(BLSprite *spr);

#ifndef SYS_BBP
void BL_SprSetLayer(BLSprite *spr, u8 layer);
BLSpr3D *BL_Spr3DCreate(u16 prio, u32 imageAddr, u32 colorAddr, BLResAnim *animRes);
void BL_Spr3DKill(BLSpr3D *spr);
void BL_Spr3DSetAnim(BLSpr3D *spr, u16 animNo, u8 animFrame);
void BL_Spr3DSetPrio(BLSpr3D *spr, u16 prio);
BLSprRaw *BL_SprRawCreate(u16 prio, u32 imageAddr, u32 colorAddr, u8 width, u8 height, u8 is8BPP);
void BL_SprRawKill(BLSprRaw *spr);
#endif

void BL_SprExec(void);

#endif
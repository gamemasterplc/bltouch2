#ifndef OBJECT_H
#define OBJECT_H

#include <nitro/types.h>

#ifdef SYS_BBP
#define BL_ObjInit BBP_ObjInit
#define BL_ObjCur BBP_ObjCur
#define BL_ObjCreate BBP_ObjCreate
#define BL_ObjExec BBP_ObjExec
#define BL_ObjCurKill BBP_ObjCurKill
#define BL_ObjKill BBP_ObjKill
#define BL_ObjCreateChild BBP_ObjCreateChild
#define BL_ObjKillChild BBP_ObjKillChild
#define BL_ObjCurSetFunc BBP_ObjCurSetFunc
#define BL_ObjSetFunc BBP_ObjSetFunc
#define BL_ObjResetPauseFlag BBP_ObjResetPauseFlag
#define BL_ObjNullExec BBP_ObjNullExec
#endif

typedef struct BLObj_s BLObj;

typedef void (*BLObjFunc)(void);

struct BLObj_s {
    BLObj *prev; //0x00
    BLObj *next; //0x04
    BLObj *parent; //0x08
    BLObj *childFirst; //0x0C
    BLObj *childPrev; //0x10
    BLObj *childNext; //0x14
    BLObjFunc func; //0x18
    BLObjFunc destroy; //0x1C
    u32 pauseFlag; //0x20
    u16 prio; //0x24
    u16 delay; //0x26
    u16 prioNew; //0x28
    u32 work[4]; //0x2C
    u16 valid : 1; //0x3C Bit 0
    u16 lock : 1; //0x3C Bit 1
    u16 kill : 1; //0x3C Bit 2
    u16 needSort : 1; //0x3C Bit 3
    void *data; //0x40
};

extern BLObj *BL_ObjCur;

void BL_ObjInit(void);
void BL_ObjExec(void);
BLObj *BL_ObjCreate(BLObjFunc func, u16 prio, u32 pauseFlag, void *data, BLObjFunc destroy);
void BL_ObjCurKill(void);
void BL_ObjKill(BLObj *obj);
BLObj *BL_ObjCreateChild(BLObj *parent, BLObjFunc func, u16 prio, u32 pauseFlag, void *data, BLObjFunc destroy);
void BL_ObjCurSetFunc(BLObjFunc func);
void BL_ObjSetFunc(BLObj *obj, BLObjFunc func);
void BL_ObjResetPauseFlag(u32 flag);

void BL_ObjNullExec(void);

#ifndef SYS_BBP
void BL_ObjCurSetPrio(u16 prio);
void BL_ObjSetPrio(BLObj *obj, u16 prio);
void BL_ObjSetPauseFlag(u32 flag);
BLObj *BL_ObjCurCreateChild(BLObjFunc func, u16 prio, u32 pauseFlag, void *data, BLObjFunc destroy);

#else
void BL_ObjKillChild(BLObj *obj);
#endif


#endif
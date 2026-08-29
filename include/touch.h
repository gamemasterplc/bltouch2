#ifndef TOUCH_H
#define TOUCH_H

#include <nitro.h>

#ifdef SYS_BBP
#define BL_TouchInit BBP_TouchInit
#define BL_TouchExec BBP_TouchExec
#define BL_TouchGetDown BBP_TouchGetDown
#define BL_TouchGetDownPlayer BBP_TouchGetDownPlayer
#define BL_TouchGetHold BBP_TouchGetHold
#define BL_TouchGetHoldPlayer BBP_TouchGetHoldPlayer
#define BL_TouchRead BBP_TouchRead
#endif

typedef struct BLTouchData_s {
    u8 x;
    u8 y;
    u8 hold;
    u8 down;
} BLTouchData;


void BL_TouchInit(void);
void BL_TouchExec(void);
BOOL BL_TouchGetDown(u8 *x, u8 *y);
BOOL BL_TouchGetDownPlayer(u8 playerNo, u8 *x, u8 *y);
BOOL BL_TouchGetHold(u8 *x, u8 *y);
BOOL BL_TouchGetHoldPlayer(u8 playerNo, u8 *x, u8 *y);
u8 BL_TouchRead(TPData *tp);

#ifndef SYS_BBP
void BL_TouchGetHis(u8 index, BLTouchData *out);
void BL_TouchGetHisPlayer(u8 playerNo, u8 index, BLTouchData *out);
#else
BOOL BBP_TouchCheckCircleHold(u8 playerNo, int cx, int cy, int radius);
BOOL BBP_TouchCheckCircleDown(u8 playerNo, int cx, int cy, int radius);
BOOL BBP_TouchCheckRectHold(u8 playerNo, int x, int y, int w, int h);
BOOL BBP_TouchCheckRectDown(u8 playerNo, int x, int y, int w, int h);
#endif

#endif
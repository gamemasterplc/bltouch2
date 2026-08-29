#include "touch.h"
#include "memory.h"
#include "wireless_defs.h"
#include "wireless.h"
#include "sys.h"

#define BL_TOUCH_HIS_SIZE 16

typedef struct TouchPlayer_s {
    BLTouchData his[BL_TOUCH_HIS_SIZE];
} TouchPlayer;

typedef struct TouchWork_s {
    TouchPlayer local;
    TouchPlayer remote[BL_WIRELESS_MAX_PLAYERS];
} TouchWork_t;

static TouchWork_t *TouchWork;

static void TouchSetData(TouchPlayer *player, TPData *tp);
static BOOL TouchGetDownData(BLTouchData *td, u8 *x, u8 *y);
static BOOL TouchGetHoldData(BLTouchData *td, u8 *x, u8 *y);

void BL_TouchInit(void)
{
    TPCalibrateParam calibrate;

    TouchWork = BL_MemCalloc(BL_MEM_TAG_SYSTEM, sizeof(TouchWork_t));
    if(!TouchWork) {
        return;
    }
    if(!TP_GetUserInfo(&calibrate))
		return;

	TP_SetCalibrateParam(&calibrate);
}

void BL_TouchExec(void)
{
    TPData *tpp;
    TPData tp;
    int i;
    
    tpp = &tp;
    if(!BL_TouchRead(&tp)) {
        tpp = NULL;
    }
    TouchSetData(&TouchWork->local, tpp);
    if(!BL_WLCheckInputSync()) {
        return;
    }
    for(i=0; i<BL_WIRELESS_MAX_PLAYERS; i++) {
        TouchSetData(&TouchWork->remote[i], BL_WLGetTPData(i));
    }
    
}

static void TouchSetData(TouchPlayer *player, TPData *tp)
{
    BLTouchData td;
    int i;
    if(!tp || (tp->touch && tp->validity != TP_VALIDITY_VALID)) {
        td.hold = player->his[0].hold;
        td.down = FALSE;
        td.x = player->his[0].x;
        td.y = player->his[0].y;
    } else if(!tp->touch) {
        td.down = td.hold = td.y = td.x = 0;
    } else {
        td.hold = TRUE;
        td.down = player->his[0].hold ^ 1;
        if(tp->validity & TP_VALIDITY_INVALID_X) {
            td.x = player->his[0].x;
        } else {
            td.x = tp->x;
        }
        if(tp->validity & TP_VALIDITY_INVALID_Y) {
            td.y = player->his[0].y;
        } else {
            td.y = tp->y;
        }
        
        
    }
    for(i=BL_TOUCH_HIS_SIZE-1; i>0; i--) {
        player->his[i] = player->his[i-1];
    }
    player->his[0] = td;
}

BOOL BL_TouchGetDown(u8 *x, u8 *y)
{
    return TouchGetDownData(&TouchWork->local.his[0], x, y);
}

BOOL BL_TouchGetDownPlayer(u8 playerNo, u8 *x, u8 *y)
{
    if(!BL_WLCheckInputSync()) {
        return BL_TouchGetDown(x, y);
    }
    if(playerNo >= BL_WIRELESS_MAX_PLAYERS) {
        return FALSE;
    }
    return TouchGetDownData(&TouchWork->remote[playerNo].his[0], x, y);
}

static BOOL TouchGetDownData(BLTouchData *td, u8 *x, u8 *y)
{
    if(td->down) {
        if(x) {
            *x = td->x;
        }
        if(y) {
            *y = td->y;
        }
        return TRUE;
    } 
    
    return FALSE;
}

BOOL BL_TouchGetHold(u8 *x, u8 *y)
{
    if(!TouchWork->local.his[0].hold) {
        return FALSE;
    }
    *x = TouchWork->local.his[0].x;
    *y = TouchWork->local.his[0].y;
    return TRUE;
}

BOOL BL_TouchGetHoldPlayer(u8 playerNo, u8 *x, u8 *y)
{
    if(!BL_WLCheckInputSync()) {
        return BL_TouchGetHold(x, y);
    }
    if(playerNo >= BL_WIRELESS_MAX_PLAYERS) {
        return FALSE;
    }
    return TouchGetHoldData(&TouchWork->remote[playerNo].his[0], x, y);
}

static BOOL TouchGetHoldData(BLTouchData *td, u8 *x, u8 *y)
{
    if(!td->hold) {
        return FALSE;
    }
    *x = td->x;
    *y = td->y;
    return TRUE;
}

#ifndef SYS_BBP
void BL_TouchGetHis(u8 index, BLTouchData *out)
{
    *out = TouchWork->local.his[index];
}

void BL_TouchGetHisPlayer(u8 playerNo, u8 index, BLTouchData *out)
{
    if(!BL_WLCheckInputSync()) {
        BL_TouchGetHis(index, out);
        return;
    }
    *out = TouchWork->remote[playerNo].his[index];
}

#else
BOOL BBP_TouchCheckCircleHold(u8 playerNo, int cx, int cy, int radius)
{
    u8 x;
    u8 y;
    int dx, dy, r2;
    if(!BL_TouchGetHoldPlayer(playerNo, &x, &y)) {
        return FALSE;
    }
    r2 = radius*radius;
    dx = x-cx;
    dy = y-cy;
    return ((dx*dx)+(dy*dy)) <= r2;
}

BOOL BBP_TouchCheckCircleDown(u8 playerNo, int cx, int cy, int radius)
{
    u8 x;
    u8 y;
    int dx, dy, r2;
    if(!BL_TouchGetDownPlayer(playerNo, &x, &y)) {
        return FALSE;
    }
    r2 = radius*radius;
    dx = x-cx;
    dy = y-cy;
    return ((dx*dx)+(dy*dy)) <= r2;
}

BOOL BBP_TouchCheckRectHold(u8 playerNo, int x, int y, int w, int h)
{
    u8 inX;
    u8 inY;
    if(!BL_TouchGetHoldPlayer(playerNo, &inX, &inY)) {
        return FALSE;
    }
    if(inX < x) {
        return FALSE;
    }
    if(inX >= (x+w)) {
        return FALSE;
    }
    if(inY < y) {
        return FALSE;
    }
    if(inY >= (y+h)) {
        return FALSE;
    }
    return TRUE;
}

BOOL BBP_TouchCheckRectDown(u8 playerNo, int x, int y, int w, int h)
{
    u8 inX;
    u8 inY;
    if(!BL_TouchGetDownPlayer(playerNo, &inX, &inY)) {
        return FALSE;
    }
    if(inX < x) {
        return FALSE;
    }
    if(inX >= (x+w)) {
        return FALSE;
    }
    if(inY < y) {
        return FALSE;
    }
    if(inY >= (y+h)) {
        return FALSE;
    }
    return TRUE;
}

#endif

u8 BL_TouchRead(TPData *tp)
{
    
    TPData temp;
    u8 invalid;
    temp.validity = TP_VALIDITY_INVALID_XY;
    temp.touch = FALSE;
    temp.x = 0;
    temp.y = 0;
    invalid = TRUE;
    if(!BL_SysCheckSleep() && TP_RequestCalibratedSampling(&temp)) {
        invalid = FALSE;
    }
    if(invalid) {
        MI_CpuCopy8(&temp, tp, sizeof(TPData));
    }
    return invalid;
}
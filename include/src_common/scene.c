#include "scene.h"
#include "sys_mode.h"

#include "vram.h"
#include "color.h"
#include "tone.h"
#include "sprite.h"
#include "gfxqueue.h"
#include "gpu_reg.h"
#include "object.h"
#include "sys.h"
#include "mic.h"
#include "hdma.h"
#include "memory.h"
#include "bl_types.h"
#include "wireless.h"
#include "res.h"
#include "res_color.h"
#include "res_anim.h"
#include "res_image.h"

#ifndef SYS_BBP
#include "bl/backup.h"
#else
#include "bbp/backup.h"
#endif

#ifdef SYS_BBP
#include "file.h"
#include "bbp/filetbl/linkicon.h"
#endif

#include <nitro.h>
#include <nnsys/gfx_transfer.h>

#define OVERLAY_ID_NULL -1

#ifndef SYS_BBP
#define SCENE_STATE_START 0
#define SCENE_STATE_END 1
#define SCENE_STATE_MAIN 2
#else
#define SCENE_STATE_START 0
#define SCENE_STATE_END 2
#define SCENE_STATE_MAIN 1
#endif

static struct {
    u16 posX; //0x00
    u16 posY; //0x02
    #ifdef SYS_BBP
    u16 useAlpha; //0x04
    #endif
} LinkIconWork;

static struct {
    BLScene scene; //0x00
    BLScene scenePrev; //0x04
    BLScene sceneNext; //0x08
    int sceneParam; //0x0C
    BLObj *obj; //0x10
    u8 state; //0x14
    BLBool exitBBP; //0x15
    BLBool sceneError; //0x16
    int errorParam; //0x18
    #ifdef SYS_BBP
    u32 unk1C[4];
    u32 overlayId; //0x2C
    #endif
} SceneWork;

void BL_SceneInit(void)
{
    MI_CpuFill8(&SceneWork, 0, sizeof(SceneWork));
    #ifndef SYS_BBP
    SceneWork.scene = SceneWork.sceneNext = BL_SceneFirst;
    if(OS_GetResetCode() == 1) {
        SceneWork.scene = SceneWork.sceneNext = BLSCENE_DEMO_TITLE;
    }
    if(Sys_IsExitBBP()) {
        SceneWork.scene = SceneWork.sceneNext = BLSCENE_MENU_PLAYMODE;
        SceneWork.sceneParam = 2;
    }
    if(BL_BackupIsLoadFail()) {
        BL_SceneCallError(0);
    } else if(BL_BackupIsCorrupt()) {
        BL_BackupForceErase();
        BL_SceneCallError(2);
    } else if(BL_BackupIsClean()) {
        BL_BackupSetClean();
        SceneWork.scene = SceneWork.sceneNext = BLSCENE_SAVE_WARNING;
    }
    #else
    SceneWork.scene = SceneWork.sceneNext = BLSCENE_MENU_MAIN;
    if(BL_BackupIsLoadFail()) {
        SceneWork.scene = SceneWork.sceneNext = BLSCENE_ERROR;
        SceneWork.sceneParam = 0;
        BL_SysResetMBBusy();
    } else if(BL_BackupIsCorrupt()) {
        SceneWork.scene = SceneWork.sceneNext = BLSCENE_ERROR;
        SceneWork.sceneParam = 2;
        BL_BackupForceErase();
    } else if(BL_BackupIsClean()) {
        BL_BackupSetClean();
        SceneWork.scene = SceneWork.sceneNext = BLSCENE_SAVE_WARNING;
    }
    SceneWork.overlayId = OVERLAY_ID_NULL;
    #endif
}

static void SceneSetNext(void);
static void LinkIconInit(void);
static void SceneFree(void);

#ifdef SYS_BBP
static inline u32 GetNextOverlay(void)
{
    if(SceneWork.scene == BLSCENE_BATTLE) {
        return BBP_SysGetBootParam()->ovlBattle;
    } else {
        return BBP_SysGetBootParam()->ovlMenu;
    }
}

static inline void LoadOverlay(u32 overlayId)
{
    if(SceneWork.overlayId != overlayId) {
        if(SceneWork.overlayId != OVERLAY_ID_NULL) {
            FS_UnloadOverlay(0, SceneWork.overlayId);
        }
        if(overlayId != OVERLAY_ID_NULL) {
            FS_LoadOverlay(0, overlayId);
        }
        SceneWork.overlayId = overlayId;
    }
}
#endif

void BL_SceneExec(void)
{
    u32 overlayId;
    switch(SceneWork.state) {
        case SCENE_STATE_START:
            BL_VramClear();
            BL_ColorInit();
            BL_ToneInit();
            BL_SprInit();
            BL_GfxQueueCreate();
            BL_GpuRegInit();
            BL_ObjResetPauseFlag(BLOBJ_PAUSEFLAG_ALL);
            BL_GfxQueueAdd((BLGfxQueueFunc)NNS_GfxTransferExec, BL_GFX_QUEUE_MAIN, 0xFFFE, NULL);
            SceneSetNext();
            #ifndef SYS_BBP
            if(!MB_IsMultiBootChild()) {
                if(BL_SceneTbl[SceneWork.scene].overlay != BL_OVERLAY_NONE) {
                    BL_OverlayLoad(BL_OVERLAY_TYPE_MODE, BL_SceneTbl[SceneWork.scene].overlay);
                }
            }
            #else
            LoadOverlay(GetNextOverlay());
            #endif
            LinkIconInit();
            SceneWork.obj = BL_ObjCreate(NULL, 0, 0, NULL, BL_SceneDestroy);
            BL_SceneCreateObj(BL_ToneExec, BLOBJ_PRIO_MAX, BLOBJ_PAUSEFLAG_NONE, NULL, NULL);
            BL_SysResetEnable();
            BL_SceneTbl[SceneWork.scene].func(SceneWork.sceneParam);
            BL_LinkIconCreate();
            SceneWork.state = SCENE_STATE_MAIN;
            break;
        
        case SCENE_STATE_MAIN:
            if(BL_BackupIsSaveFail()) {
                #ifdef SYS_BBP
                BBP_SysResetMBBusy();
                #endif
                BL_SceneCallError(1);
            }
            if(SceneWork.obj) {
                break;
            }
            if(!BL_MicAmpOff()) {
                break;
            }
            BL_SprClose();
            BL_ToneClose();
            BL_ColorClose();
            BL_FileClose();
            BL_GfxQueueClose();
            BL_VramKillMain();
            BL_VramKillSub();
            BL_HDmaInit();
            SceneFree();
            SceneWork.state = SCENE_STATE_START;
            #ifndef SYS_BBP
            if(BL_BackupIsDirty()) {
                BL_BackupWrite();
            }
            if(SceneWork.exitBBP) {
                SceneWork.exitBBP = BL_FALSE;
                Sys_SetModeBBP();
            }
            #endif
            break;
        
        default:
            SceneWork.state = SCENE_STATE_START;
            break;
    }
}

static void SceneFree(void)
{
    int i;
    for(i=BL_MEM_TAG_SCENE_BEGIN; i<BL_MEM_TAG_MAX; i++) {
        BL_MemFreeTag(i);
    }
}

BLObj *BL_SceneCreateObj(BLObjFunc func, u16 prio, u32 pauseFlag, void *data, BLObjFunc destroy)
{
    return BL_ObjCreateChild(SceneWork.obj, func, prio, pauseFlag, data, destroy);
}

void BL_SceneEnd(void)
{
    #ifdef SYS_BBP
    if(SceneWork.obj) {
        BL_ObjKill(SceneWork.obj);
    }
    #else
    BL_ObjKill(SceneWork.obj);
    #endif
    BL_SysResetDisable();
}

void BL_SceneCall(BLScene scene, int param)
{
    SceneWork.sceneNext = scene;
    SceneWork.sceneParam = param;
}

void BL_SceneCallError(int errorNo)
{
    SceneWork.sceneError = BL_TRUE;
    SceneWork.errorParam = errorNo;
    #ifndef SYS_BBP
    switch(errorNo) {
        case 0:
        case 1:
        case 4:
        case 5:
            BL_SysResetMBBusy();

        default:
            BL_SceneEnd();
    }
    #else
    switch(errorNo) {
        case 0:
        case 1:
        case 4:
        case 8:
            BL_SysResetMBBusy();

        default:
            BL_SceneEnd();
    }
    #endif
}

#ifndef SYS_BBP

BLScene BL_SceneGetCur(void)
{
    return SceneWork.scene;
}

void BL_SceneDestroy(void)
{
    SceneWork.obj = NULL;
}

int BL_SceneGetArg(void)
{
    return SceneWork.sceneParam;
}

BOOL BL_SceneIsRunning(void)
{
    return SceneWork.obj != NULL;
}

#else

BOOL BL_SceneIsRunning(void)
{
    return SceneWork.obj != NULL;
}

u32 BBP_SceneGetOverlayId(void)
{
    return SceneWork.overlayId;
}

void BL_SceneDestroy(void)
{
    SceneWork.obj = NULL;
}

#endif

static void SceneSetNext(void)
{
    #ifndef SYS_BBP
    if(SceneWork.sceneError) {
        BL_SceneCall(BLSCENE_ERROR, SceneWork.errorParam);
        SceneWork.sceneError = BL_FALSE;
    } else if(SceneWork.sceneNext == BLSCENE_NULL) {
        BL_SceneCall(BL_SceneFirst, 0);
    }
    SceneWork.scenePrev = SceneWork.scene;
    SceneWork.scene = SceneWork.sceneNext;
    SceneWork.sceneNext = BLSCENE_NULL;
    #else
    if(BBP_BackupIsNeedDWCWiFiId()) {
        BBP_BackupClearNeedDWCWiFiId();
        BL_SceneCallError(10);
    }
    if(SceneWork.sceneError) {
        BL_SceneCall(BLSCENE_ERROR, SceneWork.errorParam);
        SceneWork.sceneError = BL_FALSE;
    } else if(SceneWork.sceneNext == BLSCENE_NULL) {
        BL_SceneCall(BLSCENE_MENU_MAIN, 0);
    }
    SceneWork.scenePrev = SceneWork.scene;
    SceneWork.scene = SceneWork.sceneNext;
    SceneWork.sceneNext = BLSCENE_NULL;
    #endif
}

static void LinkIconInit(void)
{
    BL_GpuDispSelect = GX_DISP_SELECT_SUB_MAIN;
    REG_DISPCNT = 0x10000;
    #ifndef SYS_BBP
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_1, GX2D3D_MODE_3D);
    #else
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_1, GX2D3D_MODE_2D);
    #endif
    #ifndef SYS_BBP
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_64K);
    #else
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K);
    #endif
    GX_SetCharBase(GX_BGCHAROFFSET_0x00000);
    GX_SetScreenBase(GX_BGSCROFFSET_0x00000);
    REG_DISPCNT_SUB = 0x10000;
    GXS_SetGraphicsMode(GX_BGMODE_1);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_64K);
    BL_LinkIconSetPos(0, 176);
    #ifdef SYS_BBP
    BBP_LinkIconSetAlpha(FALSE);
    #endif
}

typedef struct {
    u16 state;
    BLSprite *spr[2];
    #ifndef SYS_BBP
    BLColorKey colorKey[2];
    #endif
    BLVramImageKey vramKey[2];
    #ifdef SYS_BBP
    BLFile *fileAnime;
    BLFile *fileImage;
    BLFile *fileCol;
    #endif
} LinkIconObj;

void BL_LinkIconCreate(void)
{
    LinkIconObj *work = BL_MemCalloc(BL_MEM_TAG_SCENE_GENERIC, sizeof(LinkIconObj));
    BL_SceneCreateObj(BL_LinkIconExec, BLOBJ_PRIO_MAX, BLOBJ_PAUSEFLAG_NONE, work, BL_LinkIconDestroy);
}

void BL_LinkIconExec(void)
{
    LinkIconObj *work = BL_ObjCur->data;
    u16 animNo;
    #ifdef SYS_BBP
    s32 linkLevel;
    #endif
    int i;
    int scrOther;
    int screen;
    BLResColor *resColor;
    BLResAnim *resAnim;
    BLResImage *resImage;
    switch(work->state) {
        case 0:
            if(GX_GetBankSizeForOBJ() == 0) {
                return;
            }
            if(GX_GetBankSizeForSubOBJ() == 0) {
                return;
            }
            
            if(!BL_WLIsDispLinkIcon()) {
                return;
            }
            #ifndef SYS_BBP
            extern u8 BL_LinkIconAnime[];
            extern u8 BL_LinkIconColor[];
            extern u8 BL_LinkIconImage[];
            scrOther = BL_GpuDispSelect ^ 1;
            for(i=0; i<2; i++) {
                screen = i^scrOther;
                BL_VramImageAlloc(&work->vramKey[i], screen, BL_ResGetAnimMaxVRamSize(BL_LinkIconAnime));
                resColor = (BLResColor *)BL_LinkIconColor;
                work->colorKey[i] = BL_ColorAlloc(resColor->data, NULL, screen, BL_COLOR_OBJ_PLTT, 0, 1, 0);
                work->spr[i] = BL_SprCreate(screen, 0, 0, BL_VramImageGetOfs(work->vramKey[i]), BL_ColorGetOfs(work->colorKey[i]), BL_LinkIconAnime, BL_LinkIconImage);
                BL_SprDispOff(work->spr[i]);
            }
            #else
            scrOther = BL_GpuDispSelect ^ 1;
            work->fileAnime = BL_FileLoad(linkicon_anime_DSAR, FALSE);
            work->fileImage = BL_FileLoad(linkicon_anime_DSIR, FALSE);
            work->fileCol = BL_FileLoad(linkicon_col_col_DSCR, FALSE);
            resAnim = BL_FILE_GET_DATA_AS(work->fileAnime, BLResAnim);
            resImage = BL_FILE_GET_DATA_AS(work->fileImage, BLResImage);
            resColor = BL_FILE_GET_DATA_AS(work->fileCol, BLResColor);
            for(i=0; i<2; i++) {
                screen = i^scrOther;
                BL_VramImageAlloc(&work->vramKey[i], screen, BL_ResGetAnimMaxVRamSize(resAnim));
                DC_FlushRange(resColor->data, 16*sizeof(u16));
                if(i != 0) {
                    GXS_LoadOBJPltt(resColor->data, 0x1E0, 16);
                } else {
                    GX_LoadOBJPltt(resColor->data, 0x1E0, 16);
                }
                work->spr[i] = BL_SprCreate(screen, 0, 0,
                    BL_VramImageGetOfs(work->vramKey[i]),
                    15,
                    resAnim,
                    resImage);
                BL_SprDispOff(work->spr[i]);
            }
            #endif
            work->state = 1;
            break;
        
        case 1:
            #ifdef SYS_BBP
            linkLevel = BL_WLGetLinkLevel();
            if(BL_WLGetCommState() == 11) {
                linkLevel += 4;
            }
            #endif
            if(LinkIconWork.posY < 192) {
                #ifndef SYS_BBP
                animNo = work->spr[0]->animSpr->base.animNo;
                if(animNo != BL_WLGetLinkLevel()) {
                    BL_SprSetAnim(work->spr[0], BL_WLGetLinkLevel(), 0);
                }
                #else
                animNo = work->spr[0]->animSpr->base.animNo;
                if(linkLevel != animNo) {
                    BL_SprSetAnim(work->spr[0], linkLevel, 0);
                }
                #endif
                BL_SprSetPos(work->spr[0], LinkIconWork.posX, LinkIconWork.posY);
                BL_SprDispOn(work->spr[0]);
                BL_SprDispOff(work->spr[1]);
            } else {
                #ifndef SYS_BBP
                animNo = work->spr[1]->animSpr->base.animNo;
                if(animNo != BL_WLGetLinkLevel()) {
                    BL_SprSetAnim(work->spr[1], BL_WLGetLinkLevel(), 0);
                }
                #else
                animNo = work->spr[1]->animSpr->base.animNo;
                if(linkLevel != animNo) {
                    BL_SprSetAnim(work->spr[1], linkLevel, 0);
                }
                #endif
                BL_SprSetPos(work->spr[1], LinkIconWork.posX, LinkIconWork.posY-192);
                BL_SprDispOn(work->spr[1]);
                BL_SprDispOff(work->spr[0]);
            }
            #ifdef SYS_BBP
            if(LinkIconWork.useAlpha) {
                BL_SprAlphaOn(work->spr[0]);
                BL_SprAlphaOn(work->spr[1]);
            } else {
                BL_SprAlphaOff(work->spr[0]);
                BL_SprAlphaOff(work->spr[1]);
            }
            #endif
            if(!BL_WLIsDispLinkIcon()) {
                BL_SprDispOff(work->spr[0]);
                BL_SprDispOff(work->spr[1]);
                
            }
            break;
    }
}

void BL_LinkIconDestroy(void)
{
    LinkIconObj *work = BL_ObjCur->data;
    BL_SprKill(work->spr[0]);
    BL_SprKill(work->spr[1]);
}

void BL_LinkIconSetPos(int posX, int posY)
{
    if(posX >= 0 && posX <= 240) {
        LinkIconWork.posX = posX;
    }
    if((posY >= 0 && posY <= 176) || (posY >= 192 && posY <= 368)) {
        LinkIconWork.posY = posY;
    }
}

#ifdef SYS_BBP
void BBP_LinkIconSetAlpha(u16 flag)
{
    LinkIconWork.useAlpha = flag;
}
#endif
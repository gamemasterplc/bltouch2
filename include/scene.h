#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include <nitro.h>


typedef void (*BLSceneFunc)(int param);

#ifdef SYS_BBP

#define BLSCENE(func, id) id,
typedef enum BLScene_e {
    #include "bbp/scenetbl.inc"
    BLSCENE_NULL
} BLScene;
#undef BLSCENE

typedef struct BLSceneEntry_s {
    BLSceneFunc func;
} BLSceneEntry;

#else

#include "bl/overlay.h"

#define BLSCENE(func, modeOvl, id) id,
typedef enum BLScene_e {
    #include "bl/scenetbl.inc"
    BLSCENE_NULL
} BLScene;
#undef BLSCENE

typedef struct BLSceneEntry_s {
    BLSceneFunc func;
    int overlay;
} BLSceneEntry;
#endif

#ifdef SYS_BBP
#define BL_SceneInit BBP_SceneInit
#define BL_SceneExec BBP_SceneExec
#define BL_SceneCallError BBP_SceneCallError
#define BL_SceneDestroy BBP_SceneDestroy
#define BL_SceneCreateObj BBP_SceneCreateObj
#define BL_LinkIconCreate BBP_LinkIconCreate
#define BL_LinkIconSetPos BBP_LinkIconSetPos
#define BL_LinkIconExec BBP_LinkIconExec
#define BL_LinkIconDestroy BBP_LinkIconDestroy
#define BL_SceneEnd BBP_SceneEnd
#define BL_SceneCall BBP_SceneCall
#define BL_SceneTbl BBP_SceneTbl
#define BL_SceneIsRunning BBP_SceneIsRunning
#endif

void BL_SceneInit(void);
void BL_SceneExec(void);
void BL_SceneCallError(int errorNo);
void BL_SceneDestroy(void);
BLObj *BL_SceneCreateObj(BLObjFunc func, u16 prio, u32 pauseFlag, void *data, BLObjFunc destroy);
void BL_SceneEnd(void);
void BL_SceneCall(BLScene scene, int param);
BOOL BL_SceneIsRunning(void);


#ifndef SYS_BBP
BLScene BL_SceneGetCur(void);
int BL_SceneGetArg(void);
#else
u32 BBP_SceneGetOverlayId(void);
#endif

void BL_LinkIconCreate(void);
void BL_LinkIconSetPos(int posX, int posY);
void BL_LinkIconExec(void);
void BL_LinkIconDestroy(void);

#ifdef SYS_BBP
void BBP_LinkIconSetAlpha(u16 flag);
#endif

extern BLSceneEntry BL_SceneTbl[];


#ifndef SYS_BBP
extern BLScene BL_SceneFirst;
#endif

#endif
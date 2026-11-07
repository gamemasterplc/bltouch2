#include "scene.h"

#define BLSCENE(func, modeOvl, id) extern void func(int param);
#include "bl/scenetbl.inc"
#undef BLSCENE

#define BLSCENE(func, modeOvl, id) { func, modeOvl },

BLSceneEntry BL_SceneTbl[] = {
    #include "bl/scenetbl.inc"
};

#undef BLSCENE

BLScene BL_SceneFirst = BLSCENE_MENU_LICENSED;
#include "scene.h"

#define BLSCENE(func, id) extern void func(int param);
#include "bbp/scenetbl.inc"
#undef BLSCENE

#define BLSCENE(func, id) { func },

BLSceneEntry BL_SceneTbl[] = {
    #include "bbp/scenetbl.inc"
};

#undef BLSCENE


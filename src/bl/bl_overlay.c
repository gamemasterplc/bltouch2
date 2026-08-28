#include <nitro.h>

#include "bl/overlay.h"
#include "sys.h"

#define OVERLAY_ID_NULL -1

EXTERN_OVERLAY_ID(1); //mg
EXTERN_OVERLAY_ID(2); //field
EXTERN_OVERLAY_ID(3); //mg01
EXTERN_OVERLAY_ID(4); //mg02
EXTERN_OVERLAY_ID(5); //mg03
EXTERN_OVERLAY_ID(6); //mg04
EXTERN_OVERLAY_ID(7); //mg05
EXTERN_OVERLAY_ID(8); //mg06
EXTERN_OVERLAY_ID(9); //mg07
EXTERN_OVERLAY_ID(10); //mg08
EXTERN_OVERLAY_ID(11); //mg09
EXTERN_OVERLAY_ID(12); //mg10
EXTERN_OVERLAY_ID(13); //mg11
EXTERN_OVERLAY_ID(14); //mg12
EXTERN_OVERLAY_ID(15); //mg13
EXTERN_OVERLAY_ID(16); //mg14
EXTERN_OVERLAY_ID(17); //mg15
EXTERN_OVERLAY_ID(18); //mg16
EXTERN_OVERLAY_ID(19); //mg17
EXTERN_OVERLAY_ID(20); //mg18
EXTERN_OVERLAY_ID(21); //mg19
EXTERN_OVERLAY_ID(22); //mg20
EXTERN_OVERLAY_ID(23); //mg21
EXTERN_OVERLAY_ID(24); //mg22
EXTERN_OVERLAY_ID(25); //mg23
EXTERN_OVERLAY_ID(26); //mg24
EXTERN_OVERLAY_ID(27); //mg25
EXTERN_OVERLAY_ID(28); //mg26
EXTERN_OVERLAY_ID(29); //mg27
EXTERN_OVERLAY_ID(30); //mg28
EXTERN_OVERLAY_ID(31); //mg29
EXTERN_OVERLAY_ID(32); //mg30
EXTERN_OVERLAY_ID(33); //mg31
EXTERN_OVERLAY_ID(34); //mg32
EXTERN_OVERLAY_ID(35); //mg33
EXTERN_OVERLAY_ID(36); //mg34
EXTERN_OVERLAY_ID(37); //mg35
EXTERN_OVERLAY_ID(38); //mg36
EXTERN_OVERLAY_ID(39); //mg37
EXTERN_OVERLAY_ID(40); //mg38
EXTERN_OVERLAY_ID(41); //mg39
EXTERN_OVERLAY_ID(42); //mg40
EXTERN_OVERLAY_ID(43); //field_star
EXTERN_OVERLAY_ID(44); //field_moon
EXTERN_OVERLAY_ID(45); //field_sun
EXTERN_OVERLAY_ID(46); //field_earth
EXTERN_OVERLAY_ID(47); //mes_english

static int LoadedOverlay[BL_OVERLAY_TYPE_MAX];

static u32 ModeOverlay[BL_OVERLAY_MODE_MAX] = {
    OVERLAY_ID(1),
    OVERLAY_ID(2),
};

static u32 SceneOverlay[BL_OVERLAY_SCENE_MAX] = {
    OVERLAY_ID(3),
    OVERLAY_ID(4),
    OVERLAY_ID(5),
    OVERLAY_ID(6),
    OVERLAY_ID(7),
    OVERLAY_ID(8),
    OVERLAY_ID(9),
    OVERLAY_ID(10),
    OVERLAY_ID(11),
    OVERLAY_ID(12),
    OVERLAY_ID(13),
    OVERLAY_ID(14),
    OVERLAY_ID(15),
    OVERLAY_ID(16),
    OVERLAY_ID(17),
    OVERLAY_ID(18),
    OVERLAY_ID(19),
    OVERLAY_ID(20),
    OVERLAY_ID(21),
    OVERLAY_ID(22),
    OVERLAY_ID(23),
    OVERLAY_ID(24),
    OVERLAY_ID(25),
    OVERLAY_ID(26),
    OVERLAY_ID(27),
    OVERLAY_ID(28),
    OVERLAY_ID(29),
    OVERLAY_ID(30),
    OVERLAY_ID(31),
    OVERLAY_ID(32),
    OVERLAY_ID(33),
    OVERLAY_ID(34),
    OVERLAY_ID(35),
    OVERLAY_ID(36),
    OVERLAY_ID(37),
    OVERLAY_ID(38),
    OVERLAY_ID(39),
    OVERLAY_ID(40),
    OVERLAY_ID(41),
    OVERLAY_ID(42),
    OVERLAY_ID(13),
    OVERLAY_ID(14),
    OVERLAY_ID(34),
    OVERLAY_ID(16),
    OVERLAY_ID(31),
    OVERLAY_ID(43),
    OVERLAY_ID(44),
    OVERLAY_ID(45),
    OVERLAY_ID(46)
};

static u32 MessOverlay[5] = {
    OVERLAY_ID(47),
    OVERLAY_ID_NULL,
    OVERLAY_ID_NULL,
    OVERLAY_ID_NULL,
    OVERLAY_ID_NULL
};

static u32 *OverlayTable[BL_OVERLAY_TYPE_MAX] = {
    ModeOverlay,
    SceneOverlay,
    MessOverlay
};

void BL_OverlayInit(void)
{
    int i;
    int language;
    for(i=0; i<3; i++) {
        LoadedOverlay[i] = BL_OVERLAY_NONE;
    }
    language = BL_SysGetLanguage();
    if(language != BL_SYS_LANGUAGE_NONE) {
        BL_OverlayLoad(BL_OVERLAY_TYPE_MESS, language);
    }
}

void BL_OverlayLoad(BLOverlayType type, int index)
{
    int old = LoadedOverlay[type];
    if(index == old) {
        return;
    } else {
        u32 *table = OverlayTable[type];
        if(!table) {
            return;
        }
        if(old >= 0 && table[old] != OVERLAY_ID_NULL) {
            FS_UnloadOverlay(0, table[old]);
        }
        if(table[index] != OVERLAY_ID_NULL) {
            FS_LoadOverlay(0, table[index]);
        }
        LoadedOverlay[type] = index;
    }
}

static inline void LoadSceneOverlay(int no)
{
    if(no == BL_OVERLAY_NONE) {
        return;
    }
    BL_OverlayLoad(BL_OVERLAY_TYPE_SCENE, no);
}

void BL_OverlayMgLoad(int no)
{
    if(MB_IsMultiBootChild()) {
        return;
    }
    LoadSceneOverlay(no);
}

void BL_OverlayFieldLoad(int no)
{
    no += BL_OVERLAY_SCENE_FIELD_STAR;
    LoadSceneOverlay(no);
}

void BL_OverlayClose(void)
{
    int i;
    for(i=0; i<BL_OVERLAY_TYPE_MAX; i++) {
        int curOverlay = LoadedOverlay[i];
        if(curOverlay >= 0) {
            u32 *table = OverlayTable[i];
            if(table && table[curOverlay] != OVERLAY_ID_NULL) {
                FS_UnloadOverlay(0, table[curOverlay]);
            }
        }
    }
}
#include "map.h"
#include "bl/overlay.h"
#include "random.h"
#include "bl/backup.h"

MapChrPath Map_ChrPath[MAP_CHR_MAX]; //0x020AD058


void Map_SaveInit(void)
{
    int i;

    MI_CpuFill8(&Map_Backup, 0, sizeof(MapBackup));
    MapBackup *backup = &Map_Backup;
    backup->area = MAP_AREA_STAR_A2P1;
    BL_OverlayMapLoad(Map_GetCurZoneTypeBackup());
    for(i=0; i<MAP_CHR_MAX; i++) {
        backup->chr[i].id = i;
        if(i == MAP_CHR_SHIRO) {
            backup->chr[i].moveType = 0;
        } else {
            backup->chr[i].moveType = 1;
        }
        backup->chr[i].enable = (i == MAP_CHR_SHIRO);
        backup->chr[i].dir = 0;
        if(MapSave_GetFlagBackup(MAPFLAG(MAP_FLAG_ZONE_ENTER, 9))) {
            backup->chr[i].pos.x = FX32_CONVERT(832);
            backup->chr[i].pos.y = FX32_CONVERT(224);
        } else if(backup->area == MAP_AREA_STAR_A1P2) {
            backup->chr[i].pos.x = FX32_CONVERT(384);
            backup->chr[i].pos.y = FX32_CONVERT(384);
        } else if(backup->area == MAP_AREA_STAR_A1P3) {
            if(MapSave_GetFlagBackup(MAPFLAG(MAP_FLAG_ZONE_ENTER, 2))) {
                backup->chr[i].pos.x = FX32_CONVERT(32);
                backup->chr[i].pos.y = FX32_CONVERT(416);
            } else {
                backup->chr[i].pos.x = FX32_CONVERT(32);
                backup->chr[i].pos.y = FX32_CONVERT(352);
            }
        } else if(backup->area == MAP_AREA_STAR_A2P1) {
            backup->chr[i].pos.x = FX32_CONVERT(384);
            backup->chr[i].pos.y = FX32_CONVERT(176);
        } else if(backup->area == MAP_AREA_MOON_A1P1) {
            backup->chr[MAP_CHR_SHIRO].pos.x = FX32_CONVERT(496);
            backup->chr[MAP_CHR_SHIRO].pos.y = FX32_CONVERT(296);
        } else if(backup->area == MAP_AREA_MOON_A1P2) {
            backup->chr[MAP_CHR_SHIRO].pos.x = FX32_CONVERT(192);
            backup->chr[MAP_CHR_SHIRO].pos.y = FX32_CONVERT(112);
        } else if(backup->area == MAP_AREA_MOON_A1P4) {
            backup->chr[i].pos.x = FX32_CONVERT(464);
            backup->chr[i].pos.y = FX32_CONVERT(360);
        } else if(backup->area == MAP_AREA_MOON_A2P1) {
            backup->chr[MAP_CHR_SHIRO].pos.x = FX32_CONVERT(448);
            backup->chr[MAP_CHR_SHIRO].pos.y = FX32_CONVERT(432);
        } else if(backup->area == MAP_AREA_SUN_A1P2) {
            backup->chr[i].pos.x = FX32_CONVERT(224);
            backup->chr[i].pos.y = FX32_CONVERT(320);
        } else if(backup->area == MAP_AREA_SUN_A1P3) {
            backup->chr[i].pos.x = FX32_CONVERT(32);
            backup->chr[i].pos.y = FX32_CONVERT(96);
        } else if(backup->area == MAP_AREA_SUN_A1P4) {
            backup->chr[i].pos.x = FX32_CONVERT(2848);
            backup->chr[i].pos.y = FX32_CONVERT(96);
        } else if(backup->area == MAP_AREA_EARTH_A1P1) {
            backup->chr[i].pos.x = FX32_CONVERT(64);
            backup->chr[i].pos.y = FX32_CONVERT(432);
        } else if(backup->area == MAP_AREA_EARTH_A2P3) {
            backup->chr[i].pos.x = FX32_CONVERT(288);
            backup->chr[i].pos.y = FX32_CONVERT(264);
        } else if(backup->area == MAP_AREA_EARTH_A2P5) {
            backup->chr[i].pos.x = -FX32_CONVERT(32);
            backup->chr[i].pos.y = FX32_CONVERT(96);
        } else {
            backup->chr[i].pos.x = FX32_CONVERT(128);
            backup->chr[i].pos.y = FX32_CONVERT(96);
        }
        backup->chr[i].speed = 0;
    }
    backup->playerChr = MAP_CHR_SHIRO;
    backup->playerDir = 1;
    backup->playerPos.x = 384;
    backup->playerPos.y = 176;
    
    Map_SaveFlagInit();
}

void Map_SaveFlagInit(void)
{
    u8 chance;
    MapSave_SetMenuPageBackup(2);
    MapSave_SetMessSpeedBackup(1);
    MapSave_SetMenuShortcutBackup(1);
    MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_CHR_EVIL, 6));
    MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A1P2, 9));
    MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_MOON_A2P2, 2));
    MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1, 4));
    MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A2P2, 9));
    MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A2P2, 12));
    MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_EARTH_A2P3, 0));
    MapSave_SetUranaiBackup(MAP_URANAI_STAR, MAP_URANAI_AREA_NULL, 0);
    MapSave_SetUranaiBackup(MAP_URANAI_MOON, MAP_URANAI_AREA_NULL, 0);
    MapSave_SetUranaiBackup(MAP_URANAI_SUN, MAP_URANAI_AREA_NULL, 0);
    chance = BL_RandomInt(100);
    if(chance < 33) {
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 5));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 7));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 9));
    } else if(chance < 66) {
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 6));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 7));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 8));
    } else {
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 5));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 8));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_STAR_A2P4, 9));
    }
    chance = BL_RandomInt(100);
    if(chance < 33) {
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 0));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 2));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 4));
    } else if(chance < 66) {
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 1));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 2));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 3));
    } else {
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 0));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 3));
        MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_SUN_A1P1_ALT, 4));
    }
}

void Map_SaveBackup(void)
{
    int i;
    MI_CpuFill8(&Map_Backup, 0, sizeof(MapBackup));
    MapBackup *backup = &Map_Backup;
    backup->area = Map_Work.area;
    backup->prevArea = Map_Work.area;
    backup->playerChr = Map_Work.playerChr;
    for(i=0; i<MAP_CHR_MAX; i++) {
        backup->chr[i].id = Map_Work.chr[i].id;
        backup->chr[i].moveType = Map_Work.chr[i].moveType;
        backup->chr[i].enable = Map_Work.chr[i].enable;
        backup->chr[i].dir = Map_Work.chr[i].dir;
        backup->chr[i].pos = Map_Work.chr[i].pos;
        backup->chr[i].speed = 0;
    }
    for(i=0; i<MAP_MAIL_MAX; i++) {
        backup->mail[i] = Map_Work.mail[i];
    }
    backup->escortAvail = Map_Work.escortAvail;
    backup->escortChr = Map_Work.escortChr;
    backup->save = Map_Work.save;
    backup->unk30C = Map_Work.unk166C;
    backup->unk310 = Map_Work.unk1670;
    backup->atrcWinNum = Map_Work.atrcWinNum;
}

void Map_BackupPlayerPos(void)
{
    Map_Work.playerDir = Map_Work.chr[MAP_CHR_SHIRO].dir;
    Map_Work.playerPos.x = FX_Whole(Map_Work.chr[MAP_CHR_SHIRO].pos.x);
    Map_Work.playerPos.y = FX_Whole(Map_Work.chr[MAP_CHR_SHIRO].pos.y);
}

void Map_MovePlayerMgBonus(int mgNo)
{
    switch(mgNo) {
        case 14:
            Map_Work.playerDir = 3;
            Map_Work.playerPos.x = 200;
            Map_Work.playerPos.y = 196;
            break;
        
        case 35:
            Map_Work.playerDir = 5;
            Map_Work.playerPos.x = 312;
            Map_Work.playerPos.y = 228;
            break;
        
        case 25:
            Map_Work.playerDir = 3;
            Map_Work.playerPos.x = 256;
            Map_Work.playerPos.y = 256;
            break;
        
        case 20:
            Map_Work.playerDir = 3;
            Map_Work.playerPos.x = 352;
            Map_Work.playerPos.y = 208;
            break;
        
        case 40:
            Map_Work.playerDir = 3;
            Map_Work.playerPos.x = 192;
            Map_Work.playerPos.y = 176;
            break;
        
        case 41:
            Map_Work.playerDir = 3;
            Map_Work.playerPos.x = 256;
            Map_Work.playerPos.y = 144;
            break;
        
        case 42:
            Map_Work.playerDir = 5;
            Map_Work.playerPos.x = 256;
            Map_Work.playerPos.y = 144;
            break;
        
        case 43:
            Map_Work.playerDir = 5;
            Map_Work.playerPos.x = 320;
            Map_Work.playerPos.y = 176;
            break;
        
        case 44:
            Map_Work.playerDir = 5;
            Map_Work.playerPos.x = 384;
            Map_Work.playerPos.y = 208;
            break;
    }
}

static void LoadSavePos(void);

void Map_LoadBackup(void)
{
    int i;
    MapBackup *backup = &Map_Backup;
    Map_Work.area = backup->area;
    Map_Work.prevArea = backup->prevArea;
    Map_Work.playerChr = backup->playerChr;
    LoadSavePos();
    for(i=0; i<MAP_CHR_MAX; i++) {
        Map_Work.chr[i].id = backup->chr[i].id;
        Map_Work.chr[i].moveType = backup->chr[i].moveType;
        Map_Work.chr[i].enable = backup->chr[i].enable;
        Map_Work.chr[i].dir = backup->chr[i].dir;
        Map_Work.chr[i].pos = backup->chr[i].pos;
        Map_Work.chr[i].speed = backup->chr[i].speed;
        Map_Work.chrPath[i] = Map_ChrPath[i];
    }
    for(i=0; i<MAP_MAIL_MAX; i++) {
        Map_Work.mail[i] = backup->mail[i];
    }
    Map_Work.escortAvail = backup->escortAvail;
    Map_Work.escortChr = backup->escortChr;
    Map_Work.save = backup->save;
    Map_Work.unk166C = backup->unk30C;
    Map_Work.unk1670 = backup->unk310;
    Map_Work.atrcWinNum = backup->atrcWinNum;
}

static void LoadSavePos(void)
{
    MapBackup *backup = &Map_Backup;
    Map_Work.prevArea2 = backup->prevArea;
    Map_Work.playerDir = backup->playerDir = backup->chr[MAP_CHR_SHIRO].dir;
    Map_Work.playerPos.x = backup->playerPos.x = FX_Whole(backup->chr[MAP_CHR_SHIRO].pos.x);
    Map_Work.playerPos.y = backup->playerPos.y = FX_Whole(backup->chr[MAP_CHR_SHIRO].pos.y);
}

void Map_WriteBackup(int slot)
{
    int i;
    MI_CpuFill8(&Map_BackupCopy, 0, sizeof(MapBackup));
    MapBackup *backup = &Map_BackupCopy;
    backup->area = Map_Work.area;
    backup->prevArea = Map_Work.prevArea2;
    backup->playerChr = Map_Work.playerChr;
    for(i=0; i<MAP_CHR_MAX; i++) {
        backup->chr[i].id = Map_Work.chr[i].id;
        backup->chr[i].moveType = Map_Work.chr[i].moveType;
        backup->chr[i].enable = Map_Work.chr[i].enable;
        backup->chr[i].dir = Map_Work.chr[i].dir;
        backup->chr[i].pos = Map_Work.chr[i].pos;
        backup->chr[i].speed = 0;
    }
    for(i=0; i<MAP_MAIL_MAX; i++) {
        backup->mail[i] = Map_Work.mail[i];
    }
    backup->escortAvail = Map_Work.escortAvail;
    backup->escortChr = Map_Work.escortChr;
    backup->save = Map_Work.save;
    backup->unk30C = Map_Work.unk166C;
    backup->unk310 = Map_Work.unk1670;
    backup->atrcWinNum = Map_Work.atrcWinNum;
    backup->chr[MAP_CHR_SHIRO].dir = Map_Work.playerDir;
    backup->chr[MAP_CHR_SHIRO].pos.x = Map_Work.playerPos.x << 12;
    backup->chr[MAP_CHR_SHIRO].pos.y = Map_Work.playerPos.y << 12;
    BL_BackupLandWrite(slot, backup);
    BL_BackupWrite();
}

void Map_ReadBackup(int slot)
{
    MI_CpuFill8(&Map_Backup, 0, sizeof(MapBackup));
    BL_BackupLandRead(slot, &Map_Backup);
}

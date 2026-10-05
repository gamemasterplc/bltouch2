#include "map.h"
#include "bl/overlay.h"
#include "random.h"
#include "bl/backup.h"

MapBackup Map_Backup; //0x020AD15C
MapWork Map_Work; //0x020AD480

int Map_GetZoneType(u8 area)
{
    switch(area) {
        case MAP_AREA_STAR_A1P1:
        case MAP_AREA_STAR_A1P2:
        case MAP_AREA_STAR_A1P3:
        case MAP_AREA_STAR_A1P4:
        case MAP_AREA_STAR_A2P1:
        case MAP_AREA_STAR_A2P2:
        case MAP_AREA_STAR_A2P3:
        case MAP_AREA_STAR_A2P4:
        case MAP_AREA_STAR_A2P5:
            return MAP_ZONE_TYPE_STAR;
            
        case MAP_AREA_MOON_A1P1:
        case MAP_AREA_MOON_A1P2:
        case MAP_AREA_MOON_A1P3:
        case MAP_AREA_MOON_A1P4:
        case MAP_AREA_MOON_A1P5:
        case MAP_AREA_MOON_A2P1:
        case MAP_AREA_MOON_A2P2:
        case MAP_AREA_MOON_A2P3:
            return MAP_ZONE_TYPE_MOON;
        
        case MAP_AREA_SUN_A1P1:
        case MAP_AREA_SUN_A1P2:
        case MAP_AREA_SUN_A1P3:
        case MAP_AREA_SUN_A1P4:
        case MAP_AREA_SUN_A2P1:
        case MAP_AREA_SUN_A2P2:
        case MAP_AREA_SUN_A2P3:
        case MAP_AREA_SUN_A2P4:
            return MAP_ZONE_TYPE_SUN;
        
        case MAP_AREA_EARTH_A1P1:
        case MAP_AREA_EARTH_A1P2:
        case MAP_AREA_EARTH_A1P3:
        case MAP_AREA_EARTH_A1P4:
        case MAP_AREA_EARTH_A2P1:
        case MAP_AREA_EARTH_A2P2:
        case MAP_AREA_EARTH_A2P3:
        case MAP_AREA_EARTH_A2P4:
        case MAP_AREA_EARTH_A2P5:
            return MAP_ZONE_TYPE_EARTH;
        
        default:
            return MAP_ZONE_TYPE_STAR;
    }
}

int Map_GetCurZoneType_(int saveType)
{
    int area;
    if(saveType == MAP_SAVE_WORK) {
        area = Map_Work.area;
    } else {
        area = Map_Backup.area;
    }
    return Map_GetZoneType(area);
}

int Map_GetPrevZoneType_(int saveType)
{
    int area;
    if(saveType == MAP_SAVE_WORK) {
        area = Map_Work.prevArea;
    } else {
        area = Map_Backup.prevArea;
    }
    return Map_GetZoneType(area);
}

u8 Map_GetSubWakuType_(int saveType)
{
    int area;
    if(saveType == MAP_SAVE_WORK) {
        area = Map_Work.area;
    } else {
        area = Map_Backup.area;
    }
    switch(area) {
        case MAP_AREA_STAR_A1P1:
        case MAP_AREA_STAR_A1P2:
        case MAP_AREA_STAR_A1P3:
        case MAP_AREA_STAR_A1P4:
        case MAP_AREA_STAR_A2P1:
        case MAP_AREA_STAR_A2P2:
        case MAP_AREA_STAR_A2P3:
        case MAP_AREA_STAR_A2P4:
        case MAP_AREA_STAR_A2P5:
            return area-MAP_AREA_STAR_A1P1;
            
        case MAP_AREA_MOON_A1P1:
        case MAP_AREA_MOON_A1P2:
        case MAP_AREA_MOON_A1P3:
        case MAP_AREA_MOON_A1P4:
        case MAP_AREA_MOON_A1P5:
        case MAP_AREA_MOON_A2P1:
        case MAP_AREA_MOON_A2P2:
        case MAP_AREA_MOON_A2P3:
            return area-MAP_AREA_MOON_A1P1;
        
        case MAP_AREA_SUN_A1P1:
        case MAP_AREA_SUN_A1P2:
        case MAP_AREA_SUN_A1P3:
        case MAP_AREA_SUN_A1P4:
        case MAP_AREA_SUN_A2P1:
        case MAP_AREA_SUN_A2P2:
        case MAP_AREA_SUN_A2P3:
        case MAP_AREA_SUN_A2P4:
            return area-MAP_AREA_SUN_A1P1;
        
        case MAP_AREA_EARTH_A1P1:
        case MAP_AREA_EARTH_A1P2:
        case MAP_AREA_EARTH_A1P3:
        case MAP_AREA_EARTH_A1P4:
        case MAP_AREA_EARTH_A2P1:
        case MAP_AREA_EARTH_A2P2:
        case MAP_AREA_EARTH_A2P3:
        case MAP_AREA_EARTH_A2P4:
        case MAP_AREA_EARTH_A2P5:
            return area-MAP_AREA_EARTH_A1P1;
        
        default:
            return 0;
    }
}

u8 Map_GetZoneArea_(int saveType)
{
    int area;
    if(saveType == MAP_SAVE_WORK) {
        area = Map_Work.area;
    } else {
        area = Map_Backup.area;
    }
    switch(area) {
        case MAP_AREA_STAR_A1P1:
        case MAP_AREA_STAR_A1P2:
        case MAP_AREA_STAR_A1P3:
        case MAP_AREA_STAR_A1P4:
            return area-MAP_AREA_STAR_A1P1;
            
        case MAP_AREA_STAR_A2P1:
        case MAP_AREA_STAR_A2P2:
        case MAP_AREA_STAR_A2P3:
        case MAP_AREA_STAR_A2P4:
        case MAP_AREA_STAR_A2P5:
            return area-MAP_AREA_STAR_A2P1;
            
        case MAP_AREA_MOON_A1P1:
        case MAP_AREA_MOON_A1P2:
        case MAP_AREA_MOON_A1P3:
        case MAP_AREA_MOON_A1P4:
        case MAP_AREA_MOON_A1P5:
            return area-MAP_AREA_MOON_A1P1;
            
        case MAP_AREA_MOON_A2P1:
        case MAP_AREA_MOON_A2P2:
        case MAP_AREA_MOON_A2P3:
            return area-MAP_AREA_MOON_A2P1;
        
        case MAP_AREA_SUN_A1P1:
        case MAP_AREA_SUN_A1P2:
        case MAP_AREA_SUN_A1P3:
        case MAP_AREA_SUN_A1P4:
            return area-MAP_AREA_SUN_A1P1;
            
        case MAP_AREA_SUN_A2P1:
        case MAP_AREA_SUN_A2P2:
        case MAP_AREA_SUN_A2P3:
        case MAP_AREA_SUN_A2P4:
            return area-MAP_AREA_SUN_A2P1;
        
        case MAP_AREA_EARTH_A1P1:
        case MAP_AREA_EARTH_A1P2:
        case MAP_AREA_EARTH_A1P3:
        case MAP_AREA_EARTH_A1P4:
            return area-MAP_AREA_EARTH_A1P1;
            
        case MAP_AREA_EARTH_A2P1:
        case MAP_AREA_EARTH_A2P2:
        case MAP_AREA_EARTH_A2P3:
        case MAP_AREA_EARTH_A2P4:
        case MAP_AREA_EARTH_A2P5:
            return area-MAP_AREA_EARTH_A2P1;
        
        default:
            return 0;
    }
}

u8 Map_GetCurZone_(int saveType)
{
    int area;
    if(saveType == MAP_SAVE_WORK) {
        area = Map_Work.area;
    } else {
        area = Map_Backup.area;
    }
    switch(area) {
        case MAP_AREA_STAR_A1P1:
        case MAP_AREA_STAR_A1P2:
        case MAP_AREA_STAR_A1P3:
        case MAP_AREA_STAR_A1P4:
            return MAP_ZONE_STAR_OUT;
            
        case MAP_AREA_STAR_A2P1:
        case MAP_AREA_STAR_A2P2:
        case MAP_AREA_STAR_A2P3:
        case MAP_AREA_STAR_A2P4:
        case MAP_AREA_STAR_A2P5:
            return MAP_ZONE_STAR_IN;
            
        case MAP_AREA_MOON_A1P1:
        case MAP_AREA_MOON_A1P2:
        case MAP_AREA_MOON_A1P3:
        case MAP_AREA_MOON_A1P4:
        case MAP_AREA_MOON_A1P5:
            return MAP_ZONE_MOON_OUT;
            
        case MAP_AREA_MOON_A2P1:
        case MAP_AREA_MOON_A2P2:
        case MAP_AREA_MOON_A2P3:
            return MAP_ZONE_MOON_IN;
        
        case MAP_AREA_SUN_A1P1:
        case MAP_AREA_SUN_A1P2:
        case MAP_AREA_SUN_A1P3:
        case MAP_AREA_SUN_A1P4:
            return MAP_ZONE_SUN_OUT;
            
        case MAP_AREA_SUN_A2P1:
        case MAP_AREA_SUN_A2P2:
        case MAP_AREA_SUN_A2P3:
        case MAP_AREA_SUN_A2P4:
            return MAP_ZONE_SUN_IN;
        
        case MAP_AREA_EARTH_A1P1:
        case MAP_AREA_EARTH_A1P2:
        case MAP_AREA_EARTH_A1P3:
        case MAP_AREA_EARTH_A1P4:
            return MAP_ZONE_EARTH_OUT;
            
        case MAP_AREA_EARTH_A2P1:
        case MAP_AREA_EARTH_A2P2:
        case MAP_AREA_EARTH_A2P3:
        case MAP_AREA_EARTH_A2P4:
        case MAP_AREA_EARTH_A2P5:
            return MAP_ZONE_EARTH_IN;
        
        default:
            return MAP_ZONE_STAR_OUT;
    }
}

u8 Map_GetZone(int area)
{
    switch(area) {
        case MAP_AREA_STAR_A1P1:
        case MAP_AREA_STAR_A1P2:
        case MAP_AREA_STAR_A1P3:
        case MAP_AREA_STAR_A1P4:
            return MAP_ZONE_STAR_OUT;
            
        case MAP_AREA_STAR_A2P1:
        case MAP_AREA_STAR_A2P2:
        case MAP_AREA_STAR_A2P3:
        case MAP_AREA_STAR_A2P4:
        case MAP_AREA_STAR_A2P5:
            return MAP_ZONE_STAR_IN;
            
        case MAP_AREA_MOON_A1P1:
        case MAP_AREA_MOON_A1P2:
        case MAP_AREA_MOON_A1P3:
        case MAP_AREA_MOON_A1P4:
        case MAP_AREA_MOON_A1P5:
            return MAP_ZONE_MOON_OUT;
            
        case MAP_AREA_MOON_A2P1:
        case MAP_AREA_MOON_A2P2:
        case MAP_AREA_MOON_A2P3:
            return MAP_ZONE_MOON_IN;
        
        case MAP_AREA_SUN_A1P1:
        case MAP_AREA_SUN_A1P2:
        case MAP_AREA_SUN_A1P3:
        case MAP_AREA_SUN_A1P4:
            return MAP_ZONE_SUN_OUT;
            
        case MAP_AREA_SUN_A2P1:
        case MAP_AREA_SUN_A2P2:
        case MAP_AREA_SUN_A2P3:
        case MAP_AREA_SUN_A2P4:
            return MAP_ZONE_SUN_IN;
        
        case MAP_AREA_EARTH_A1P1:
        case MAP_AREA_EARTH_A1P2:
        case MAP_AREA_EARTH_A1P3:
        case MAP_AREA_EARTH_A1P4:
            return MAP_ZONE_EARTH_OUT;
            
        case MAP_AREA_EARTH_A2P1:
        case MAP_AREA_EARTH_A2P2:
        case MAP_AREA_EARTH_A2P3:
        case MAP_AREA_EARTH_A2P4:
        case MAP_AREA_EARTH_A2P5:
            return MAP_ZONE_EARTH_IN;
        
        default:
            return MAP_ZONE_STAR_OUT;
    }
}

BOOL Map_GetChrPosInt(int chrNo, s16 *outX, s16 *outY)
{
    if(!Map_Work.chr[chrNo].enable) {
        return FALSE;
    }
    *outX = FX_Whole(Map_Work.chr[chrNo].pos.x);
    *outY = FX_Whole(Map_Work.chr[chrNo].pos.y);
    return TRUE;
}

void Map_AddAtrcWinNum(int num)
{
    int val = Map_Backup.atrcWinNum+num;
    if(val < 0) {
        Map_Backup.atrcWinNum = 0;
    } else if(val >= 9999) {
        Map_Backup.atrcWinNum = 9999;
    } else {
        Map_Backup.atrcWinNum = val;
    }
    
}

int Map_GetAtrcWinNum(void)
{
    return Map_Backup.atrcWinNum;
}

BLBool Map_IsPieceFull(int flagType)
{
    switch(flagType) {
        case MAP_FLAG_PIECE_STAR:
            return MapSave_GetPieceNum(MAP_FLAG_PIECE_STAR) >= 20;
        
        case MAP_FLAG_PIECE_MOON:
            return MapSave_GetPieceNum(MAP_FLAG_PIECE_MOON) >= 20;
        
        case MAP_FLAG_PIECE_SUN:
            return MapSave_GetPieceNum(MAP_FLAG_PIECE_SUN) >= 20;
        
        case MAP_FLAG_PIECE_EARTH:
            return MapSave_GetPieceNum(MAP_FLAG_PIECE_EARTH) >= 20;
        
        case MAP_FLAG_PIECE_LIGHTNING:
            return MapSave_GetPieceNum(MAP_FLAG_PIECE_LIGHTNING) >= 10;

        case MAP_FLAG_PIECE_RAINBOW:
            return MapSave_GetPieceNum(MAP_FLAG_PIECE_RAINBOW) >= 5;
        
        case MAP_FLAG_PIECE_HEART:
            return MapSave_GetPieceNum(MAP_FLAG_PIECE_HEART) >= 5;
        
        default:
            return BL_FALSE;
    }
}

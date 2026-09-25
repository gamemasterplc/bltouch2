#include "map.h"

u8 MapSave_ItemHeadBitTbl[6] = {
    MAP_ITEM_BIT(MAP_ITEM_HEAD_STAR),
    MAP_ITEM_BIT(MAP_ITEM_HEAD_EVIL),
    MAP_ITEM_BIT(MAP_ITEM_HEAD_FLUTE),
    MAP_ITEM_BIT(MAP_ITEM_HEAD_PUMPKIN),
    MAP_ITEM_BIT(MAP_ITEM_HEAD_COWBOY),
    MAP_ITEM_BIT(MAP_ITEM_HEAD_SAGE),
};

u8 MapSave_ItemHeadOrderTbl[6] = {
    MAP_ITEM_HEAD_STAR,
    MAP_ITEM_HEAD_EVIL,
    MAP_ITEM_HEAD_FLUTE,
    MAP_ITEM_HEAD_PUMPKIN,
    MAP_ITEM_HEAD_COWBOY,
    MAP_ITEM_HEAD_SAGE,
};

u8 MapSave_ItemBodyOrderTbl[8] = {
    MAP_ITEM_BODY_STAR_CAPE,
    MAP_ITEM_BODY_EVIL_CAPE,
	MAP_ITEM_BODY_FISHING_ROD,
	MAP_ITEM_BODY_SWIM_RING,
	MAP_ITEM_BODY_BROOM,
	MAP_ITEM_BODY_SHURIKEN,
	MAP_ITEM_BODY_BROOM_WATER,
	MAP_ITEM_BODY_BROOM_FIRE,
};

u8 MapSave_ItemTicketBitTbl[8] = {
    MAP_ITEM_BIT(MAP_ITEM_TICKET_TRUCK),
    MAP_ITEM_BIT(MAP_ITEM_TICKET_SUNJET),
    MAP_ITEM_BIT(MAP_ITEM_TICKET_SOCCER),
    MAP_ITEM_BIT(MAP_ITEM_TICKET_BASKETBALL),
    MAP_ITEM_BIT(MAP_ITEM_TICKET_FISHING),
    MAP_ITEM_BIT(MAP_ITEM_TICKET_DANCE),
    MAP_ITEM_BIT(MAP_ITEM_TICKET_CYCLE),
    MAP_ITEM_BIT(MAP_ITEM_TICKET_SUMO),
};

u8 MapSave_ItemBombBitTbl[8] = {
    MAP_ITEM_BIT(MAP_ITEM_BOMB_NORMAL),
	MAP_ITEM_BIT(MAP_ITEM_BOMB_SOCCER),
	MAP_ITEM_BIT(MAP_ITEM_BOMB_BASKETBALL),
	MAP_ITEM_BIT(MAP_ITEM_BOMB_POT),
	MAP_ITEM_BIT(MAP_ITEM_BOMB_MINI), 
	MAP_ITEM_BIT(MAP_ITEM_BOMB_MAGNET),
	MAP_ITEM_BIT(MAP_ITEM_BOMB_WATER),
	MAP_ITEM_BIT(MAP_ITEM_BOMB_FIRE),
};

u8 MapSave_ItemBodyBitTbl[8] = {
    MAP_ITEM_BIT(MAP_ITEM_BODY_STAR_CAPE),
    MAP_ITEM_BIT(MAP_ITEM_BODY_EVIL_CAPE),
	MAP_ITEM_BIT(MAP_ITEM_BODY_FISHING_ROD),
	MAP_ITEM_BIT(MAP_ITEM_BODY_SWIM_RING),
	MAP_ITEM_BIT(MAP_ITEM_BODY_BROOM),
	MAP_ITEM_BIT(MAP_ITEM_BODY_SHURIKEN),
	MAP_ITEM_BIT(MAP_ITEM_BODY_BROOM_WATER),
	MAP_ITEM_BIT(MAP_ITEM_BODY_BROOM_FIRE),
};

u8 MapSave_ItemBombOrderTbl[8] = {
    MAP_ITEM_BOMB_NORMAL,
	MAP_ITEM_BOMB_SOCCER,
	MAP_ITEM_BOMB_BASKETBALL,
	MAP_ITEM_BOMB_POT,
	MAP_ITEM_BOMB_MINI, 
	MAP_ITEM_BOMB_MAGNET,
	MAP_ITEM_BOMB_WATER,
	MAP_ITEM_BOMB_FIRE,
};


static inline MapSave *GetMapSave(int saveType)
{
    if(saveType == MAP_SAVE_WORK) {
        return &Map_Work.save;
    } else {
        return &Map_Backup.save;
    }
}

void MapSave_SetNewGame(void)
{
    if(MapSave_GetFlagBackup(MAPFLAG(MAP_FLAG_CHR_PRETTY, 0)) || Map_GetCurZoneType(TRUE) == MAP_ZONE_TYPE_STAR) {
        return;
    }
    MapSave_SetFlagBackup(MAPFLAG(MAP_FLAG_CHR_PRETTY, 0));
}

#define AREA_TABLE \
    AREAENTRY(MAP_FLAG_STAR_A1P1, MAP_AREA_STAR_A1P1) \
    AREAENTRY(MAP_FLAG_STAR_A1P2, MAP_AREA_STAR_A1P2) \
    AREAENTRY(MAP_FLAG_STAR_A1P3, MAP_AREA_STAR_A1P3) \
    AREAENTRY(MAP_FLAG_STAR_A1P4, MAP_AREA_STAR_A1P4) \
    AREAENTRY(MAP_FLAG_STAR_A2P1, MAP_AREA_STAR_A2P1) \
    AREAENTRY(MAP_FLAG_STAR_A2P2, MAP_AREA_STAR_A2P2) \
    AREAENTRY(MAP_FLAG_STAR_A2P3, MAP_AREA_STAR_A2P3) \
    AREAENTRY(MAP_FLAG_STAR_A2P4, MAP_AREA_STAR_A2P4) \
    AREAENTRY(MAP_FLAG_STAR_A2P5, MAP_AREA_STAR_A2P5) \
    AREAENTRY(MAP_FLAG_MOON_A1P1, MAP_AREA_MOON_A1P1) \
    AREAENTRY(MAP_FLAG_MOON_A1P2, MAP_AREA_MOON_A1P2) \
    AREAENTRY(MAP_FLAG_MOON_A1P3, MAP_AREA_MOON_A1P3) \
    AREAENTRY(MAP_FLAG_MOON_A1P4, MAP_AREA_MOON_A1P4) \
    AREAENTRY(MAP_FLAG_MOON_A1P5, MAP_AREA_MOON_A1P5) \
    AREAENTRY(MAP_FLAG_MOON_A2P1, MAP_AREA_MOON_A2P1) \
    AREAENTRY(MAP_FLAG_MOON_A2P2, MAP_AREA_MOON_A2P2) \
    AREAENTRY(MAP_FLAG_MOON_A2P3, MAP_AREA_MOON_A2P3) \
    AREAENTRY(MAP_FLAG_SUN_A1P1, MAP_AREA_SUN_A1P1) \
    AREAENTRY(MAP_FLAG_SUN_A1P1_ALT, MAP_AREA_SUN_A1P1) \
    AREAENTRY(MAP_FLAG_SUN_A1P2, MAP_AREA_SUN_A1P2) \
    AREAENTRY(MAP_FLAG_SUN_A1P3, MAP_AREA_SUN_A1P3) \
    AREAENTRY(MAP_FLAG_SUN_A1P4, MAP_AREA_SUN_A1P4) \
    AREAENTRY(MAP_FLAG_SUN_A2P1, MAP_AREA_SUN_A2P1) \
    AREAENTRY(MAP_FLAG_SUN_A2P2, MAP_AREA_SUN_A2P2) \
    AREAENTRY(MAP_FLAG_SUN_A2P3, MAP_AREA_SUN_A2P3) \
    AREAENTRY(MAP_FLAG_SUN_A2P4, MAP_AREA_SUN_A2P4) \
    AREAENTRY(MAP_FLAG_EARTH_A1P1, MAP_AREA_EARTH_A1P1) \
    AREAENTRY(MAP_FLAG_EARTH_A1P2, MAP_AREA_EARTH_A1P2) \
    AREAENTRY(MAP_FLAG_EARTH_A1P3, MAP_AREA_EARTH_A1P3) \
    AREAENTRY(MAP_FLAG_EARTH_A1P4, MAP_AREA_EARTH_A1P4) \
    AREAENTRY(MAP_FLAG_EARTH_A2P1, MAP_AREA_EARTH_A2P1) \
    AREAENTRY(MAP_FLAG_EARTH_A2P2, MAP_AREA_EARTH_A2P2) \
    AREAENTRY(MAP_FLAG_EARTH_A2P3, MAP_AREA_EARTH_A2P3) \
    AREAENTRY(MAP_FLAG_EARTH_A2P4, MAP_AREA_EARTH_A2P4) \
    AREAENTRY(MAP_FLAG_EARTH_A2P5, MAP_AREA_EARTH_A2P5)

u8 MapSave_GetFlagArea(int flag)
{
    int type = flag >> 8;
    switch(type) {
        case MAP_FLAG_PLAYER:
            return Map_Work.area;
        
        #define AREAENTRY(flagType, area) \
        case flagType: \
            return area; \
            break;
        AREA_TABLE
        #undef AREAENTRY
        
        default:
            return 0;
    }
}

#define FLAG_TABLE \
    FLAGENTRY(MAP_FLAG_PLAYER, playerFlag) \
    FLAGENTRY(MAP_FLAG_PAD, padFlag) \
    FLAGENTRY(MAP_FLAG_MAIL_KURO, mailKuro) \
    FLAGENTRY(MAP_FLAG_MAIL_PRETTY, mailPretty) \
    FLAGENTRY(MAP_FLAG_MAIL_GOLD, mailGold) \
    FLAGENTRY(MAP_FLAG_MAIL_AO, mailAo) \
    FLAGENTRY(MAP_FLAG_MAIL_MIDORI, mailMidori) \
    FLAGENTRY(MAP_FLAG_MAIL_AQUA, mailAqua) \
    FLAGENTRY(MAP_FLAG_MAIL_STAR, mailStar) \
    FLAGENTRY(MAP_FLAG_MAIL_THIEF, mailThief) \
    FLAGENTRY(MAP_FLAG_MAIL_ZAKO, mailZako) \
    FLAGENTRY(MAP_FLAG_STAMP, stampFlag) \
    FLAGENTRY(MAP_FLAG_STAR_A1P1, starA1P1Flag) \
    FLAGENTRY(MAP_FLAG_STAR_A1P2, starA1P2Flag) \
    FLAGENTRY(MAP_FLAG_STAR_A1P3, starA1P3Flag) \
    FLAGENTRY(MAP_FLAG_STAR_A1P4, starA1P4Flag) \
    FLAGENTRY(MAP_FLAG_STAR_A2P1, starA2P1Flag) \
    FLAGENTRY(MAP_FLAG_STAR_A2P2, starA2P2Flag) \
    FLAGENTRY(MAP_FLAG_STAR_A2P3, starA2P3Flag) \
    FLAGENTRY(MAP_FLAG_STAR_A2P4, starA2P4Flag) \
    FLAGENTRY(MAP_FLAG_STAR_A2P5, starA2P5Flag) \
    FLAGENTRY(MAP_FLAG_MOON_A1P1, moonA1P1Flag) \
    FLAGENTRY(MAP_FLAG_MOON_A1P2, moonA1P2Flag) \
    FLAGENTRY(MAP_FLAG_MOON_A1P3, moonA1P3Flag) \
    FLAGENTRY(MAP_FLAG_MOON_A1P4, moonA1P4Flag) \
    FLAGENTRY(MAP_FLAG_MOON_A1P5, moonA1P5Flag) \
    FLAGENTRY(MAP_FLAG_MOON_A2P1, moonA2P1Flag) \
    FLAGENTRY(MAP_FLAG_MOON_A2P2, moonA2P2Flag) \
    FLAGENTRY(MAP_FLAG_MOON_A2P3, moonA2P3Flag) \
    FLAGENTRY(MAP_FLAG_SUN_A1P1, sunA1P1Flag) \
    FLAGENTRY(MAP_FLAG_SUN_A1P1_ALT, sunA1P1AltFlag) \
    FLAGENTRY(MAP_FLAG_SUN_A1P2, sunA1P2Flag) \
    FLAGENTRY(MAP_FLAG_SUN_A1P3, sunA1P3Flag) \
    FLAGENTRY(MAP_FLAG_SUN_A1P4, sunA1P4Flag) \
    FLAGENTRY(MAP_FLAG_SUN_A2P1, sunA2P1Flag) \
    FLAGENTRY(MAP_FLAG_SUN_A2P2, sunA2P2Flag) \
    FLAGENTRY(MAP_FLAG_SUN_A2P3, sunA2P3Flag) \
    FLAGENTRY(MAP_FLAG_SUN_A2P4, sunA2P4Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A1P1, earthA1P1Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A1P2, earthA1P2Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A1P3, earthA1P3Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A1P4, earthA1P4Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A2P1, earthA2P1Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A2P2, earthA2P2Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A2P3, earthA2P3Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A2P4, earthA2P4Flag) \
    FLAGENTRY(MAP_FLAG_EARTH_A2P5, earthA2P5Flag) \
    FLAGENTRY(MAP_FLAG_CHR_SHIRO, chrShiroFlag) \
    FLAGENTRY(MAP_FLAG_CHR_KURO, chrKuroFlag) \
    FLAGENTRY(MAP_FLAG_CHR_PRETTY, chrPrettyFlag) \
    FLAGENTRY(MAP_FLAG_CHR_EVIL, chrEvilFlag) \
    FLAGENTRY(MAP_FLAG_CHR_AO, chrAoFlag) \
    FLAGENTRY(MAP_FLAG_CHR_MIDORI, chrMidoriFlag) \
    FLAGENTRY(MAP_FLAG_CHR_GOLD, chrGoldFlag) \
    FLAGENTRY(MAP_FLAG_MINI, miniFlag) \
    FLAGENTRY(MAP_FLAG_MAGIC, magicFlag) \
    FLAGENTRY(MAP_FLAG_BOSS_JACK, bossJackFlag) \
    FLAGENTRY(MAP_FLAG_BOSS_QUEEN, bossQueenFlag) \
    FLAGENTRY(MAP_FLAG_BOSS_ACE, bossAceFlag) \
    FLAGENTRY(MAP_FLAG_BOSS_EVIL, bossEvilFlag) \
    FLAGENTRY(MAP_FLAG_THIEF, thiefFlag) \
    FLAGENTRY(MAP_FLAG_NINJA, ninjaFlag) \
    FLAGENTRY(MAP_FLAG_SAGE, sageFlag) \
    FLAGENTRY(MAP_FLAG_MUMMY, mummyFlag) \
    FLAGENTRY(MAP_FLAG_URANAI, uranaiFlag) \
    FLAGENTRY(MAP_FLAG_MG_HYPNOTIZE, mgHypnotizeFlag) \
    FLAGENTRY(MAP_FLAG_STAR, starFlag) \
    FLAGENTRY(MAP_FLAG_MOON, moonFlag) \
    FLAGENTRY(MAP_FLAG_SUN, sunFlag) \
    FLAGENTRY(MAP_FLAG_EARTH, earthFlag) \
    FLAGENTRY(MAP_FLAG_AREA_VISIT, areaVisitFlag) \
    FLAGENTRY(MAP_FLAG_NEXT_STAMP, nextStampFlag) \
    FLAGENTRY(MAP_FLAG_ZONE_EVIL, zoneEvilFlag) \
    FLAGENTRY(MAP_FLAG_ZONE_ENTER, zoneEnterFlag) \
    FLAGENTRY(MAP_FLAG_ZONE_EXIT, zoneExitFlag) \
    FLAGENTRY(MAP_FLAG_EVENT, eventFlag) \
    FLAGENTRY(MAP_FLAG_EVENT_TOUCH, eventTouchFlag) \
    FLAGENTRY(MAP_FLAG_5A, unkFE) \
    FLAGENTRY(MAP_FLAG_BONUS, bonusFlag) \
    FLAGENTRY(MAP_FLAG_5C, unk102) \
    FLAGENTRY(MAP_FLAG_BONUS2, bonusFlag2) \
    FLAGENTRY(MAP_FLAG_5E, unk106)

#define FLAG_PIECE_TABLE \
    FLAGENTRY(MAP_FLAG_PIECE_STAR, starPieceFlag) \
    FLAGENTRY(MAP_FLAG_PIECE_MOON, moonPieceFlag) \
    FLAGENTRY(MAP_FLAG_PIECE_SUN, sunPieceFlag) \
    FLAGENTRY(MAP_FLAG_PIECE_EARTH, earthPieceFlag) \
    FLAGENTRY(MAP_FLAG_PIECE_LIGHTNING, lightningPieceFlag) \
    FLAGENTRY(MAP_FLAG_PIECE_RAINBOW, rainbowPieceFlag) \
    FLAGENTRY(MAP_FLAG_PIECE_HEART, heartPieceFlag) \
    FLAGENTRY(MAP_FLAG_PIECE_KEY, keyPieceFlag) \

void MapSave_SetFlag_(int flag, int saveType)
{
    int temp = flag & 0xFF00;
    u8 bit = flag & 0xFF;
    u8 type = temp >> 8;
    MapSave *save = GetMapSave(saveType);
    switch(type) {
        #define FLAGENTRY(flagType, field) \
        case flagType: \
            save->field |= (1 << bit); \
            break;
        FLAG_TABLE
        #undef FLAGENTRY
    }
}

void MapSave_ResetFlag_(int flag, int saveType)
{
    int temp = flag & 0xFF00;
    u8 bit = flag & 0xFF;
    u8 type = temp >> 8;
    MapSave *save = GetMapSave(saveType);
    switch(type) {
        #define FLAGENTRY(flagType, field) \
        case flagType: \
            save->field &= ~(1 << bit); \
            break;
        FLAG_TABLE
        #undef FLAGENTRY
    }
}
    
BLBool MapSave_GetFlag_(int flag, int saveType)
{
    int temp = flag & 0xFF00;
    u8 bit = flag & 0xFF;
    u8 type = temp >> 8;
    MapSave *save = GetMapSave(saveType);
    switch(type) {
        #define FLAGENTRY(flagType, field) \
        case flagType: \
            return (save->field & (1 << bit)) ? BL_TRUE : BL_FALSE; \
            break;
        FLAG_TABLE
        #undef FLAGENTRY
        default:
            return BL_FALSE;
    }
}

void MapSave_SetPieceFlag_(int type, int piece, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    switch(type) {
        #define FLAGENTRY(flagType, field) \
        case flagType: \
            save->field |= (1 << piece); \
            break;
        FLAG_PIECE_TABLE
        #undef FLAGENTRY
    }
}

u32 MapSave_GetPieceFlag_(int type, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    switch(type) {
        #define FLAGENTRY(flagType, field) \
        case flagType: \
            return save->field; \
            break;
        FLAG_PIECE_TABLE
        #undef FLAGENTRY
        default:
            return 0;
    }
}

int MapSave_GetPieceNum_(int type, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    int i, num;
    u32 flag;
    switch(type) {
        #define FLAGENTRY(flagType, field) \
        case flagType: \
            flag = save->field; \
            break;
        FLAG_PIECE_TABLE
        #undef FLAGENTRY
    }
    
    switch(type) {
        case MAP_FLAG_PIECE_STAR:
        case MAP_FLAG_PIECE_MOON:
        case MAP_FLAG_PIECE_SUN:
        case MAP_FLAG_PIECE_EARTH:
            for(num=i=0; i<20; i++) {
                if(flag & (1 << i)) {
                    num++;
                }
            }
            return num;
        
        case MAP_FLAG_PIECE_LIGHTNING:
            for(num=i=0; i<10; i++) {
                if(flag & (1 << i)) {
                    num++;
                }
            }
            return num;
        
        case MAP_FLAG_PIECE_RAINBOW:
            for(num=i=0; i<5; i++) {
                if(flag & (1 << i)) {
                    num++;
                }
            }
            return num;
        
        case MAP_FLAG_PIECE_HEART:
            for(num=i=0; i<10; i++) {
                if(flag & (1 << i)) {
                    num++;
                }
            }
            return num;
        
        case MAP_FLAG_PIECE_KEY:
            for(num=i=0; i<4; i++) {
                if(flag & (1 << i)) {
                    num++;
                }
            }
            return num;
    }
}

void MapSave_CollectToken_(int type, int token, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    save->numTokens++;
    if(save->numTokens > 99) {
        save->numTokens = 99;
    }
    MapSave_SetFlag(MAPFLAG(MAP_FLAG_BONUS2, 14));
}

void MapSave_AddToken_(s16 num, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    save->numTokens += num;
    int tokens = save->numTokens;
    if(tokens > 99) {
        save->numTokens = 99;
    }
    if(tokens < 0) {
        save->numTokens = 0;
    }
    MapSave_SetFlag(MAPFLAG(MAP_FLAG_BONUS2, 14));
}

s16 MapSave_GetNumTokens_(int saveType)
{
    MapSave *save = GetMapSave(saveType);
    return save->numTokens;
}

int MapSave_GetNumStamps_(int saveType)
{
    MapSave *save = GetMapSave(saveType);
    int i, num;
    for(num=i=0; i<10; i++) {
        if(save->stampFlag & (1 << i)) {
            num++;
        }
    }
    return num;
}

void MapSave_SetAtrcFlag_(int mgNo, int type, BLBool on, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(type == MAP_ATRC_FLAG_COMPLETE) {
        switch(on) {
            case BL_FALSE:
                save->atrcFlag[mgNo].complete = BL_FALSE;
                break;
            
            case BL_TRUE:
                save->atrcFlag[mgNo].complete = BL_TRUE;
                break;
        }
        return;
    } else if (type == MAP_ATRC_FLAG_DEFEAT) {
        save->atrcFlag[mgNo].defeat = BL_TRUE;
    } else if (type == MAP_ATRC_FLAG_COMPLETE_TOKEN) {
        if(on) {
            save->atrcFlag[mgNo].completeToken = BL_TRUE;
        } else {
            save->atrcFlag[mgNo].completeToken = BL_FALSE;
        }
    } else {
        if(on) {
            save->atrcFlag[mgNo].unk1 = BL_TRUE;
        } else {
            save->atrcFlag[mgNo].unk1 = BL_FALSE;
        }
    }
}

BLBool MapSave_GetAtrcFlag_(int mgNo, int type, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(type == MAP_ATRC_FLAG_COMPLETE) {
        return save->atrcFlag[mgNo].complete;
    } else if (type == MAP_ATRC_FLAG_DEFEAT) {
        return save->atrcFlag[mgNo].defeat;
    } else if (type == MAP_ATRC_FLAG_COMPLETE_TOKEN) {
        return save->atrcFlag[mgNo].completeToken;
    } else {
        return save->atrcFlag[mgNo].unk1;
    }
}

u8 MapSave_GetNumAtrcFlag_(int type, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    int i, num;
    for(i=num=0; i<45; i++) {
        if(type == MAP_ATRC_FLAG_COMPLETE) {
            if(save->atrcFlag[i].complete) {
                num++;
            }
        } else if (type == MAP_ATRC_FLAG_DEFEAT) {
            if(save->atrcFlag[i].defeat) {
                num++;
            }
        } else if (type == MAP_ATRC_FLAG_COMPLETE_TOKEN) {
            if(save->atrcFlag[i].completeToken) {
                num++;
            }
        } else {
            if(save->atrcFlag[i].unk1) {
                num++;
            }
        }
    }
    return num;
}

void MapSave_AddAtrcPlayNum_(int atrcNo, u8 num, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    s8 oldNum = save->atrcFlag[atrcNo].playNum;
    s8 newNum = oldNum+num;
    if(newNum > 15) {
        newNum = 15;
    }
    if(newNum < 0) {
        newNum = 0;
    }
    save->atrcFlag[atrcNo].playNum = newNum;
    
}

u8 MapSave_GetAtrcPlayNum_(int atrcNo, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    return save->atrcFlag[atrcNo].playNum;
}

MapSaveAtrcRecord *MapSave_GetAtrcRecord_(int saveType)
{
    if(saveType == MAP_SAVE_WORK) {
        return &Map_Work.save.atrcRecord;
    } else {
        return &Map_Backup.save.atrcRecord;
    }
}

void MapSave_SetBHouseFlag_(int bhouseNo, int type, BLBool on, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(type == MAP_BHOUSE_FLAG_COMPLETE) {
        switch(on) {
            case BL_FALSE:
                save->bhouseFlag[bhouseNo].complete = BL_FALSE;
                break;
            
            case BL_TRUE:
                save->bhouseFlag[bhouseNo].complete = BL_TRUE;
                break;
        }
        return;
    } else if (type == MAP_BHOUSE_FLAG_DEFEAT) {
        save->bhouseFlag[bhouseNo].defeat = BL_TRUE;
    } else {
        if(on) {
            save->bhouseFlag[bhouseNo].unk1 = BL_TRUE;
        } else {
            save->bhouseFlag[bhouseNo].unk1 = BL_FALSE;
        }
    }
}

BLBool MapSave_GetBHouseFlag_(int bhouseNo, int type, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(type == MAP_BHOUSE_FLAG_COMPLETE) {
        return save->bhouseFlag[bhouseNo].complete;
    } else if (type == MAP_BHOUSE_FLAG_DEFEAT) {
        return save->bhouseFlag[bhouseNo].defeat;
    } else {
        return save->bhouseFlag[bhouseNo].unk1;
    }
}

void MapSave_AddBHousePlayNum_(int bhouseNo, u8 num, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    s8 oldNum = save->bhouseFlag[bhouseNo].playNum;
    s8 newNum = oldNum+num;
    if(newNum > 15) {
        newNum = 15;
    }
    if(newNum < 0) {
        newNum = 0;
    }
    save->bhouseFlag[bhouseNo].playNum = newNum;
}

u8 MapSave_GetBHousePlayNum_(int bhouseNo, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    return save->bhouseFlag[bhouseNo].playNum;
}

void MapSave_SetItemFlag_(int type, int bit, BLBool reset, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(type == MAP_ITEM_HEAD) {
        if(!reset) {
            save->itemHeadFlag |= (bit & 0xFF);
        } else {
            save->itemHeadFlag &= ~(bit & 0xFF);
        }
    } else if(type == MAP_ITEM_BODY) {
        if(!reset) {
            save->itemBodyFlag |= (bit & 0xFF);
        } else {
            save->itemBodyFlag &= ~(bit & 0xFF);
        }
    } else if(type == MAP_ITEM_BOMB) {
        if(!reset) {
            save->itemBombFlag |= (bit & 0xFF);
        } else {
            save->itemBombFlag &= ~(bit & 0xFF);
        }
    } else {
        if(!reset) {
            save->itemTicketFlag |= (bit & 0xFF);
        } else {
            save->itemTicketFlag &= ~(bit & 0xFF);
        }
    }
}

BLBool MapSave_GetItemFlag_(int type, int bit, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(type == MAP_ITEM_HEAD) {
        if(save->itemHeadFlag & bit) {
            return BL_TRUE;
        }
    } else if(type == MAP_ITEM_BODY) {
        if(save->itemBodyFlag & bit) {
            return BL_TRUE;
        }
    } else if(type == MAP_ITEM_BOMB) {
        if(save->itemBombFlag & bit) {
            return BL_TRUE;
        }
    } else {
        if(save->itemTicketFlag & bit) {
            return BL_TRUE;
        }
    }
    return BL_FALSE;
}

u8 MapSave_GetNumItem_(int type, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    int i;
    s8 num = 0;
    
    if(type == MAP_ITEM_HEAD) {
        for(i=0; i<6; i++) {
            if(save->itemHeadFlag & MapSave_ItemHeadBitTbl[i]) {
                num++;
            }
        }
    } else if(type == MAP_ITEM_BODY) {
        for(i=0; i<8; i++) {
            if(save->itemBodyFlag & MapSave_ItemBodyBitTbl[i]) {
                num++;
            }
        }
    } else if(type == MAP_ITEM_BOMB) {
        for(i=0; i<8; i++) {
            if(save->itemBombFlag & MapSave_ItemBombBitTbl[i]) {
                num++;
            }
        }
    } else {
        for(i=0; i<9; i++) {
            if(save->itemTicketFlag & MapSave_ItemTicketBitTbl[i]) {
                num++;
            }
        }
    }
    return num;
}

void MapSave_SetCurItem_(int type, u8 no, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(type == MAP_ITEM_HEAD) {
        save->itemHeadCur = no & 0xFF;
    } else if(type == MAP_ITEM_BODY) {
        save->itemBodyCur = no & 0xFF;
    } else if(type == MAP_ITEM_BOMB) {
        save->itemBombCur = no & 0x7F;
    } else {
        save->itemTicketCur = no & 0xFF;
    }
}

u8 MapSave_GetCurItem_(int type, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(type == MAP_ITEM_HEAD) {
        return save->itemHeadCur;
    } else if(type == MAP_ITEM_BODY) {
        return save->itemBodyCur;
    } else if(type == MAP_ITEM_BOMB) {
        return save->itemBombCur;
    } else {
        return save->itemTicketCur;
    }
}

void MapSave_SetSunRideJetFlag_(u16 flag, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    u16 word = flag/16;
    save->sunRideFlag[word] |= 1 << (flag-(word*16));
}

BLBool MapSave_GetSunRideJetFlag_(u16 flag, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    u16 word = flag/16;
    return (save->sunRideFlag[word] & 1 << (flag-(word*16))) ? BL_TRUE : BL_FALSE;
}

u16 MapSave_GetNumSunRideStars_(int saveType)
{
    u16 j;
    u16 i;
    u16 num;
    num = 0;
    for(i=0; i<5; i++) {
        for(j=0; j<16; j++) {
            if(MapSave_GetSunRideJetFlag_((i*16)+j, saveType)) {
                num++;
            }
        }
    }
    return num;
}

void MapSave_SetMenuPage_(u8 page, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    save->menuPage = page;
}

u8 MapSave_GetMenuPage_(int saveType)
{
    MapSave *save = GetMapSave(saveType);
    if(save->menuPage < 2) {
        return 2;
    } else {
        return save->menuPage;
    }
}

void MapSave_SetMessSpeed_(u8 speed, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    save->messSpeed = speed;
}

u8 MapSave_GetMessSpeed_(int saveType)
{
    MapSave *save = GetMapSave(saveType);
    return save->messSpeed;
}

void MapSave_SetMenuShortcut_(u8 shortcut, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    save->shortcut = shortcut;
    
}

u8 MapSave_GetMenuShortcut_(int saveType)
{
    MapSave *save = GetMapSave(saveType);
    return save->shortcut;
}

void MapSave_SetMailReadFlag_(u16 flag, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    u16 word = flag/16;
    if(word >= 6) {
        return;
    }
    save->mailReadFlag[word] |= 1 << (flag-(word*16));
}

u8 MapSave_GetMailReadFlag_(u16 flag, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    u16 word = flag/16;
    if(word >= 6) {
        return 0;
    }
    return (save->mailReadFlag[word] & 1 << (flag-(word*16))) ? BL_TRUE : BL_FALSE;
}

void MapSave_SetZoneVisitFlag_(int type, int zoneType, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    MapSaveZoneVisit *visit = (type == MAP_FLAG_ZONE_VISIT) ? &save->visit : &save->visitInforme;
    switch(zoneType) {
        case MAP_ZONE_TYPE_STAR:
            visit->star = BL_TRUE;
            break;
        
        case MAP_ZONE_TYPE_MOON:
            visit->moon = BL_TRUE;
            break;
        
        case MAP_ZONE_TYPE_SUN:
            visit->sun = BL_TRUE;
            break;
        
        case MAP_ZONE_TYPE_EARTH:
            visit->earth = BL_TRUE;
            break;
    }
}

BLBool MapSave_GetZoneVisitFlag_(int type, int zoneType, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    MapSaveZoneVisit *visit = (type == MAP_FLAG_ZONE_VISIT) ? &save->visit : &save->visitInforme;
    switch(zoneType) {
        case MAP_ZONE_TYPE_STAR:
            return visit->star;
        
        case MAP_ZONE_TYPE_MOON:
            return visit->moon;
        
        case MAP_ZONE_TYPE_SUN:
            return visit->sun;
        
        case MAP_ZONE_TYPE_EARTH:
            return visit->earth;
        
        default:
            return BL_FALSE;
            
    }
}


#define QUEST_TABLE \
    QUEST_ENTRY(MAP_QUEST_JACK_BOSS, jackBossProgress, 3) \
    QUEST_ENTRY(MAP_QUEST_QUEEN_BOSS, queenBossProgress, 1) \
    QUEST_ENTRY(MAP_QUEST_CHAR_KEY, charKeyProgress, 3) \
    QUEST_ENTRY(MAP_QUEST_EVIL, evilProgress, 5) \
    QUEST_ENTRY(MAP_QUEST_GREEN_KEY, greenKeyProgress, 2) \
    QUEST_ENTRY(MAP_QUEST_AO_FLUTE, aoFluteProgress, 3) \
    QUEST_ENTRY(MAP_QUEST_AO_BASKETBALL, aoBasketballProgress, 5) \
    QUEST_ENTRY(MAP_QUEST_AO_ACE_SWIM, aoAceSwimProgress, 4) \
    QUEST_ENTRY(MAP_QUEST_MIDORI_FLUTE, midoriFluteProgress, 3) \
    QUEST_ENTRY(MAP_QUEST_MIDORI_PUZZLE, midoriPuzzleProgress, 5) \
    QUEST_ENTRY(MAP_QUEST_URANAI_AQUA, uranaiAquaProgress, 1) \
    QUEST_ENTRY(MAP_QUEST_URANAI_PRETTY, uranaiPrettyProgress, 1) \
    QUEST_ENTRY(MAP_QUEST_PRETTY, prettyQuestProgress, 3) \
    QUEST_ENTRY(MAP_QUEST_AQUA, aquaQuestProgress, 3) \
    
    
void MapSave_IncQuestProgress_(int questType, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    switch(questType) {
        #define QUEST_ENTRY(type, field, max) \
        case type: \
            if(max == 1) { \
                if(save->field != 0) { \
                    break; \
                } \
                save->field++; \
            } else { \
                if(save->field >= max) { \
                    break; \
                } \
                save->field++; \
            } \
            break;
        QUEST_TABLE
        #undef QUEST_ENTRY
    }
}

u8 MapSave_GetQuestProgress_(int questType, int saveType)
{
    MapSave *save = GetMapSave(saveType);
    switch(questType) {
        #define QUEST_ENTRY(type, field, max) \
        case type: \
            return save->field;
        QUEST_TABLE
        #undef QUEST_ENTRY
        default:
            return 0;
    }
}
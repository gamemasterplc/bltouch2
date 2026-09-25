#include "map_save.h"

#define ATRC_RECORD_LIST \
    ATRC_RECORD_1(1) \
    ATRC_RECORD_1(2) \
    ATRC_RECORD_1_SPLIT(3, 10) \
    ATRC_RECORD_1(4) \
    ATRC_RECORD_1_SPLIT(5, 6) \
    ATRC_RECORD_1(6) \
    ATRC_RECORD_1_SPLIT(7, 2) \
    ATRC_RECORD_1(8) \
    ATRC_RECORD_1(9) \
    ATRC_RECORD_SPLIT(10, 12) \
    ATRC_RECORD(11) \
    ATRC_RECORD_SPLIT(12, 8) \
    ATRC_RECORD(13) \
    ATRC_RECORD_SPLIT(14, 4) \
    ATRC_RECORD(15) \
    ATRC_RECORD(16) \
    ATRC_RECORD(17) \
    ATRC_RECORD(18) \
    ATRC_RECORD_SPLIT(19, 10) \
    ATRC_RECORD(20) \
    ATRC_RECORD_SPLIT(21, 6) \
    ATRC_RECORD(22) \
    ATRC_RECORD_SPLIT(23, 2) \
    ATRC_RECORD(24) \
    ATRC_RECORD(25) \
    ATRC_RECORD_SPLIT(26, 12) \
    ATRC_RECORD(27) \
    ATRC_RECORD_SPLIT(28, 8) \
    ATRC_RECORD(29) \
    ATRC_RECORD_SPLIT(30, 4) \
    ATRC_RECORD(31) \
    ATRC_RECORD(32) \
    ATRC_RECORD(33) \
    ATRC_RECORD(34) \
    ATRC_RECORD_SPLIT(35, 10) \
    ATRC_RECORD(36) \
    ATRC_RECORD_SPLIT(37, 6) \
    ATRC_RECORD(38) \
    ATRC_RECORD_SPLIT(39, 2) \
    ATRC_RECORD(40) \
    ATRC_RECORD(41) \
    ATRC_RECORD_SPLIT(42, 12) \
    ATRC_RECORD(43) \
    ATRC_RECORD_SPLIT(44, 8) \
    ATRC_RECORD(45)

void SetMapAtrcRecord(int mgNo, int type, u32 value)
{
    MapSaveAtrcRecord *record = MapSave_GetAtrcRecordBackup();
    u16 maskTbl[17] = { 0x0000,
        0x0001, 0x0003, 0x0007, 0x000F,
        0x001F, 0x003F, 0x007F, 0x00FF,
        0x01FF, 0x03FF, 0x07FF, 0x0FFF,
        0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF
    };
    if(!record) {
        return;
    }
    u16 hi = value >> 16;
    u16 lo = value & 0xFFFF;
    switch(type) {
        case 0:
            break;
        
        case 1:
            value = (hi*100)+lo;
            break;
        
        case 2:
            value = (hi*10)+lo;
            break;
    }
    switch(mgNo) {
        #define ATRC_RECORD(no) \
            case (no)-1:\
                record->mg##no##Record = value; \
                break;

        #define ATRC_RECORD_SPLIT(no, pos) \
        case (no)-1:\
            record->mg##no##RecordHi = (value & ~maskTbl[pos]) >> pos; \
            record->mg##no##RecordLo = value & maskTbl[pos]; \
            break;

        #define ATRC_RECORD_1(no) \
        case (no)-1:\
            record->mg0##no##Record = value; \
            break;

        #define ATRC_RECORD_1_SPLIT(no, pos) \
        case (no)-1:\
            record->mg0##no##RecordHi = (value & ~maskTbl[pos]) >> pos; \
            record->mg0##no##RecordLo = value & maskTbl[pos]; \
            break;
        ATRC_RECORD_LIST
        #undef ATRC_RECORD
        #undef ATRC_RECORD_SPLIT
        #undef ATRC_RECORD_1
        #undef ATRC_RECORD_1_SPLIT
    }
}

u32 GetMapAtrcRecord(int mgNo, int type)
{
    MapSaveAtrcRecord *record = MapSave_GetAtrcRecordBackup();
    u32 value;
    if(!record) {
        return 0;
    }
    switch(mgNo) {
        
        #define ATRC_RECORD(no) \
        case (no)-1:\
            value = record->mg##no##Record; \
            break;

        #define ATRC_RECORD_SPLIT(no, pos) \
        case (no)-1:\
            value = (record->mg##no##RecordHi << pos)|record->mg##no##RecordLo; \
            break;

        #define ATRC_RECORD_1(no) \
        case (no)-1:\
            value = record->mg0##no##Record; \
            break;

        #define ATRC_RECORD_1_SPLIT(no, pos) \
        case (no)-1:\
            value = (record->mg0##no##RecordHi << pos)|record->mg0##no##RecordLo; \
            break;

        ATRC_RECORD_LIST
        #undef ATRC_RECORD
        #undef ATRC_RECORD_SPLIT
        #undef ATRC_RECORD_1
        #undef ATRC_RECORD_1_SPLIT
    }
    switch(type) {
        case 0:
            break;

        case 1:
            value = ((value/100) << 16)|(value % 100);
            break;
        
        case 2:
            value = ((value/10) << 16)|(value % 10);
            break;
    }
    return value;
}
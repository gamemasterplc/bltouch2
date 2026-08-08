#include "sys_mode.h"
#include <nitro.h>

//TODO: Find better name for this function
extern void func_0200b16c(void);

EXTERN_OVERLAY_ID(0);
EXTERN_OVERLAY_ID(48);
EXTERN_OVERLAY_ID(49);
EXTERN_OVERLAY_ID(50);

extern void BL_Main(void *heapPtr, u32 heapSize);
extern BOOL BBP_Main(SYS_BBP_PARAM *param);

void NitroMain(void)
{
    int lock_id;
    BOOL prev;
    
    u32 resetCode;
    OS_Init();
    OS_EnableInterrupts();
    prev = OS_EnableIrq();
    lock_id = OS_GetLockID();
    CARD_LockBackup(lock_id);
    CARD_IdentifyBackup(CARD_BACKUP_TYPE_EEPROM_64KBITS);
    CARD_UnlockBackup(lock_id);
    OS_ReleaseLockID(lock_id);
    OS_RestoreIrq(prev);
    SysModeParam.heapStart = OS_GetMainArenaLo();
    SysModeParam.heapEnd = OS_GetArenaHi(2);
    if(!SysModeParam.heapEnd) {
        SysModeParam.heapEnd = OS_GetMainArenaHi();
    }
    SysModeParam.exitBBP = FALSE;
    while(1) {
        SysModeParam.isBBP = FALSE;
        FS_Init(-1);
        FS_LoadOverlay(0, OVERLAY_ID(0));
        func_0200b16c();
        BL_Main(SysModeParam.heapStart, ((u32)SysModeParam.heapEnd-(u32)SysModeParam.heapStart));
        FS_Init(-1);
        FS_UnloadOverlay(0, OVERLAY_ID(0));
        func_0200b16c();
        if(!SysIsModeBBP()) {
            resetCode = 1;
            break;
        }
        MI_CpuFill8(&SysModeParam.bbp, 0, sizeof(SysModeParam.bbp));
        SysModeParam.bbp.unk0 = 0;
        SysModeParam.bbp.heapPtr = SysModeParam.heapStart;
        SysModeParam.bbp.heapSize = ((u32)SysModeParam.heapEnd-(u32)SysModeParam.heapStart);
        SysModeParam.bbp.unkC = 0x1000;
        SysModeParam.bbp.ovlBattle = OVERLAY_ID(49);
        SysModeParam.bbp.ovlMenu = OVERLAY_ID(50);
        
        FS_Init(-1);
        FS_LoadOverlay(0, OVERLAY_ID(48));
        func_0200b16c();
        if(BBP_Main(&SysModeParam.bbp) == TRUE) {
            resetCode = 1;
            break;
        }
        FS_Init(-1);
        FS_UnloadOverlay(0, OVERLAY_ID(48));
        func_0200b16c();
        SysModeParam.exitBBP = TRUE;
    }
    OS_EnableIrq();
    OS_EnableInterrupts();
    OS_ResetSystem(resetCode);
    
}
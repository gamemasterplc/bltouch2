#include "sys_mode.h"
#include <nitro.h>

EXTERN_OVERLAY_ID(0);
EXTERN_OVERLAY_ID(48);
EXTERN_OVERLAY_ID(49);
EXTERN_OVERLAY_ID(50);

extern void BL_Main(void *heapPtr, u32 heapSize);
extern BOOL BBP_Main(SysBBPParam *param);

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
    Sys_ModeParam.heapStart = OS_GetMainArenaLo();
    Sys_ModeParam.heapEnd = OS_GetArenaHi(2);
    if(!Sys_ModeParam.heapEnd) {
        Sys_ModeParam.heapEnd = OS_GetMainArenaHi();
    }
    Sys_ModeParam.exitBBP = FALSE;
    while(1) {
        Sys_ModeParam.isBBP = FALSE;
        FS_Init(-1);
        FS_LoadOverlay(0, OVERLAY_ID(0));
        FS_Close();
        BL_Main(Sys_ModeParam.heapStart, ((u32)Sys_ModeParam.heapEnd-(u32)Sys_ModeParam.heapStart));
        FS_Init(-1);
        FS_UnloadOverlay(0, OVERLAY_ID(0));
        FS_Close();
        if(!Sys_IsModeBBP()) {
            resetCode = 1;
            break;
        }
        MI_CpuFill8(&Sys_ModeParam.bbp, 0, sizeof(Sys_ModeParam.bbp));
        Sys_ModeParam.bbp.unk0 = 0;
        Sys_ModeParam.bbp.heapPtr = Sys_ModeParam.heapStart;
        Sys_ModeParam.bbp.heapSize = ((u32)Sys_ModeParam.heapEnd-(u32)Sys_ModeParam.heapStart);
        Sys_ModeParam.bbp.unkC = 0x1000;
        Sys_ModeParam.bbp.ovlBattle = OVERLAY_ID(49);
        Sys_ModeParam.bbp.ovlMenu = OVERLAY_ID(50);
        
        FS_Init(-1);
        FS_LoadOverlay(0, OVERLAY_ID(48));
        FS_Close();
        if(BBP_Main(&Sys_ModeParam.bbp) == TRUE) {
            resetCode = 1;
            break;
        }
        FS_Init(-1);
        FS_UnloadOverlay(0, OVERLAY_ID(48));
        FS_Close();
        Sys_ModeParam.exitBBP = TRUE;
    }
    OS_EnableIrq();
    OS_EnableInterrupts();
    OS_ResetSystem(resetCode);
    
}
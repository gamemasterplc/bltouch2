#include "gpu_reg.h"

int BL_GpuDispSelect;
s16 BL_GpuMasterBrightness[DISPLAY_MAX];
int BL_GpuPlaneMask[DISPLAY_MAX];
BLGpuRegBlend BL_GpuRegBlend[DISPLAY_MAX];
BLGpuRegWindow BL_GpuRegWindow[DISPLAY_MAX];
BLGpuRegScroll BL_GpuRegScroll[DISPLAY_MAX];

void BL_GpuRegInit(void)
{
    int i;
    for(i=0; i<DISPLAY_MAX; i++) {
        BL_GpuPlaneMask[i] = GX_PLANEMASK_NONE;
        MI_CpuFill16(0, &BL_GpuRegScroll[i], sizeof(BLGpuRegScroll));
        MI_CpuFill16(0, &BL_GpuRegWindow[i], sizeof(BLGpuRegWindow));
        MI_CpuFill16(0, &BL_GpuRegBlend[i], 6);
    }
    
    G2_SetBG0Priority(0);
    G2_SetBG1Priority(0);
    G2_SetBG2Priority(0);
    G2_SetBG3Priority(0);
    G2S_SetBG0Priority(0);
    G2S_SetBG1Priority(0);
    G2S_SetBG2Priority(0);
    G2S_SetBG3Priority(0);
}

void BL_GpuRegExec(void)
{
    //Necessary so that inlines do not get their own copy of the 
    volatile int DISPCNT;
    GX_SetDispSelect(BL_GpuDispSelect);
    
    DISPCNT = (REG_DISPCNT & ~0xFF00);
    DISPCNT |= (BL_GpuPlaneMask[DISPLAY_MAIN] & 0xFF00);
    REG_DISPCNT = DISPCNT;
    
    MI_CpuCopy16(&BL_GpuRegScroll[DISPLAY_MAIN], (void *)(&REG_BG0OFS), sizeof(BLGpuRegScroll));
    MI_CpuCopy16(&BL_GpuRegWindow[DISPLAY_MAIN], (void *)(&REG_WIN0H), sizeof(BLGpuRegWindow));
    MI_CpuCopy16(&BL_GpuRegBlend[DISPLAY_MAIN], (void *)(&REG_BLDCNT), 6);
    GX_SetMasterBrightness(BL_GpuMasterBrightness[DISPLAY_MAIN]);
    
    DISPCNT = (REG_DISPCNT_SUB & ~0xFF00);
    DISPCNT |= (BL_GpuPlaneMask[DISPLAY_SUB] & 0xFF00);
    REG_DISPCNT_SUB = DISPCNT;
    
    MI_CpuCopy16(&BL_GpuRegScroll[DISPLAY_SUB], (void *)(&REG_BG0OFS_SUB), sizeof(BLGpuRegScroll));
    MI_CpuCopy16(&BL_GpuRegWindow[DISPLAY_SUB], (void *)(&REG_WIN0H_SUB), sizeof(BLGpuRegWindow));
    MI_CpuCopy16(&BL_GpuRegBlend[DISPLAY_SUB], (void *)(&REG_BLDCNT_SUB), 6);
    GXS_SetMasterBrightness(BL_GpuMasterBrightness[DISPLAY_SUB]);
}
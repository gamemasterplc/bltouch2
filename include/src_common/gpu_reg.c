#include "gpu_reg.h"

int GpuDispSelect;
s16 GpuMasterBrightness[GPU_DISPLAY_MAX];
int GpuPlaneMask[GPU_DISPLAY_MAX];
GPU_REG_BLEND GpuRegBlend[GPU_DISPLAY_MAX];
GPU_REG_WINDOW GpuRegWindow[GPU_DISPLAY_MAX];
GPU_REG_SCROLL GpuRegScroll[GPU_DISPLAY_MAX];

void GpuRegInit(void)
{
    int i;
    for(i=0; i<GPU_DISPLAY_MAX; i++) {
        GpuPlaneMask[i] = GX_PLANEMASK_NONE;
        MI_CpuFill16(0, &GpuRegScroll[i], sizeof(GPU_REG_SCROLL));
        MI_CpuFill16(0, &GpuRegWindow[i], sizeof(GPU_REG_WINDOW));
        MI_CpuFill16(0, &GpuRegBlend[i], 6);
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

void GpuRegExec(void)
{
    //Necessary so that inlines do not get their own copy of the 
    volatile int DISPCNT;
    GX_SetDispSelect(GpuDispSelect);
    
    DISPCNT = (REG_DISPCNT & ~0xFF00);
    DISPCNT |= (GpuPlaneMask[GPU_DISPLAY_MAIN] & 0xFF00);
    REG_DISPCNT = DISPCNT;
    
    MI_CpuCopy16(&GpuRegScroll[GPU_DISPLAY_MAIN], (void *)(&REG_BG0OFS), sizeof(GPU_REG_SCROLL));
    MI_CpuCopy16(&GpuRegWindow[GPU_DISPLAY_MAIN], (void *)(&REG_WIN0H), sizeof(GPU_REG_WINDOW));
    MI_CpuCopy16(&GpuRegBlend[GPU_DISPLAY_MAIN], (void *)(&REG_BLDCNT), 6);
    GX_SetMasterBrightness(GpuMasterBrightness[GPU_DISPLAY_MAIN]);
    
    DISPCNT = (REG_DISPCNT_SUB & ~0xFF00);
    DISPCNT |= (GpuPlaneMask[GPU_DISPLAY_SUB] & 0xFF00);
    REG_DISPCNT_SUB = DISPCNT;
    
    MI_CpuCopy16(&GpuRegScroll[GPU_DISPLAY_SUB], (void *)(&REG_BG0OFS_SUB), sizeof(GPU_REG_SCROLL));
    MI_CpuCopy16(&GpuRegWindow[GPU_DISPLAY_SUB], (void *)(&REG_WIN0H_SUB), sizeof(GPU_REG_WINDOW));
    MI_CpuCopy16(&GpuRegBlend[GPU_DISPLAY_SUB], (void *)(&REG_BLDCNT_SUB), 6);
    GXS_SetMasterBrightness(GpuMasterBrightness[GPU_DISPLAY_SUB]);
}
#ifndef BL_GPU_REG_H
#define BL_GPU_REG_H

#include <nitro.h>
#include "display.h"

typedef struct BLGpuRegScroll_s {
    s16 BG0HOFS;
    s16 BG0VOFS;
    s16 BG1HOFS;
    s16 BG1VOFS;
    s16 BG2HOFS;
    s16 BG2VOFS;
    s16 BG3HOFS;
    s16 BG3VOFS;
    u32 BG2AFFINE[4];
    u32 BG3AFFINE[4];
} BLGpuRegScroll;

typedef struct BLGpuRegWindow_s {
    u16 WIN0H;
    u16 WIN1H;
    u16 WIN0V;
    u16 WIN1V;
    u16 WININ;
    u16 WINOUT;
    u16 MOSAIC;
} BLGpuRegWindow;

typedef struct BLGpuRegBlend_s {
    u16 BLDCNT;
    u16 BLDALPHA;
    u16 BLDY;
    u16 pad;
} BLGpuRegBlend;

#ifdef SYS_BBP
#define BL_GpuMasterBrightness BBP_GpuMasterBrightness
#define BL_GpuDispSelect BBP_GpuDispSelect
#define BL_GpuPlaneMask BBP_GpuPlaneMask
#define BL_GpuRegBlend BBP_GpuRegBlend
#define BL_GpuRegWindow BBP_GpuRegWindow
#define BL_GpuRegScroll BBP_GpuRegScroll
#define BL_GpuRegInit BBP_GpuRegInit
#define BL_GpuRegExec BBP_GpuRegExec
#endif


extern int BL_GpuDispSelect;
extern s16 BL_GpuMasterBrightness[DISPLAY_MAX];
extern int BL_GpuPlaneMask[DISPLAY_MAX];
extern BLGpuRegBlend BL_GpuRegBlend[DISPLAY_MAX];
extern BLGpuRegWindow BL_GpuRegWindow[DISPLAY_MAX];
extern BLGpuRegScroll BL_GpuRegScroll[DISPLAY_MAX];

void BL_GpuRegInit(void);
void BL_GpuRegExec(void);

#endif
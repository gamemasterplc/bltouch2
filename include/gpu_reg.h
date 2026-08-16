#ifndef GPU_REG_H
#define GPU_REG_H

#include <nitro.h>

#define GPU_DISPLAY_MAIN 0
#define GPU_DISPLAY_SUB 1
#define GPU_DISPLAY_MAX 2

typedef struct GpuRegScroll_s {
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
} GPU_REG_SCROLL;

typedef struct GpuRegWindow_s {
    u16 WIN0H;
    u16 WIN1H;
    u16 WIN0V;
    u16 WIN1V;
    u16 WININ;
    u16 WINOUT;
    u16 MOSAIC;
} GPU_REG_WINDOW;

typedef struct GpuRegBlend_s {
    u16 BLDCNT;
    u16 BLDALPHA;
    u16 BLDY;
    u16 pad;
} GPU_REG_BLEND;

#ifdef SYS_BBP
#define GpuMasterBrightness BBP_GpuMasterBrightness
#define GpuDispSelect BBP_GpuDispSelect
#define GpuPlaneMask BBP_GpuPlaneMask
#define GpuRegBlend BBP_GpuRegBlend
#define GpuRegWindow BBP_GpuRegWindow
#define GpuRegScroll BBP_GpuRegScroll
#define GpuRegInit BBP_GpuRegInit
#define GpuRegExec BBP_GpuRegExec
#else
#define GpuMasterBrightness BL_GpuMasterBrightness
#define GpuDispSelect BL_GpuDispSelect
#define GpuPlaneMask BL_GpuPlaneMask
#define GpuRegBlend BL_GpuRegBlend
#define GpuRegWindow BL_GpuRegWindow
#define GpuRegScroll BL_GpuRegScroll
#define GpuRegInit BL_GpuRegInit
#define GpuRegExec BL_GpuRegExec
#endif


extern int GpuDispSelect;
extern s16 GpuMasterBrightness[GPU_DISPLAY_MAX];
extern int GpuPlaneMask[GPU_DISPLAY_MAX];
extern GPU_REG_BLEND GpuRegBlend[GPU_DISPLAY_MAX];
extern GPU_REG_WINDOW GpuRegWindow[GPU_DISPLAY_MAX];
extern GPU_REG_SCROLL GpuRegScroll[GPU_DISPLAY_MAX];

void GpuRegInit(void);
void GpuRegExec(void);

#endif
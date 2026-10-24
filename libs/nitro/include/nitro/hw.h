#ifndef _NITRO_HW_H
#define _NITRO_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#define HW_PLTT      ((void*)0x05000000)
#define HW_PLTT_SIZE 0x400

#define HW_DB_PLTT      ((void*)0x05000400)
#define HW_DB_PLTT_SIZE 0x400

#define HW_BG_VRAM      ((void*)0x06000000)
#define HW_BG_VRAM_SIZE // TODO

#define HW_DB_BG_VRAM      ((void*)0x06200000)
#define HW_DB_BG_VRAM_SIZE // TODO

#define HW_OBJ_VRAM      ((void*)0x06400000)
#define HW_OBJ_VRAM_SIZE // TODO

#define HW_DB_OBJ_VRAM      ((void*)0x06600000)
#define HW_DB_OBJ_VRAM_SIZE // TODO

#define HW_LCDC_VRAM      ((void*)0x06800000)
#define HW_LCDC_VRAM_SIZE 0xA4000

//Guessed VRAM Bank Defines from Princess Maker 4 (explicitly names HW_LCDC_VRAM_A and HW_LCDC_VRAM_B)

#define HW_LCDC_VRAM_A ((void*)0x06800000)
#define HW_LCDC_VRAM_A_SIZE 0x20000

#define HW_LCDC_VRAM_B ((void*)0x06820000)
#define HW_LCDC_VRAM_B_SIZE 0x20000

#define HW_LCDC_VRAM_C ((void*)0x06840000)
#define HW_LCDC_VRAM_C_SIZE 0x20000

#define HW_LCDC_VRAM_D ((void*)0x06860000)
#define HW_LCDC_VRAM_D_SIZE 0x20000

#define HW_LCDC_VRAM_E ((void*)0x06880000)
#define HW_LCDC_VRAM_E_SIZE 0x10000

#define HW_LCDC_VRAM_F ((void*)0x06890000)
#define HW_LCDC_VRAM_F_SIZE 0x4000

#define HW_LCDC_VRAM_G ((void*)0x06894000)
#define HW_LCDC_VRAM_G_SIZE 0x4000

#define HW_LCDC_VRAM_H ((void*)0x06898000)
#define HW_LCDC_VRAM_H_SIZE 0x8000

#define HW_LCDC_VRAM_I ((void*)0x068A0000)
#define HW_LCDC_VRAM_I_SIZE 0x4000

#define HW_OAM      ((void*)0x07000000)
#define HW_OAM_SIZE 0x400

#define HW_DB_OAM      ((void*)0x07000400)
#define HW_DB_OAM_SIZE 0x400

#ifdef __cplusplus
} // extern "C"
#endif

#endif
#ifndef RANDOM_H
#define RANDOM_H

#include <nitro/types.h>

#ifdef SYS_BBP
#define BL_RandomInit BBP_RandomInit
#define BL_RandomSetSeed BBP_RandomSetSeed 
#define BL_RandomInt BBP_RandomInt 
#define BL_RandomGetSeed BBP_RandomGetSeed 

#endif

void BL_RandomInit(void);
void BL_RandomSetSeed(u32 value);
u16 BL_RandomInt(u16 max);
u32 BL_RandomGetSeed(void);

#endif
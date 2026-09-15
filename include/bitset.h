#ifndef BITSET_H
#define BITSET_H

#include <nitro/types.h>

#ifdef SYS_BBP
#define BL_BitSetCountSet BBP_BitSetCountSet
#define BL_BitSetGetEnd BBP_BitSetGetEnd
#endif

u16 BL_BitSetCountSet(u32 *arr, int startBit, int endBit);
u16 BL_BitSetGetEnd(u32 *arr, int minSet, int startBit, int endBit);

#endif
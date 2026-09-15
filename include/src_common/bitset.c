#include "bitset.h"

static u8 IsBitSet(u32 *arr, u16 bit)
{
    if(arr[bit >> 5] & (1 << (bit & 0x1F))) {
        return 1;
    } else {
        return 0;
    }
}
    
u16 BL_BitSetCountSet(u32 *arr, int startBit, int endBit)
{
    u16 numOnes;
    int i;
    numOnes = 0;
    for(i=startBit; i<endBit; i++) {
        if(IsBitSet(arr, i)) {
            numOnes++;
        }
    }
    return numOnes;
}

#ifdef SYS_BBP
u16 BL_BitSetGetEnd(u32 *arr, int minSet, int startBit, int endBit)
{
    int numOnes;
    int i;
    numOnes = 0;
    for(i=startBit; i<endBit; i++) {
        if(IsBitSet(arr, i)) {
            if(numOnes++ == minSet) {
                break;
            }
        }
    }
    return i;
}
#endif
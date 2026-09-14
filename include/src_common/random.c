#include "random.h"
#include <nitro.h>

static struct {
    u32 seed;
    u32 mul;
    u32 add;
    
} RandomWork;

void BL_RandomInit(void)
{
    RTCTime time;
    RTC_Init();
    RTC_GetTime(&time);
    BL_RandomSetSeed((time.hour*3600)+(time.minute*60)+time.second);
}

void BL_RandomSetSeed(u32 value)
{
    RandomWork.seed = value;
    RandomWork.mul = 0x5d588b65;
    RandomWork.add = 0x269ec3;
}

u16 BL_RandomInt(u16 max)
{
    RandomWork.seed = (RandomWork.mul*RandomWork.seed)+RandomWork.add;
    if(max == 0) {
        return RandomWork.seed >> 16;
    } else {
        return ((RandomWork.seed >> 16)*max)>>16;
    }
}

u32 BL_RandomGetSeed(void)
{
    return RandomWork.seed;
}
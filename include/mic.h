#ifndef MIC_H
#define MIC_H

#include <nitro/types.h>

#ifdef SYS_BBP
#define BL_MicAmpOff BBP_MicAmpOff
#endif

BOOL BL_MicAmpOff(void);

#endif
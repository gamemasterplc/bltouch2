#ifndef _NITRO_FX_H
#define _NITRO_FX_H

#include "nitro/types.h"

#include <nitro/fx/fx_atan.h>
#include <nitro/fx/fx_division.h>
#include <nitro/fx/fx_init.h>
#include <nitro/fx/fx_matrix.h>
#include <nitro/fx/fx_vector.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FX32_SHIFT 12
#define FX16_SHIFT 12

#define FX32_ONE (fx32)(1 << FX32_SHIFT)
#define FX16_ONE (fx16)(1 << FX16_SHIFT)

#define FX32_CONVERT(x) (fx32)(((x) > 0) ? (((x) << 12)+0.5f) : (((x) << 12)-0.5f))


typedef s32 fx32;
typedef s16 fx16;

extern const fx16 FX_SinCosTable_[];

static inline fx32 FX_Mul(fx32 x, fx32 y)
{
    return (((s64)x*(s64)y)+0x800) >> FX32_SHIFT;
}

static inline fx32 FX_SinIdx(u16 angle)
{
    return FX_SinCosTable_[(angle >> 4)*2];
}

static inline fx32 FX_CosIdx(u16 angle)
{
    return FX_SinCosTable_[((angle >> 4)*2)+1];
}


typedef union VecFx32 {
    struct {
        /* 00 */ fx32 x;
        /* 04 */ fx32 y;
        /* 08 */ fx32 z;
        /* 0c */
    };
    fx32 coords[3];
} VecFx32;

typedef union MtxFx22 {
    struct {
        /* 00 */ fx32 x;
        /* 04 */ fx32 y;
        /* 08 */ fx32 z;
        /* 0c */ fx32 w;
        /* 10 */
    };
    fx32 coords[4];
} MtxFx22;

void FX_Init(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
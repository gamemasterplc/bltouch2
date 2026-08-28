#ifndef _NITRO_MI_H
#define _NITRO_MI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>

#define MI_DMA_MAX_NUM 3

void MI_DmaFill32(u32, void* ptr, u8 value, u32 size);
void MI_DmaCopy16(u32, void* src, void* dst, u32 size);
void MI_DmaCopy32(u32, void* src, void* dst, u32 size);
void MI_func_0206d87c(u32, void* src, void* dst, u32 size, u32, u32);
void MI_func_0206d934(u32);
BOOL MI_IsDmaBusy(u32);

void MIi_CpuCopyFast(void* src, void* dest, u32 size);
void MIi_CpuClearFast(u32 value, void* ptr, u32 size);
void MI_CpuFill8(void* ptr, u8 value, u32 size);
void MI_CpuCopy8(void* src, void* dst, u32 size);
void MIi_CpuCopy16(void* src, void* dst, u32 size);
void MIi_CpuCopy32(void* src, void* dst, u32 size);
void MIi_CpuClear16(int value, void* dst, size_t size);
void MIi_CpuClear32(u32 value, void* ptr, u32 size);
void MI_Swap(u32* a, u32* b);
void MI_UncompressLZ8(void *src, void *dst);

inline void MI_CpuCopy16(void* src, void* dst, u32 size)
{
    MIi_CpuCopy16(src, dst, size);
}

inline void MI_CpuFill16(int value, void* dst, size_t size)
{
    MIi_CpuClear16(value, dst, size);
}

inline void MI_CpuCopy32(void* src, void* dst, u32 size)
{
    MIi_CpuCopy32(src, dst, size);
}

inline void MI_CpuFill32(int value, void* dst, size_t size)
{
    MIi_CpuClear32(value, dst, size);
}

inline void MI_CpuClearFast(void* ptr, u32 size) {
    MIi_CpuClearFast(0, ptr, size);
}

inline void MI_CpuFillFast(void* ptr, u8 value, u32 size) {
    MIi_CpuClearFast(value, ptr, size);
}

inline void MI_CpuCopyFast(void* src, void* dest, u32 size) {
    MIi_CpuCopyFast(src, dest, size);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif
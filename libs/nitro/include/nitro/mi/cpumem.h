#ifndef MI_CPUMEM_H
#define MI_CPUMEM_H

#include <nitro/types.h>

// Fill memory with a 16-bit value
void MI_CpuFill16(register u16 value, register u16* dest, register u32 count);

// Copy 16-bit values from source to destination
void MI_CpuCopy16(register const u16* src, register u16* dest, register u32 count);

// Fill memory with an unsigned 32-bit value
void MI_CpuFill32(register u32 value, register u32* dest, register u32 count);

// Copy 32-bit values from source to destination
void MI_CpuCopy32(register const u32* src, register u32* dest, register u32 count);

// Fill memory with a 32-bit value
void MI_CpuFillFast(register u32 value, register void* dest, register u32 count);

// Copy 32-bit values from source to destination
void MI_CpuCopyFast(register const u32* src, register u32* dest, register u32 count);

// Set memory to a byte value (similar to memset)
void MI_CpuFill8(register void* dest, register u8 value, register u32 count);

// Copy bytes from source to destination (similar to memcpy)
void MI_CpuCopy8(register const void* src, register void* dest, register u32 count);

// Clear exactly 24 bytes (6 words) to zero
void MI_CpuClear24(register u32* dest);

// Atomically swap a value with a memory location, returning the old value
u32 MI_CpuSwap(register u32 value, register u32* addr);

#endif // MI_CPUMEM_H
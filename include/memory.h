#ifndef MEMORY_H
#define MEMORY_H

#include <nitro/types.h>

typedef enum MemTag_s {
    MEM_TAG_FILE_TABLE = 0,
    MEM_TAG_SYSTEM = 1,
    MEM_TAG_OBJECT = 2,
    MEM_TAG_FILE = 6,
    MEM_TAG_SCENE_BEGIN = 7, 
    MEM_TAG_SCENE_GFX_HEAP = 7,
    MEM_TAG_SCENE_VBLANK_QUEUE = 8,
    MEM_TAG_SCENE_SPRITE = 9,
    MEM_TAG_SCENE_COLOR_ANIM = 10,
    MEM_TAG_SCENE_IMAGE_3D = 11,
    MEM_TAG_SCENE_GENERIC = 12,
    
    MEM_TAG_MAX = 14,
} MEM_TAG;

#define MEM_FLAG_CLEAR (1 << 0)

#ifndef SYS_BBP
#define MemInit BL_MemInit
#define MemAlloc BL_MemAlloc
#define MemFree BL_MemFree
#define MemFreeTag BL_MemFreeTag
#define MemShrink BL_MemShrink
#define MemGetAllocSize BL_MemGetAllocSize
#else
#define MemInit BBP_MemInit
#define MemAlloc BBP_MemAlloc
#define MemFree BBP_MemFree
#define MemFreeTag BBP_MemFreeTag
#define MemShrink BBP_MemShrink
#define MemGetAllocSize BBP_MemGetAllocSize
#endif

void MemInit(void *heapBase, u32 size);
void *MemAlloc(MEM_TAG tag, size_t size, u32 flag);
void MemFree(void *ptr);
void MemFreeTag(MEM_TAG tag);
void MemShrink(void *ptr, u32 amount);
u32 MemGetAllocSize(void *ptr);

#endif
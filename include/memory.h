#ifndef BL_MEMORY_H
#define BL_MEMORY_H

#include <nitro/types.h>

#ifndef SYS_BBP
typedef enum BLMemTag_e {
    BL_MEM_TAG_FILE_TABLE = 0,
    BL_MEM_TAG_SYSTEM = 1,
    BL_MEM_TAG_OBJECT = 2,
    BL_MEM_TAG_FILE_TEMP = 4,
    BL_MEM_TAG_FILE = 6,
    BL_MEM_TAG_SCENE_BEGIN = 7, 
    BL_MEM_TAG_SCENE_GFX_HEAP = 7,
    BL_MEM_TAG_SCENE_VBLANK_QUEUE = 8,
    BL_MEM_TAG_SCENE_SPRITE = 9,
    BL_MEM_TAG_SCENE_COLOR_ANIM = 10,
    BL_MEM_TAG_SCENE_IMAGE_3D = 11,
    BL_MEM_TAG_SCENE_GENERIC = 12,
    
    BL_MEM_TAG_MAX = 14,
} BLMemTag;
#else
typedef enum BLMemTag_e {
    BL_MEM_TAG_FILE_TABLE = 0,
    BL_MEM_TAG_SYSTEM = 1,
    BL_MEM_TAG_OBJECT = 2,
    BL_MEM_TAG_FILE_TEMP = 4,
    BL_MEM_TAG_FILE = 7,
    BL_MEM_TAG_SCENE_BEGIN = 7, 
    BL_MEM_TAG_SCENE_GFX_HEAP = 8,
    BL_MEM_TAG_SCENE_VBLANK_QUEUE = 8,
    BL_MEM_TAG_SCENE_SPRITE = 9,
    BL_MEM_TAG_SCENE_COLOR_ANIM = 10,
    BL_MEM_TAG_SCENE_IMAGE_3D = 11,
    BL_MEM_TAG_SCENE_GENERIC = 12,
    
    BL_MEM_TAG_MAX = 14,
} BLMemTag;
#endif

#define BL_MEM_FLAG_CLEAR (1 << 0)

#ifdef SYS_BBP
#define BL_MemInit BBP_MemInit
#define BL_MemAlloc BBP_MemAlloc
#define BL_MemFree BBP_MemFree
#define BL_MemFreeTag BBP_MemFreeTag
#define BL_MemShrink BBP_MemShrink
#define BL_MemGetAllocSize BBP_MemGetAllocSize
#endif

#define BL_MemCalloc(tag, size) BL_MemAlloc(tag, size, BL_MEM_FLAG_CLEAR)

void BL_MemInit(void *heapBase, u32 size);
void *BL_MemAlloc(BLMemTag tag, size_t size, u32 flag);
void BL_MemFree(void *ptr);
void BL_MemFreeTag(BLMemTag tag);
void BL_MemShrink(void *ptr, u32 amount);
u32 BL_MemGetAllocSize(void *ptr);

#endif
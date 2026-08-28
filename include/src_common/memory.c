#include <nitro.h>
#include "memory.h"

#define HEAP_ALIGN 31
#define HEAP_BLOCK_SIZE 32
#define HEAP_ALLOC_LIST_MAX 4096
#ifndef SYS_BBP
#define HEAP_MAX_SIZE 0x400000
#else
#define HEAP_MAX_SIZE 0x200000

#endif

typedef struct HeapBlock_s HEAP_BLOCK;

#ifndef SYS_BBP
struct HeapBlock_s {
    u32 magic;
    u32 serialNum;
    u32 tag;
    u32 blockSize;
    HEAP_BLOCK *prev;
    HEAP_BLOCK *next;
    u32 allocSize;
};
#else
struct HeapBlock_s {
    u32 magic;    
    u32 tag;
    u32 blockSize;
    u32 allocSize;
    HEAP_BLOCK *prev;
    HEAP_BLOCK *next;
};    
#endif

typedef struct HeapAllocEntry_s {
    u32 serialNum;
    HEAP_BLOCK *blockPtr;
} HEAP_ALLOC_ENTRY;

typedef struct HeapWork_s {
    #ifndef SYS_BBP
    u32 serialNum;
    #endif
    HEAP_BLOCK *firstBlock;
    HEAP_BLOCK *lastBlock;
    u32 allocBlockNum;
    #ifndef SYS_BBP
    HEAP_ALLOC_ENTRY entry[2][HEAP_ALLOC_LIST_MAX];
    #endif
} HEAP_WORK;

static HEAP_WORK *HeapWork;

static void MemAddBlock(HEAP_BLOCK *block, u32 blockSize);
static void MemRemoveBlock(HEAP_BLOCK *block);
static BOOL MemAddAllocList(u32 baseBlockNum, int blockNum, u32 serialNum, HEAP_BLOCK *block);
static BOOL MemRemoveAllocList(u32 baseBlockNum, u32 blockNum, HEAP_BLOCK *block);

#ifdef SYS_BBP
#define ENTER_CRITICAL_SECTION() { BOOL prevIrq = OS_DisableIrq();
#define EXIT_CRITICAL_SECTION() OS_RestoreIrq(prevIrq); }
#else
#define ENTER_CRITICAL_SECTION() { OS_DisableIrq();
#define EXIT_CRITICAL_SECTION() OS_EnableIrq(); }
#endif

void BL_MemInit(void *heapBase, u32 size)
{
    u32 initHeapSize;
    void *newHeapPtr;
    
    HeapWork = (HEAP_WORK *)(((u8 *)heapBase+size)-sizeof(HEAP_WORK));
    MI_CpuFill8(HeapWork, 0, sizeof(HEAP_WORK));
    newHeapPtr = heapBase;
    initHeapSize = size-sizeof(HEAP_WORK);
    if((u32)newHeapPtr & HEAP_ALIGN) {
        newHeapPtr = (((u8 *)heapBase)+(HEAP_BLOCK_SIZE))-((u32)newHeapPtr & HEAP_ALIGN);
        initHeapSize -= (u32)newHeapPtr-(u32)heapBase;
    }
    if(initHeapSize & HEAP_ALIGN) {
        initHeapSize -= (initHeapSize & HEAP_ALIGN);
    }
    if(initHeapSize > HEAP_MAX_SIZE) {
        initHeapSize = HEAP_MAX_SIZE;
    }
    MemAddBlock(newHeapPtr, initHeapSize);
}

//TODO: Reg Swaps in BBP
void *BL_MemAlloc(BLMemTag tag, size_t size, u32 flag)
{
    void *ret;
    HEAP_BLOCK *newBlock;
    u32 blockSize;
    u32 freeSpace;
    HEAP_BLOCK *curBlock;    
    
    ENTER_CRITICAL_SECTION();
    ret = NULL;
    #ifndef SYS_BBP
    if(HeapWork->allocBlockNum >= HEAP_ALLOC_LIST_MAX) {
        goto end;
    }
    #endif
    blockSize = (size+HEAP_ALIGN)& ~HEAP_ALIGN;
    newBlock = NULL;
    for(curBlock=HeapWork->lastBlock; curBlock;) {
        #ifdef SYS_BBP
        HEAP_BLOCK *temp = curBlock->next;
        #endif
        if(curBlock->blockSize >= blockSize+HEAP_BLOCK_SIZE) {
            freeSpace = curBlock->blockSize-(blockSize+HEAP_BLOCK_SIZE);
            MemRemoveBlock(curBlock);
            newBlock = curBlock;
            if(freeSpace) {
                MemAddBlock((HEAP_BLOCK *)((u8 *)newBlock+(blockSize+HEAP_BLOCK_SIZE)), freeSpace);
            }
            break;
        }
        #ifndef SYS_BBP
        curBlock = curBlock->next;
        #else
        curBlock = temp;
        #endif
    }
    #ifndef SYS_BBP
    if(!newBlock) {
        goto end;
    }
    #endif
    #ifndef SYS_BBP
    if(!MemAddAllocList(0, HeapWork->allocBlockNum+1, HeapWork->serialNum, newBlock)) {
        goto end;
    }
    #endif
    if(flag & BL_MEM_FLAG_CLEAR) {
        MI_CpuClearFast(newBlock, blockSize+HEAP_BLOCK_SIZE);
    }
    newBlock->magic = 'HEAP';
    #ifndef SYS_BBP
    newBlock->serialNum = HeapWork->serialNum;
    #endif
    newBlock->tag = tag;
    newBlock->blockSize = blockSize+HEAP_BLOCK_SIZE;
    newBlock->prev = NULL;
    newBlock->next = HeapWork->firstBlock;
    if(newBlock->next) {
        newBlock->next->prev = newBlock;
    }
    newBlock->allocSize = size;
    ret = (u8 *)newBlock+HEAP_BLOCK_SIZE;
    HeapWork->allocBlockNum++;
    #ifndef SYS_BBP
    HeapWork->serialNum++;
    #endif
    HeapWork->firstBlock = newBlock;
    end:
    EXIT_CRITICAL_SECTION();
    return ret;
}

void BL_MemFree(void *ptr)
{
    HEAP_BLOCK *block;
    #ifdef SYS_BBP
    if(!ptr) {
        return;
    }
    #endif
    
    ENTER_CRITICAL_SECTION();
    block = (HEAP_BLOCK *)((u8 *)ptr-HEAP_BLOCK_SIZE);
    #ifndef SYS_BBP
    if(block->magic != 'HEAP') {
        goto end;
    }
    if(!((block->prev || block->next || HeapWork->firstBlock == block) && MemRemoveAllocList(0, HeapWork->allocBlockNum, block))) {
        goto end;
    }
    #else 
    HeapWork->allocBlockNum--;
    #endif
    if(block->prev) {
        block->prev->next = block->next;
    }
    if(block->next) {
        block->next->prev = block->prev;
    }
    
    if(HeapWork->firstBlock == block) {
        HeapWork->firstBlock = block->next;
    }
    MemAddBlock(block, block->blockSize);
    end:
    EXIT_CRITICAL_SECTION();
}

void BL_MemFreeTag(BLMemTag tag)
{
    HEAP_BLOCK *block;
    
    if(tag >= BL_MEM_TAG_MAX) {
        return;
    }
    #ifdef SYS_BBP
    ENTER_CRITICAL_SECTION();
    #endif
    for(block=HeapWork->firstBlock; block;) {
        HEAP_BLOCK *next = block->next;
        if(block->tag == tag) {
            BL_MemFree((u8 *)block+HEAP_BLOCK_SIZE);
        }
        block = next;
    }
    #ifdef SYS_BBP
    EXIT_CRITICAL_SECTION();
    #endif
}

void BL_MemShrink(void *ptr, u32 amount)
{
    HEAP_BLOCK *block;
    #ifndef SYS_BBP
    ENTER_CRITICAL_SECTION();
    #endif
    block = (HEAP_BLOCK *)((u8 *)ptr-HEAP_BLOCK_SIZE);
    
    if(block->blockSize < amount+HEAP_BLOCK_SIZE) {
        goto end;
    }
    if(amount & HEAP_ALIGN) {
        amount -= amount & HEAP_ALIGN;
    }
    if(amount == 0) {
        goto end;
    }
    block->blockSize -= amount;
    block->allocSize -= amount;
    MemAddBlock((HEAP_BLOCK *)((u8 *)block+block->blockSize), amount);
    
    end:
    #ifndef SYS_BBP
    EXIT_CRITICAL_SECTION();
    #endif
}

u32 BL_MemGetAllocSize(void *ptr)
{
    HEAP_BLOCK *block = (HEAP_BLOCK *)((u8 *)ptr-HEAP_BLOCK_SIZE);
    return block->allocSize;
}


static void MemAddBlock(HEAP_BLOCK *block, u32 blockSize)
{
    HEAP_BLOCK *iterBlock;
    HEAP_BLOCK *prev;
    if(blockSize < HEAP_BLOCK_SIZE) {
        return;
    }
    #ifdef SYS_BBP
    ENTER_CRITICAL_SECTION();
    #endif
    block->magic = 'HEAP';
    #ifndef SYS_BBP
    block->serialNum = 0;
    #endif
    block->tag = BL_MEM_TAG_MAX;
    block->prev = NULL;
    block->next = NULL;
    block->blockSize = blockSize;
    block->allocSize = 0;
    for(iterBlock=HeapWork->lastBlock; iterBlock; ) {
        #ifdef SYS_BBP
        HEAP_BLOCK *next = iterBlock->next;
        #endif
        if(block == (HEAP_BLOCK *)((u8 *)iterBlock+iterBlock->blockSize)) {
            MemRemoveBlock(iterBlock);
            MemAddBlock(iterBlock, iterBlock->blockSize+block->blockSize);
            goto end;
        } else if(iterBlock == (HEAP_BLOCK *)((u8 *)block+block->blockSize)) {
            MemRemoveBlock(iterBlock);
            MemAddBlock(block, block->blockSize+iterBlock->blockSize);
            goto end;
        }
        #ifdef SYS_BBP
        iterBlock = next;
        #else
        iterBlock=iterBlock->next;
        #endif
    }
    prev = NULL;
    for(iterBlock=HeapWork->lastBlock; iterBlock;) {
        #ifdef SYS_BBP
        HEAP_BLOCK *next = iterBlock->next;
        #endif
        if(block->blockSize < iterBlock->blockSize) {
            break;
        }
        prev = iterBlock;
        #ifdef SYS_BBP
        iterBlock = next;
        #else
        iterBlock=iterBlock->next;
        #endif
    }
    block->prev = prev;
    block->next = iterBlock;
    if(block->prev) {
        block->prev->next = block;
    } else {
        HeapWork->lastBlock = block;
    }
    if(block->next) {
        block->next->prev = block;
    }
    end:
    #ifdef SYS_BBP
    EXIT_CRITICAL_SECTION();
    #endif
}

static void MemRemoveBlock(HEAP_BLOCK *block)
{
    #ifdef SYS_BBP
    ENTER_CRITICAL_SECTION();
    #endif
    if(block == HeapWork->lastBlock) {
        HeapWork->lastBlock = block->next;
    }
    if(block->prev) {
        block->prev->next = block->next;
    }
    if(block->next) {
        block->next->prev = block->prev;
    }
    block->prev = NULL;
    block->next = NULL;
    #ifdef SYS_BBP
    EXIT_CRITICAL_SECTION();
    #endif
}


#ifndef SYS_BBP
static BOOL MemAddAllocList(u32 baseBlockNum, int blockNum, u32 serialNum, HEAP_BLOCK *block)
{
    u32 newBlockNum;
    newBlockNum = baseBlockNum;
    if(newBlockNum == blockNum) {
        return FALSE;
    }
    if(newBlockNum+1 == blockNum) {
        blockNum = HeapWork->allocBlockNum;
        if(newBlockNum < blockNum && serialNum == HeapWork->entry[0][newBlockNum].serialNum && blockNum != 0) {
            return FALSE;
        }
        blockNum = (blockNum-newBlockNum)-1;
        if(blockNum >= 1) {
            MI_CpuCopy32(&HeapWork->entry[0][newBlockNum], &HeapWork->entry[1][0], blockNum*sizeof(HEAP_ALLOC_ENTRY));
            MI_CpuCopy32(&HeapWork->entry[1][0], &HeapWork->entry[0][newBlockNum+1], (HeapWork->allocBlockNum-newBlockNum-1)*sizeof(HEAP_ALLOC_ENTRY));
            
        }
        HeapWork->entry[0][newBlockNum].serialNum = serialNum;
        HeapWork->entry[0][newBlockNum].blockPtr = block;
        return TRUE;
    }
    newBlockNum += blockNum;
    newBlockNum /= 2;
    if(((&HeapWork->entry[0][-1])+newBlockNum)->serialNum < serialNum) {
        return MemAddAllocList(newBlockNum, blockNum, serialNum, block);
    } else {
        return MemAddAllocList(baseBlockNum, newBlockNum, serialNum, block);
    }
}

static BOOL MemRemoveAllocList(u32 baseBlockNum, u32 blockNum, HEAP_BLOCK *block)
{
    u32 newBlockNum;
    
    u32 nextBlockNum;
    HEAP_WORK *work;
    
    newBlockNum = baseBlockNum;
    if(newBlockNum == blockNum) {
        return FALSE;
    }
    nextBlockNum = newBlockNum+1;
    if(nextBlockNum == blockNum) {
        if(block->serialNum != HeapWork->entry[0][newBlockNum].serialNum || HeapWork->entry[0][newBlockNum].blockPtr != block) {
            return FALSE;
        }
        HeapWork->allocBlockNum--;
        if(HeapWork->allocBlockNum-newBlockNum != 0) {
            MI_CpuCopy32(&HeapWork->entry[0][nextBlockNum], &HeapWork->entry[1][0], (HeapWork->allocBlockNum-newBlockNum)*sizeof(HEAP_ALLOC_ENTRY));
            MI_CpuCopy32(&HeapWork->entry[1][0], &HeapWork->entry[0][newBlockNum], (HeapWork->allocBlockNum-newBlockNum)*sizeof(HEAP_ALLOC_ENTRY));
        }
        return TRUE;
    }
    newBlockNum += blockNum;
    newBlockNum /= 2;
    if(((&HeapWork->entry[0][-1])+newBlockNum)->serialNum < block->serialNum) {
        return MemRemoveAllocList(newBlockNum, blockNum, block);
    } else {
        return MemRemoveAllocList(baseBlockNum, newBlockNum, block);
    }
}
#endif
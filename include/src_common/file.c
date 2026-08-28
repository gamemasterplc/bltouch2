#include "file.h"
#include "filetbl.h"

#include "memory.h"
#include "wireless.h"

#include <nitro.h>

#define SWAP32(value) ((((value) & 0xFF000000) >> 24)|(((value) & 0xFF0000) >> 8)|(((value) & 0xFF00) << 8)|(((value) & 0xFF) << 24))

static BLFile *FilePtr;

typedef struct {
    u32 magic;
    u32 rawSize : 22;
    u32 : 9;
    u32 compress : 1;
} BLDTHeader;


BLFile *BL_FileLoad(u32 fileID, BOOL temp)
{
    BLFile *file;
    FSFile fsFile;
    BLDTHeader header;
    u32 fileSize;
    BOOL compress;
    u32 rawSize;
    u32 compressSize;
    u32 allocSize;
    u8 *bufBase;
    BLFile **listHead;
    
    compress = FALSE;
    
    if(MB_IsMultiBootChild()) {
        file = BL_WLGetFilePtr();
    } else {
        file = FilePtr;
    }
    if(!temp) {
        while(file) {
            if(file->fileID == fileID) {
                file->refCnt++;
                return file;
            }
            file=file->next;
        }
    }
    
    FS_InitFile(&fsFile);
    if(!FS_OpenFile(&fsFile, BL_GetFilePath(fileID))) {
        OS_Terminate();
    }
    FS_ReadFile(&fsFile, &header, sizeof(BLDTHeader));
    if(SWAP32(header.magic) != 'BLDT') {
        fileSize = FS_GetLength(&fsFile);
        FS_SeekFile(&fsFile, 0, FS_SEEK_SET);
    } else {
        if(header.compress) {
            compressSize = FS_GetLength(&fsFile)-sizeof(BLDTHeader);
            rawSize = header.rawSize;
            fileSize = rawSize+compressSize;
            compress = TRUE;
        } else {
            fileSize = FS_GetLength(&fsFile)-sizeof(BLDTHeader);
        }
        FS_SeekFile(&fsFile, sizeof(BLDTHeader), FS_SEEK_SET);
    }
    
    allocSize = fileSize+sizeof(BLFile);
    if(temp) {
        file = BL_MemAlloc(BL_MEM_TAG_FILE_TEMP, allocSize, 0);
    } else {
        file = BL_MemAlloc(BL_MEM_TAG_FILE, allocSize, 0);
    }
    file->fileID = fileID;
    file->refCnt = 1;
    listHead = &FilePtr;
    if(!compress) {
        file->size = fileSize;
    } else {
        file->size = rawSize;
    }
    
    file->prev = NULL;
    
    if(!temp) {
        file->next = *listHead;
        if(file->next) {
            file->next->prev = file;
        }
        *listHead = file;
    } else {
        file->next = NULL;
    }
    bufBase = &file->data[0];
    if(!compress) {
        FS_ReadFile(&fsFile, bufBase, fileSize+sizeof(BLFile));
    } else {
        FS_ReadFile(&fsFile, &bufBase[rawSize], compressSize);
        MI_UncompressLZ8(&bufBase[rawSize],  &bufBase[0]);
        BL_MemShrink(file, compressSize);
    }
    FS_CloseFile(&fsFile);
    return file;
}

void BL_FileUnload(BLFile *file)
{
    if(!file) {
        return;
    }
    file->refCnt--;
    if(file->refCnt != 0) {
        return;
    }
    if(file->prev) {
        file->prev->next = file->next;
    } else {
        FilePtr = file->next;
    }
    if(file->next) {
        file->next->prev = file->prev;
    }
    BL_MemFree(file);
}

void BL_FileInit(void)
{
    FilePtr = NULL;
}

void BL_FileClose(void)
{
    FilePtr = NULL;
    BL_MemFreeTag(BL_MEM_TAG_FILE);
}
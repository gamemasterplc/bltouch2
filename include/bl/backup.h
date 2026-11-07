#ifndef BL_BACKUP_H
#define BL_BACKUP_H

#include "map.h"

extern void BL_BackupLandWrite(int slot, MapBackup *backup);
extern void BL_BackupLandRead(int slot, MapBackup *backup);

extern void BL_BackupWrite(void);

BOOL BL_BackupIsLoadFail(void);
BOOL BL_BackupIsSaveFail(void);
BOOL BL_BackupIsDirty(void);

BOOL BL_BackupIsCorrupt(void);
void BL_BackupForceErase(void);
BOOL BL_BackupIsClean(void);
void BL_BackupSetClean(void);


#endif
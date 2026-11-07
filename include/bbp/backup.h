#ifndef BBP_BACKUP_H
#define BBP_BACKUP_H


#include <nitro/types.h>

#define BL_BackupIsLoadFail BBP_BackupIsLoadFail
#define BL_BackupIsSaveFail BBP_BackupIsSaveFail

#define BL_BackupIsCorrupt BBP_BackupIsCorrupt
#define BL_BackupForceErase BBP_BackupForceErase
#define BL_BackupIsClean BBP_BackupIsClean
#define BL_BackupSetClean BBP_BackupSetClean

BOOL BL_BackupIsLoadFail(void);
BOOL BL_BackupIsSaveFail(void);

BOOL BL_BackupIsCorrupt(void);
void BL_BackupForceErase(void);
BOOL BL_BackupIsClean(void);
void BL_BackupSetClean(void);

BOOL BBP_BackupIsNeedDWCWiFiId(void);
void BBP_BackupClearNeedDWCWiFiId(void);

#endif
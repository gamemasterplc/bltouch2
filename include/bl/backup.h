#ifndef BL_BACKUP_H
#define BL_BACKUP_H

#include "map.h"

extern void BL_BackupLandWrite(int slot, MapBackup *backup);
extern void BL_BackupLandRead(int slot, MapBackup *backup);

extern void BL_BackupWrite(void);

#endif
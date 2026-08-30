#ifndef RES_H
#define RES_H

#include "file.h"
#include "display.h"

#ifdef SYS_BBP
#define BL_ResGetAnimVRamSize BBP_ResGetAnimVRamSize
#define BL_ResGetAnimMaxVRamSize BBP_ResGetAnimMaxVRamSize
#define BL_ResLoadBackground BBP_ResLoadBackground
#define BL_ResLoadBackgroundAffine BBP_ResLoadBackgroundAffine
#define BL_ResLoadImage BBP_ResLoadImage
#endif

#define BL_ResLoadBGImage(file, layer, ofs) BL_ResLoadImage(file, (layer)+0, ofs)
#define BL_ResLoadOBJImage(file, ofs) BL_ResLoadImage(file, 4, ofs)
#define BL_ResLoadSubBGImage(file, layer, ofs) BL_ResLoadImage(file, (layer)+5, ofs)
#define BL_ResLoadSubOBJImage(file, ofs) BL_ResLoadImage(file, 9, ofs)

u32 BL_ResGetAnimVRamSize(void *res, int animNo);
u32 BL_ResGetAnimMaxVRamSize(void *res);
void BL_ResLoadBackground(BLFile *file, int display, int scrBase, int tileOfs, u8 palOfs);
void BL_ResLoadBackgroundAffine(BLFile *file, int display, int scrBase, int tileOfs, u8 palOfs);
void BL_ResLoadImage(BLFile *file, int type, u32 ofs);
#ifndef SYS_BBP
void BL_ResLoadImage3D(BLFile *file, u32 ofs);
#endif

#endif
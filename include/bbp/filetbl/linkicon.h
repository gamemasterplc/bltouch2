#ifndef FILETBL_LINKICON_H
#define FILETBL_LINKICON_H

#include "filetbl.h"

enum {
    BBP_LINKICON_FILE_NULL = BL_FILEID(BBP_DIRID_LINKICON, 0)-1,
    #define BLFILE_ENTRY(path, id) id,
    #include "bbp/filetbl/linkicon.inc"
    #undef BLFILE_ENTRY
};

#endif
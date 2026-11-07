

char *BBP_LinkIcon_FileTbl[] = {
    #define BLFILE_ENTRY(path, id) path,
    #include "bbp/filetbl/linkicon.inc"
    #undef BLFILE_ENTRY
};

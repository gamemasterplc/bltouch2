#ifndef HDMA_H
#define HDMA_H


#ifdef SYS_BBP
#define BL_HDmaInit BBP_HDmaInit
#endif

void BL_HDmaInit(void);

#endif
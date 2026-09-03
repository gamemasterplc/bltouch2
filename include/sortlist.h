#ifndef BL_SORTLIST_H
#define BL_SORTLIST_H

#include <nitro/types.h>

#define BL_SORTLIST_CMP_GREATER 0
#define BL_SORTLIST_CMP_LESS 1
#define BL_SORTLIST_CMP_EQUAL 2

typedef struct BLSortListNode_s BLSortListNode;

typedef void (*BLSortListFunc)(BLSortListNode *node);

typedef struct BLSortList_s {
    u8 cmp;
    u8 preserveOrder;
    BLSortListNode *head;
    BLSortListNode *tail;
    BLSortListNode *unk1;
    BLSortListNode *unk2;
} BLSortList;

struct BLSortListNode_s {
    int order;
    BLSortListNode *prev;
    BLSortListNode *next;
};

#ifdef SYS_BBP
#define BL_SortListCreate BBP_SortListCreate
#define BL_SortListNodeInsert BBP_SortListNodeInsert
#define BL_SortListNodeSetOrder BBP_SortListNodeSetOrder
#define BL_SortListNodeRemove BBP_SortListNodeRemove
#define BL_SortListForEach BBP_SortListForEach
#endif

void BL_SortListCreate(BLSortList *list, u8 cmp, u8 preserveOrder);
void BL_SortListNodeInsert(BLSortList *list, BLSortListNode *node, int order);
void BL_SortListNodeSetOrder(BLSortList *list, BLSortListNode *node, int order);
void BL_SortListNodeRemove(BLSortList *list, BLSortListNode *node);
void BL_SortListForEach(BLSortList *list, BLSortListFunc func);

#endif
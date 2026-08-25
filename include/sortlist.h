#ifndef SORTLIST_H
#define SORTLIST_H

#include <nitro/types.h>

#define SORT_LIST_CMP_GREATER 0
#define SORT_LIST_CMP_LESS 1
#define SORT_LIST_CMP_EQUAL 2

typedef struct SortListNode_s SORTLIST_NODE;

typedef void (*SORTLIST_FUNC)(SORTLIST_NODE *node);

typedef struct SortList_s {
    u8 cmp;
    u8 preserveOrder;
    SORTLIST_NODE *head;
    SORTLIST_NODE *tail;
    SORTLIST_NODE *unk;
} SORTLIST;

struct SortListNode_s {
    int order;
    SORTLIST_NODE *prev;
    SORTLIST_NODE *next;
};

#ifndef SYS_BBP
#define SortListCreate BL_SortListCreate
#define SortListNodeInsert BL_SortListNodeInsert
#define SortListNodeSetOrder BL_SortListNodeSetOrder
#define SortListNodeRemove BL_SortListNodeRemove
#define SortListForEach BL_SortListForEach
#else
#define SortListCreate BBP_SortListCreate
#define SortListNodeInsert BBP_SortListNodeInsert
#define SortListNodeSetOrder BBP_SortListNodeSetOrder
#define SortListNodeRemove BBP_SortListNodeRemove
#define SortListForEach BBP_SortListForEach
#endif

void SortListCreate(SORTLIST *list, u8 cmp, u8 preserveOrder);
void SortListNodeInsert(SORTLIST *list, SORTLIST_NODE *node, int order);
void SortListNodeSetOrder(SORTLIST *list, SORTLIST_NODE *node, int order);
void SortListNodeRemove(SORTLIST *list, SORTLIST_NODE *node);
void SortListForEach(SORTLIST *list, SORTLIST_FUNC func);

#endif
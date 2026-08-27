#include "sortlist.h"

static void InsertNode(BLSortList *list, BLSortListNode *node);
static void RemoveNode(BLSortList *list, BLSortListNode *node);

void BL_SortListCreate(BLSortList *list, u8 cmp, u8 preserveOrder)
{
    list->cmp = cmp;
    list->preserveOrder = preserveOrder;
    list->head = list->tail = list->unk = NULL;
}

void BL_SortListNodeInsert(BLSortList *list, BLSortListNode *node, int order)
{
    node->order = order;
    InsertNode(list, node);
}

void BL_SortListNodeSetOrder(BLSortList *list, BLSortListNode *node, int order)
{
    node->order = order;
    RemoveNode(list, node);
    InsertNode(list, node);
}

void BL_SortListNodeRemove(BLSortList *list, BLSortListNode *node)
{
    RemoveNode(list, node);
}

void BL_SortListForEach(BLSortList *list, BLSortListFunc func)
{
    BLSortListNode *node = list->head;
    while(node) {
        BLSortListNode *next = node->next;
        func(node);
        node = next;
    }
}

static void InsertNode(BLSortList *list, BLSortListNode *node)
{
    BLSortListNode *prev;
    BLSortListNode *temp;
    BLSortListNode *cur;
    node->prev = node->next = NULL;
    prev = NULL;
    #ifndef SYS_BBP
    for(cur=list->head; cur; prev=cur, cur=cur->next) {
        if(list->cmp == BL_SORTLIST_CMP_GREATER && node->order > cur->order) {
            continue;
        }
        if(list->cmp == BL_SORTLIST_CMP_LESS && node->order < cur->order) {
            continue;
        }
        if(node->order != cur->order && list->cmp != BL_SORTLIST_CMP_EQUAL) {
            break;
        }
        if(list->preserveOrder) {
            break;
        }
    }
    #else
    for(cur=list->head; cur; prev=cur, cur=temp) {
        temp = cur->next;
        if(list->cmp == BL_SORTLIST_CMP_GREATER && node->order > cur->order) {
            continue;
        }
        if(list->cmp == BL_SORTLIST_CMP_LESS && node->order < cur->order) {
            continue;
        }
        if(node->order != cur->order && list->cmp != BL_SORTLIST_CMP_EQUAL) {
            break;
        }
        if(list->preserveOrder) {
            break;
        }
    }
    #endif
    node->prev = prev;
    node->next = cur;
    if(node->prev) {
        node->prev->next = node;
    } else {
        list->head = node;
    }
    if(node->next) {
        node->next->prev = node;
    } else {
        list->tail = node;
    }
}

static void RemoveNode(BLSortList *list, BLSortListNode *node)
{
    if(node->prev) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if(node->next) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    node->prev = node->next = NULL;
}
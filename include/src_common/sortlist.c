#include "sortlist.h"

static void InsertNode(SORTLIST *list, SORTLIST_NODE *node);
static void RemoveNode(SORTLIST *list, SORTLIST_NODE *node);

void SortListCreate(SORTLIST *list, u8 cmp, u8 preserveOrder)
{
    list->cmp = cmp;
    list->preserveOrder = preserveOrder;
    list->head = list->tail = list->unk = NULL;
}

void SortListNodeInsert(SORTLIST *list, SORTLIST_NODE *node, int order)
{
    node->order = order;
    InsertNode(list, node);
}

void SortListNodeSetOrder(SORTLIST *list, SORTLIST_NODE *node, int order)
{
    node->order = order;
    RemoveNode(list, node);
    InsertNode(list, node);
}

void SortListNodeRemove(SORTLIST *list, SORTLIST_NODE *node)
{
    RemoveNode(list, node);
}

void SortListForEach(SORTLIST *list, SORTLIST_FUNC func)
{
    SORTLIST_NODE *node = list->head;
    while(node) {
        SORTLIST_NODE *next = node->next;
        func(node);
        node = next;
    }
}

static void InsertNode(SORTLIST *list, SORTLIST_NODE *node)
{
    SORTLIST_NODE *prev;
    SORTLIST_NODE *temp;
    SORTLIST_NODE *cur;
    node->prev = node->next = NULL;
    prev = NULL;
    #ifndef SYS_BBP
    for(cur=list->head; cur; prev=cur, cur=cur->next) {
        if(list->cmp == SORT_LIST_CMP_GREATER && node->order > cur->order) {
            continue;
        }
        if(list->cmp == SORT_LIST_CMP_LESS && node->order < cur->order) {
            continue;
        }
        if(node->order != cur->order && list->cmp != SORT_LIST_CMP_EQUAL) {
            break;
        }
        if(list->preserveOrder) {
            break;
        }
    }
    #else
    for(cur=list->head; cur; prev=cur, cur=temp) {
        temp = cur->next;
        if(list->cmp == SORT_LIST_CMP_GREATER && node->order > cur->order) {
            continue;
        }
        if(list->cmp == SORT_LIST_CMP_LESS && node->order < cur->order) {
            continue;
        }
        if(node->order != cur->order && list->cmp != SORT_LIST_CMP_EQUAL) {
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

static void RemoveNode(SORTLIST *list, SORTLIST_NODE *node)
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
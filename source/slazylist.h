#ifndef _SLAZYLIST_H_
#define _SLAZYLIST_H_

#include <stddef.h>

typedef struct _slazynode_
{
    void *data;
    struct _slazynode_ *next;
} SLazyNode;

typedef struct _slazylist_
{
    size_t length;
    struct _slazynode_ *head;
    struct _slazynode_ *tail;
    void (*destroy)(void *data);
} SLazyList;

void slazylist_set_alloc(void *(*alloc)(size_t bytes));

void slazylist_set_dealloc(void (*dealloc)(void *pointer));

SLazyNode *slazylist_get(SLazyList *list, size_t index);

int slazylist_create(void (*destroy)(void *data), SLazyList **list_out);

int slazylist_insert(void *data, SLazyList *list, SLazyNode **out_node);

SLazyNode *slazylist_is_inside(void *data, SLazyList *list, int (*inside)(void *a, void *b));

int slazylist_remove_node(SLazyList *list, SLazyNode **node);

int slazylist_remove_index(SLazyList *list, size_t index);

int slazylist_remove_node_n(SLazyList *list, SLazyNode **node, void **out_data);

int slazylist_remove_index_n(SLazyList *list, size_t index, void **out_data);

void slazylist_destroy(SLazyList **list);

#endif
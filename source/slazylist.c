#include "slazylist.h"
#include "slazylist_codes.h"
#include <stdlib.h>
#include <string.h>

void *(*slazy_alloc)(size_t bytes) = malloc;

void (*slazy_dealloc)(void *pointer) = free;

void *remove_node(SLazyList *list, SLazyNode *node, SLazyNode *before, int call_destroy)
{
    // If the list length is 1, the head and the tail are the same pointer.
    // By checking, separatelty, if the node to delete is the same as the
    // head or the tail, we ensure the list head and tail are equals to
    // NULL in case of list's length equals to 0

    // If the node to delete is the head of the list
    if (node == list->head)
    {
        list->head = node->next;
    }

    // If the node to delete is the tail of the list
    if (node == list->tail)
    {
        list->tail = before;

        if (before)
            before->next = NULL;
    }

    // This only applies if the node to delete is between two node of the list
    if (node->next && before)
        before->next = node->next;

    void *data = node->data;

    slazy_dealloc(node);
    list->length--;

    if (call_destroy)
    {
        if (list->destroy)
            list->destroy(data);

        return NULL;
    }

    return data;
}

void slazylist_set_alloc(void *(*alloc)(size_t bytes))
{
    if (!alloc)
    {
        slazy_alloc = malloc;
        return;
    }

    slazy_alloc = alloc;
}

void slazylist_set_dealloc(void (*dealloc)(void *pointer))
{
    if (!dealloc)
    {
        slazy_dealloc = free;
        return;
    }

    slazy_dealloc = dealloc;
}

SLazyNode *slazylist_get(SLazyList *list, size_t index)
{
    if (index >= list->length)
        return NULL;

    size_t counter = 0;
    SLazyNode *current = list->head;

    while (current)
    {
        if (counter == index)
            return current;

        SLazyNode *next = current->next;
        current = next;
        counter++;
    }

    return NULL;
}

int slazylist_create(void (*destroy)(void *data), SLazyList **list_out)
{
    SLazyList *list = slazy_alloc(sizeof(SLazyList));

    if (!list)
        return SLAZYLIST_FAILED_ALLOC;

    list->length = 0;
    list->head = NULL;
    list->tail = NULL;
    list->destroy = destroy;

    *list_out = list;

    return SLAZYLIST_OK;
}

int slazylist_insert(void *data, SLazyList *list, SLazyNode **out_node)
{
    SLazyNode *node = slazy_alloc(sizeof(SLazyNode));

    if (!node)
        return SLAZYLIST_FAILED_ALLOC;

    node->data = data;
    node->next = NULL;

    if (list->length == 0)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }

    list->length++;

    if (out_node)
        *out_node = node;

    return SLAZYLIST_OK;
}

SLazyNode *slazylist_is_inside(void *data, SLazyList *list, int (*inside)(void *a, void *b))
{
    SLazyNode *current = list->head;

    while (current)
    {
        if (inside(data, current->data))
            return current;

        SLazyNode *next = current->next;
        current = next;
    }

    return NULL;
}

int slazylist_remove_node(SLazyList *list, SLazyNode **node)
{
    if (list->length == 0)
        return SLAZYLIST_IS_EMPTY;

    char found = 0;
    SLazyNode *before = NULL;
    SLazyNode *current = list->head;

    while (current)
    {
        if (current == *node)
        {
            found = 1;
            break;
        }

        SLazyNode *next = current->next;
        before = current;
        current = next;
    }

    if (!found)
        return SLAZYLIST_INVALID_NODE;

    remove_node(list, *node, before, 1);
    *node = NULL;

    return SLAZYLIST_OK;
}

int slazylist_remove_index(SLazyList *list, size_t index)
{
    if (list->length == 0)
        return SLAZYLIST_IS_EMPTY;

    if (index >= list->length)
        return SLAZYLIST_INDEX_OUT_OF_BOUNDS;

    SLazyNode *before = NULL;
    SLazyNode *current = list->head;

    for (size_t i = 0; i < list->length; i++)
    {
        if (index == i)
            break;

        SLazyNode *next = current->next;
        before = current;
        current = next;
    }

    remove_node(list, current, before, 1);

    return SLAZYLIST_OK;
}

int slazylist_remove_node_n(SLazyList *list, SLazyNode **node, void **out_data)
{
    if (list->length == 0)
        return SLAZYLIST_IS_EMPTY;

    char found = 0;
    SLazyNode *before = NULL;
    SLazyNode *current = list->head;

    while (current)
    {
        if (current == *node)
        {
            found = 1;
            break;
        }

        SLazyNode *next = current->next;
        before = current;
        current = next;
    }

    if (!found)
        return SLAZYLIST_INVALID_NODE;

    *node = NULL;
    *out_data = remove_node(list, *node, before, 0);

    return SLAZYLIST_OK;
}

int slazylist_remove_index_n(SLazyList *list, size_t index, void **out_data)
{
    if (list->length == 0)
        return SLAZYLIST_IS_EMPTY;

    if (index >= list->length)
        return SLAZYLIST_INDEX_OUT_OF_BOUNDS;

    SLazyNode *before = NULL;
    SLazyNode *current = list->head;

    for (size_t i = 0; i < list->length; i++)
    {
        if (index == i)
            break;

        SLazyNode *next = current->next;
        before = current;
        current = next;
    }

    *out_data = remove_node(list, current, before, 0);

    return SLAZYLIST_OK;
}

void slazylist_destroy(SLazyList **list)
{
    if (!list || !*list)
        return;

    SLazyNode *current = (*list)->head;

    while (current)
    {
        SLazyNode *next = current->next;

        if ((*list)->destroy)
            (*list)->destroy(current->data);

        slazy_dealloc(current);
        current = next;
    }

    slazy_dealloc(*list);
    *list = NULL;
}
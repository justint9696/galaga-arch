#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct Node_s {
    void *item;
    struct Node_s *next;
} Node;

typedef struct {
    size_t size;
    Node *head, *tail;
} LinkedList;

#define list_loop(_list)\
    __typeof__(_list) list = (_list);\
    for (Node *tmp = list->head; tmp != NULL; tmp = tmp->next)

LinkedList *list_init();
void list_clear(LinkedList *);

void list_add(LinkedList *, void *item);
void list_remove(LinkedList *, void *item);
void list_join(LinkedList *dst, LinkedList *src);

bool list_is_empty(const LinkedList *);

#endif

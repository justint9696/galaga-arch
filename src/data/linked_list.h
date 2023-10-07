#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct Node_s {
    void *item;
    struct Node_s *next;
} Node;

typedef struct {
    Node *head, *tail;
} LinkedList;

LinkedList *LinkedList_Init();
void LinkedList_Add(LinkedList *, void *item);
void LinkedList_Remove(LinkedList *, void *item);
void LinkedList_Free(LinkedList *);

#endif

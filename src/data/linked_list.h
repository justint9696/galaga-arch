#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct Node_s {
    void *item;
    struct Node_s *next;
} Node;

typedef struct {
    Node *head, *tail;
} LinkedList;

/*
typedef struct LinkedList_s {
    void *item;
    struct LinkedList_s *next;
} LinkedList;
*/

LinkedList *LinkedList_Init();
void LinkedList_Add(LinkedList *list, void *item);
void LinkedList_Remove(LinkedList *list, void *item);

#endif

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct LinkedList_s {
    void *item;
    struct LinkedList_s *next;
} LinkedList;

LinkedList *LinkedList_Init();
void LinkedList_Add(LinkedList *head, void *item);
void LinkedList_Remove(LinkedList **head, void *item);

#endif

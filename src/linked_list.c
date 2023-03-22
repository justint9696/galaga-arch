#include "include/linked_list.h"

void LinkedList_Add(LinkedList *head, void *item) {
    LinkedList *tmp = head;
    tmp->item = item;
    if (!tmp->item)
        head->item = item;
    else {
        while (!tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = item;
    }
}

void LinkedList_Remove(LinkedList *head, void *item) {
    LinkedList *tmp = head;
    tmp->item = item;
    if (tmp->item == item) 
        head = head->next;
    else {
        while (tmp->next) {
            if (tmp->next->item == item) {
                tmp->next = tmp->next->next;
                break;
            }

            tmp = tmp->next;
        }
    }
}
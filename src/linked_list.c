#include "include/linked_list.h"

void LinkedList_Add(LinkedList *head, void *item) {
    if (!head->item)
        head->item = item;
    else {
        LinkedList *tmp = head;
        while (!tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = item;
    }
}

void LinkedList_Remove(LinkedList *head, void *item) {
    if (head->item == item) 
        head = head->next;
    else {
        LinkedList *tmp = head;
        while (tmp->next) {
            if (tmp->next->item == item) {
                tmp->next = tmp->next->next;
                break;
            }

            tmp = tmp->next;
        }
    }
}
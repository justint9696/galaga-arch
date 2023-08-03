#include "linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

inline LinkedList *LinkedList_Init() {
    return (LinkedList *)calloc(1, sizeof(LinkedList));
}

void LinkedList_Add(LinkedList *head, void *item) {
    if (!head->item)
        head->item = item;
    else {
        LinkedList *tmp = head;
        while (tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = LinkedList_Init();
        tmp->next->item = item;
    }
}

void LinkedList_Remove(LinkedList **head, void *item) {
    LinkedList *tmp = *head, *prev = NULL;
    while (tmp) {
        if (!memcmp(tmp->item, item, sizeof(void *))) {
            if (prev)
                memcpy(&prev->next, &tmp->next, sizeof(LinkedList *));
            else if (tmp->next)
                memcpy(head, &tmp->next, sizeof(LinkedList *));
            else
                memset(*head, 0, sizeof(LinkedList));

            if (memcmp(head, &tmp, sizeof(LinkedList *)))
                free(tmp);

            break;
        }

        prev = tmp;
        tmp = tmp->next;
    }
}

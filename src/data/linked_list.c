#include "linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static inline Node *_node_init() {
    return (Node *)calloc(1, sizeof(Node));
}

inline LinkedList *LinkedList_Init() {
    return (LinkedList *)calloc(1, sizeof(LinkedList));
}

void LinkedList_Add(LinkedList *list, void *item) {
    if (!list->head) {
        list->head = _node_init();
        list->head->item = item;
        memcpy(&list->tail, &list->head, sizeof(Node *));
    } else {
        Node *tmp = list->head;
        while (tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = _node_init();
        memcpy(&tmp->next->item, &item, sizeof(void *));
        memcpy(&list->tail, &tmp->next, sizeof(Node *));
    }
}

void LinkedList_Remove(LinkedList *list, void *item) {
    Node *tmp = list->head, *prev = NULL;
    while (tmp) {
        if (!memcmp(tmp->item, item, sizeof(void *))) {
            if (prev)
                memcpy(&prev->next, &tmp->next, sizeof(Node *));
            else if (tmp->next)
                memcpy(list->head->item, &tmp->next, sizeof(Node *));
            else
                memset(list, 0, sizeof(LinkedList));

            if (!memcmp(&list->tail, &tmp, sizeof(Node *)))
                memcpy(&list->tail, &prev, sizeof(Node *));

            if (memcmp(&list->head, &tmp, sizeof(Node *)))
                free(tmp);

            break;
        }

        prev = tmp;
        tmp = tmp->next;
    }
}


#include "data/linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static inline Node *_node_init() {
    return calloc(1, sizeof(Node));
}

inline LinkedList *LinkedList_Init() {
    return calloc(1, sizeof(LinkedList));
}

void LinkedList_Add(LinkedList *self, void *item) {
    if (!self->head) {
        self->head = _node_init();
        self->head->item = item;
        memcpy(&self->tail, &self->head, sizeof(void *));
    } else {
        Node *tmp = self->head;
        while (tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = _node_init();
        memcpy(&tmp->next->item, &item, sizeof(void *));
        memcpy(&self->tail, &tmp->next, sizeof(void *));
    }
}

void LinkedList_Remove(LinkedList *self, void *item) {
    Node *tmp = self->head, *prev = NULL;
    while (tmp) {
        if (!memcmp(tmp->item, item, sizeof(void *))) {
            if (prev)
                memcpy(&prev->next, &tmp->next, sizeof(void *));
            else if (tmp->next)
                memcpy(self->head->item, &tmp->next, sizeof(void *));
            else
                memset(self, 0, sizeof(LinkedList));

            if (!memcmp(&self->tail, &tmp, sizeof(void *)))
                memcpy(&self->tail, &prev, sizeof(void *));

            if (memcmp(&self->head, &tmp, sizeof(void *)))
                free(tmp);

            break;
        }

        prev = tmp;
        tmp = tmp->next;
    }
}

void LinkedList_Free(LinkedList *self) {
    assert(self);

    Node *tmp = self->head, *current;
    while (tmp) {
        current = tmp;
        tmp = tmp->next;
        free(current);
    }

    memset(self, 0, sizeof(LinkedList));
}


#include "data/linked_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static inline Node *node_init(void *item) {
    Node *node = calloc(1, sizeof(Node));
    node->item = item;
    return node;
}

inline LinkedList *list_init() {
    LinkedList *lst = calloc(1, sizeof(LinkedList));
    return lst;
}

void list_add(LinkedList *self, void *item) {
    if (!self->head) {
        self->head = node_init(item);
        self->tail = self->head;
    } else {
        Node *tmp = self->head;
        while (tmp->next) 
            tmp = tmp->next;

        tmp->next = node_init(item);
        self->tail = tmp->next;
    }

    self->size++;
}

void list_remove(LinkedList *self, void *item) {
    Node *prev = NULL;
    for (Node *tmp = self->head; tmp != NULL; ) {
        if (tmp->item == item) {
            if (prev) 
                prev->next = tmp->next;
            else if (tmp->next)
                self->head = tmp->next;
            else
                self->head = NULL;

            if (self->tail == tmp) {
                if (prev)
                    self->tail = prev;
                else
                    self->tail = self->head;
            }

            if (self->head != tmp)
                free(tmp);

            self->size--;
            break;
        }

        prev = tmp;
        tmp = tmp->next;
    }
}

void list_clear(LinkedList *self) {
    assert(self);

    Node *current;
    for (Node *tmp = self->head; tmp != NULL; ) {
        current = tmp;
        tmp = tmp->next;
        free(current);
    }

    memset(self, 0, sizeof(LinkedList));
}

void list_join(LinkedList *dst, LinkedList *src) {
    for (Node *tmp = src->head; tmp != NULL; tmp = tmp->next) {
        list_add(dst, tmp->item);
    }
}


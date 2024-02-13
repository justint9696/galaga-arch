#include "data/queue.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Queue *queue_init() {
    Queue *q = calloc(1, sizeof(Queue));
    assert(q);
    return q;
}

void queue_clear(Queue *q) {
    size_t size = q->size;
    for (size_t i = 0; i < size; i++)
        dequeue(q);

    memset(q, 0, sizeof(Queue));
}

void queue_free(Queue *q) {
    void *item;
    size_t size = q->size;
    for (size_t i = 0; i < size; i++) {
        item = queue_pop(q);
        assert(item);
        free(item);
    }

    memset(q, 0, sizeof(Queue));
}

void *queue_front(const Queue *q) {
    return q->queue[q->index];
}

void *queue_rear(const Queue *q) {
    return q->queue[(q->index + q->size - 1) % QUEUE_MAX];
}

void *queue_pop(Queue *q) {
    assert(q->size);
    void *item = queue_front(q);
    dequeue(q);
    return item;
}

void enqueue(Queue *q, void *item) {
    assert(q->size < QUEUE_MAX);
    q->queue[(q->index + q->size++) % QUEUE_MAX] = item;
}

void dequeue(Queue *q) {
    assert(q->size > 0);
    q->index = (++q->index % QUEUE_MAX);
    --q->size;
}

bool queue_is_empty(const Queue *q) {
    return !q->size;
}

bool queue_contains(const Queue *q, void *item) {
    size_t index, size = q->size;
    for (size_t i = 0; i < size; i++) {
        index = (q->index + i) % QUEUE_MAX;
        if (q->queue[index] == item)
            return true;
    }

    return false;
}

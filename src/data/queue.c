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

void *queue_front(const Queue *q) {
    return q->queue[q->index];
}

void *queue_rear(const Queue *q) {
    return q->queue[(q->index + q->size - 1) % QUEUE_MAX];
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

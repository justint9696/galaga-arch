#include "data/queue.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Queue *queue_init() {
    return calloc(1, sizeof(Queue));
}

void queue_free(Queue *q) {
    size_t size = q->size;
    for (int i = 0; i < size; i++)
        dequeue(q);

    memset(q, 0, sizeof(Queue));
}

void *queue_front(const Queue *q) {
    return q->queue[q->index];
}

void *queue_rear(const Queue *q) {
    return q->queue[(q->index + q->size - 1) % MAX_QUEUE];
}

void enqueue(Queue *q, void *item) {
    assert(q->size < MAX_QUEUE);
    q->queue[(q->index + q->size++) % MAX_QUEUE] = item;
}

void dequeue(Queue *q) {
    assert(q->size > 0);
    q->index = (++q->index % MAX_QUEUE);
    --q->size;
}


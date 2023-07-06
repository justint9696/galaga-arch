#include "inc/queue.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

Queue *queue_init() {
    return (Queue *)calloc(1, sizeof(Queue));
}

void queue_free(Queue *q) {
    for (int i = q->index; i < q->size; i++) 
        free(q->queue[i]);

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
    free(queue_front(q));
    q->index = (++q->index % MAX_QUEUE);
    --q->size;
}


#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stddef.h>

#define MAX_QUEUE       8

typedef struct {
    int index;
    size_t size;
    void *queue[MAX_QUEUE];
} Queue;

Queue *queue_init();
void queue_free(Queue *);

void *queue_front(const Queue *);
void *queue_rear(const Queue *);

void enqueue(Queue *, void *);
void dequeue(Queue *);

#endif

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define QUEUE_MAX 32

typedef struct {
    uint32_t index;
    size_t size;
    void *queue[QUEUE_MAX];
} Queue;

Queue *queue_init();
void queue_clear(Queue *);

void *queue_front(const Queue *);
void *queue_rear(const Queue *);
void *queue_pop(Queue *);

void enqueue(Queue *, void *);
void dequeue(Queue *);

bool queue_is_empty(const Queue *);

#endif

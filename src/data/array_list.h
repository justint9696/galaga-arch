#ifndef _DATA_ARRAY_LIST_H_
#define _DATA_ARRAY_LIST_H_

#include <stddef.h>
#include <stdint.h>

typedef enum {
    S_USED = 1 << 0,
} slot_t;

typedef struct {
    size_t size, capacity, item_size;
    uint8_t *buffer;
} ArrayList, List;

#define array_list_init(_list, _s, _size) (_array_list_init(_list, sizeof(_s), _size))

void _array_list_init(List *, size_t item_size, size_t capacity);
void array_list_destroy(List *);

void array_list_resize(List *, size_t);
void array_list_clear(List *);

// returns the index of the appended item
uint32_t array_list_append(List *, void *);
void array_list_remove(List *, uint32_t);

void *array_list_get(List *, uint32_t);

#endif

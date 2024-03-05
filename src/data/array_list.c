#include "data/array_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define get_offset(_index, _size) (_index * _size)

void _array_list_init(List *self, size_t item_size, size_t capacity) {
    memset(self, 0, sizeof(List));
    self->capacity = capacity;
    self->item_size = item_size;
    self->buffer = calloc(capacity, item_size);
    assert(self->buffer);
}

void array_list_destroy(List *self) {
    assert(self->buffer);
    free(self->buffer);
    memset(self, 0, sizeof(List));
}

void array_list_resize(List *self, size_t capacity) {
    data_t *copy = calloc(capacity, self->item_size);
    assert(copy);

    memcpy(copy, self->buffer, self->item_size * self->capacity);
    free(self->buffer);

    self->buffer = copy;
    self->capacity = capacity;
}

void array_list_clear(List *self) {
    memset(self->buffer, 0, self->item_size * self->capacity);
    self->size = 0;
}

uint32_t array_list_append(List *self, void *item) {
    assert(self->size++ < self->capacity);
    size_t i;
    data_t *chunk;
    for (i = 0; i < self->capacity; i++) {
        chunk = (self->buffer + get_offset(i, self->item_size));
        if (!*chunk) break;
    }

    memcpy(chunk, item, self->item_size);
    return i;
}

void array_list_remove(List *self, uint32_t index) {
    assert(index < self->capacity);
    void *chunk = (self->buffer + get_offset(index, self->item_size)); 
    memset(chunk, 0, sizeof(self->item_size));
}

void *array_list_get(List *self, uint32_t index) {
    assert(index < self->capacity);
    void *chunk = (self->buffer + get_offset(index, self->item_size)); 
    return chunk;
}


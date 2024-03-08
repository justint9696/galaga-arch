#include "data/array_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define get_offset(_index, _size) (_index * (sizeof(uint8_t) + _size))

void _array_list_init(List *self, size_t item_size, size_t capacity) {
    memset(self, 0, sizeof(List));
    self->capacity = capacity;
    self->item_size = item_size;
    self->buffer = calloc(1, capacity * (sizeof(uint8_t) + item_size));
    assert(self->buffer);
}

void array_list_destroy(List *self) {
    assert(self->buffer);
    free(self->buffer);
    memset(self, 0, sizeof(List));
}

void array_list_resize(List *self, size_t capacity) {
    void *copy = realloc(self->buffer, capacity * (self->item_size + sizeof(uint8_t)));
    assert(copy);
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
    uint8_t *chunk;
    for (i = 0; i < self->capacity; i++) {
        chunk = (uint8_t *)(self->buffer + get_offset(i, self->item_size));
        if (!(*chunk & S_USED)) {
            break;
        }
    }

    *chunk |= S_USED;
    memcpy((sizeof(uint8_t) + chunk), item, self->item_size);
    return i;
}

void array_list_remove(List *self, uint32_t index) {
    assert(index < self->capacity);
    uint8_t *chunk = (uint8_t *)(self->buffer + get_offset(index, self->item_size));
    memset(chunk, 0, sizeof(uint8_t) + self->item_size);
    self->size--;
}

void *array_list_get(List *self, uint32_t index) {
    assert(index < (self->capacity));
    uint8_t *chunk = (uint8_t *)(self->buffer + get_offset(index, self->item_size));
    return (sizeof(uint8_t) + chunk);
}

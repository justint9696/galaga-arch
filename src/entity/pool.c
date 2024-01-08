#include "entity/pool.h"

#include <assert.h>
#include <string.h>

static EntityList entities; 

void entity_prepare_all() {
    memset(&entities, 0, sizeof(EntityList));
    entities.items = calloc(ENTITY_MAX, sizeof(item_s));
}

void entity_release_all() {
    free(entities.items);
}

Entity *entity_request() {
    assert(entities.size < ENTITY_MAX);

    size_t i;
    Entity *e;
    item_s *item;
    for (i = 0; i < ENTITY_MAX; i++) {
        item = &entities.items[i];
        if (item->slot == S_FREE)
            break;
    }

    e = &item->data;
    e->index = i;

    item->slot = S_USED;
    entities.size++;

    return e;
}

void entity_release(Entity *self) {
    assert(self->index < ENTITY_MAX);

    uint32_t index = self->index;
    item_s *item = &entities.items[index];
    memset(&item->data, 0, sizeof(Entity));

    item->slot = S_FREE;
    entities.size--;
}

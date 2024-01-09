#include "entity/pool.h"

#include <assert.h>
#include <string.h>

static Pool entities;

void entity_prepare_all() {
    memset(&entities, 0, sizeof(Pool));
    entities.items = calloc(ENTITY_MAX, sizeof(Entity));
}

void entity_release_all() {
    free(entities.items);
}

Entity *entity_request() {
    assert(entities.size < ENTITY_MAX);

    size_t i;
    Entity *e;
    for (i = 0; i < ENTITY_MAX; i++) {
        e = &entities.items[i];
        if (!entity_has_flag(e, FLAG_ACTIVE))
            break;
    }

    e->index = i;
    entity_set_flag(e, FLAG_ACTIVE);
    entities.size++;

    return e;
}

void entity_release(Entity *self) {
    assert(self->index < ENTITY_MAX);

    uint32_t index = self->index;
    Entity *e = &entities.items[index];
    memset(e, 0, sizeof(Entity));

    entity_clear_flag(e, FLAG_ACTIVE);
    entities.size--;
}

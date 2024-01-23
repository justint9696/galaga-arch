#include "data/queue.h"

#include "entity/entity.h"
#include "entity/logic/path.h"
#include "entity/invader.h"

#include "game/world.h"

void invader_init(Entity *self, World *world) {
    self->dim = (vec2) {
        .width = INVADER_WIDTH,
        .height = INVADER_HEIGHT,
    };
    self->team = TEAM_AXIS;
    self->texture = renderer_texture(TEX_INVADER);
    self->health = 1.f;
    self->state = STATE_SPAWN;
    entity_set_flag(self, (FLAG_COLLISION | FLAG_AI_CONTROLLED));
}

static void destroy_queue(Queue *q) {
    Path *p;
    size_t size = q->size;
    for (size_t i = 0; i < size; i++) {
        p = (Path *)queue_pop(q);
        free(p);
    }
}

void invader_destroy(Entity *self, World *world) {
    destroy_queue(&self->path);
}

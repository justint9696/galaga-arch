#include "data/queue.h"

#include "entity/entity.h"
#include "entity/logic/path.h"
#include "entity/abductor.h"

#include "game/world.h"

void abductor_init(Entity *self, World *world) {
    self->dim = (vec2) {
        .width = ABDUCTOR_WIDTH,
        .height = ABDUCTOR_HEIGHT,
    };
    self->team = TEAM_AXIS;
    self->texture = renderer_texture(TEX_ABDUCTOR);
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

void abductor_destroy(Entity *self, World *world) {
    destroy_queue(&self->path);
}

#include "data/queue.h"

#include "entity/formation.h"
#include "entity/invader.h"

#include "game/world.h"

void invader_init(Entity *self, World *world) {
    self->dim = VEC2(INVADER_WIDTH, INVADER_HEIGHT);
    self->team = TEAM_AXIS;
    self->texture = renderer_texture_handle(TEX_INVADER);
    self->health = 1.f;
    entity_link(self, world->formation);
    entity_set_flag(self, INVADER_FLAGS);
    entity_set_state(self, STATE_SPAWN);
}

void invader_destroy(Entity *self, World *world) {
    queue_free(&self->path);
}

void invader_update(Entity *self, World *world) {
    if (!queue_is_empty(&self->path))
        return;

    vec2 pos = formation_entity_position(world->formation, self);
    if (self->pos.y >= pos.y)
        return;

    switch (self->state) {
        case STATE_IDLE:
            entity_set_state(self, STATE_CHARGE);
            break;
        default:
            break;
    }
}

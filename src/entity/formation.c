#include "entity/logic/path.h"
#include "entity/logic/route.h"
#include "entity/formation.h"

#include "game/world.h"

#include <assert.h>

void formation_init(Entity *self, World *world) {
    self->pos = (vec2) {
        .x = FORMATION_SPAWN_X,
        .y = FORMATION_SPAWN_Y,
    };
    self->dim = (vec2) {
        .width = FORMATION_WIDTH,
        .height = FORMATION_HEIGHT,
    };
    self->team = TEAM_ALLY;
    self->health = 1.f;
    self->state = STATE_ALIVE;
}

void formation_update(Entity *self, World *world) {
    if (!self->path.size)
        route_idle(self, FORMATION_VELOCITY);

    Path *path = (Path *)queue_front(&self->path);
    assert(path);

    path_update(path, self);
    if (path->complete) {
        dequeue(&self->path);
        free(path);
    }
}

vec2 formation_entity_position(const Entity *self, uint32_t id) {
    vec2 pos = entity_tag(self, TAG_TOP_LEFT);
    pos.x += ((int)fmod(id, FORMATION_ROW_MAX) * (ENEMY_WIDTH + FORMATION_DISTANCE)) + (ENEMY_WIDTH / 2.f);
    pos.y -= ((int)(id / FORMATION_ROW_MAX) * (ENEMY_HEIGHT + FORMATION_DISTANCE)) + (ENEMY_HEIGHT / 2.f);
    return pos;
}

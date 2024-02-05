#include "entity/logic/enemy.h"
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
    if (queue_is_empty(&self->path))
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
    // starting from top left
    // vec2 pos = entity_tag(self, TAG_TOP_LEFT);
    // pos.x += ((int)fmod(id, FORMATION_ROW_MAX) * (ENEMY_WIDTH + FORMATION_DISTANCE)) + (ENEMY_WIDTH / 2.f);
    // pos.y -= ((int)(id / FORMATION_ROW_MAX) * (ENEMY_HEIGHT + FORMATION_DISTANCE)) + (ENEMY_HEIGHT / 2.f);

    // starting from top middle
    ivec2 offset = {
        .x = id % 2 == 0 ? -ceil(fmod(id + 1, FORMATION_ROW_MAX) / 2.f) : floor(fmod(id, FORMATION_ROW_MAX) / 2.f),
    };

    vec2 pos = entity_tag(self, TAG_TOP_MIDDLE);
    pos.x += (((ENEMY_WIDTH + FORMATION_DISTANCE) / 2.f) + (ENEMY_WIDTH / 2.f)) * offset.x;
    pos.y -= ((int)(id / FORMATION_ROW_MAX) * (ENEMY_HEIGHT + FORMATION_DISTANCE)) + (ENEMY_HEIGHT / 2.f);

    return pos;
}

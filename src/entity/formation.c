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

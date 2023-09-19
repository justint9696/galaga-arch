#include "entity/logic/enemy.h"
#include "entity/logic/formation.h"

#include "common/util.h"

#include "gfx/window.h"

#include <string.h>

#define FORM_CENTER_X       ENEMY_SPAWN_X
#define FORM_CENTER_Y       ENEMY_SPAWN_Y
#define FORM_VELOCITY       ENEMY_IDLE_VELOCITY
#define FORM_DISTANCE       5.f

static inline float _get_x_dist() {
    return (ENEMY_WIDTH + FORM_DISTANCE);
}

static inline float _get_y_dist() {
    return (ENEMY_HEIGHT + FORM_DISTANCE);
}

void Formation_Init(Formation *self) {
    memset(self, 0, sizeof(Formation));
    self->center = (vec2) {
        .x = FORM_CENTER_X,
        .y = FORM_CENTER_Y,
    };
    self->vel.x = FORM_VELOCITY;
}

vec2 Formation_GetPosition(Formation *self, uint32_t id) {
    self->id = max(self->id, id);

    ivec2 offset = {
        .x = id % 2 == 0 ? -floor(id / 2) - 1: floor(id / 2) + 1,
        .y = 0,
    };

    return (vec2) {
        .x = self->center.x + (offset.x * _get_x_dist()),
        .y = self->center.y + (offset.y * _get_y_dist()),
    };
}

void Formation_Update(Formation *self, uint64_t deltaTime) {
    self->center.x += (1.f * deltaTime * self->vel.x);
    self->center.y += (1.f * deltaTime * self->vel.y);

    vec2 pos;
    if (self->vel.x < 0)
        pos = Formation_GetPosition(self, self->id % 2 == 0 ? self->id: self->id - 1);
    else
        pos = Formation_GetPosition(self, self->id);

    if (pos.x + ENEMY_WIDTH > WINDOW_WIDTH || pos.x < 0) 
        self->vel.x = -self->vel.x;
}

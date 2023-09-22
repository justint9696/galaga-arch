#include "entity/entity.h"
#include "entity/logic/enemy.h"
#include "entity/logic/formation.h"

#include "common/util.h"

#include "gfx/window.h"

#include <string.h>

#define FORM_CENTER_X       ENEMY_SPAWN_X
#define FORM_CENTER_Y       ENEMY_SPAWN_Y
#define FORM_VELOCITY       ENEMY_IDLE_VELOCITY
#define FORM_DISTANCE       15.f

static inline float _get_x_dist() {
    return (ENEMY_WIDTH + FORM_DISTANCE);
}

static inline float _get_y_dist() {
    return (ENEMY_HEIGHT + FORM_DISTANCE);
}

void Formation_Init(Formation *self) {
    memset(self, 0, sizeof(Formation));
    Entity_Init(&self->entity, TYPE_FORMATION, TEAM_AXIS, 1.f, FORM_CENTER_X, FORM_CENTER_Y, 10, 10, NULL);
    Entity_SetVelocity(&self->entity, (vec2) { .x = FORM_VELOCITY, .y = 0.f });
}

vec2 Formation_GetPosition(Formation *self, uint32_t id) {
    self->id = max(self->id, id);

    ivec2 offset = {
        .x = id % 2 == 0 ? -ceil(id / 2) : floor(id / 2) + 1,
        .y = 0,
    };

    return (vec2) {
        .x = self->entity.pos.x + (offset.x * _get_x_dist()) - (ENEMY_WIDTH / 2.f),
        .y = self->entity.pos.y + (offset.y * _get_y_dist()),
    };
}

vec2 Formation_ApproxPosition(Formation *self, uint32_t id, float time) {
    /*
     * displacement-time formula:
     * sf = s0 + v0 * t + (1/2) a * t^2
     */
    vec2 pos = Formation_GetPosition(self, id);
    pos = (vec2) {
        .x = pos.x + self->entity.vel.x * time,
        .y = pos.y + self->entity.vel.y * time,
    };

    const uint32_t max_dist = WINDOW_WIDTH - ENEMY_WIDTH;
    if (pos.x && pos.x < max_dist)
        return pos;

    float dx = max_dist - self->entity.pos.x;
    time -= (dx / self->entity.vel.x);
    
    pos.x = max_dist + (pos.x ? self->entity.vel.x : -self->entity.vel.x) * time;

    return pos;
}

void Formation_Update(Formation *self) {
    vec2 pos;
    int num = ((WAVE_COMPLETE) * 2) - 1;
    if (self->entity.vel.x < 0)
        pos = Formation_GetPosition(self, num - 1);
    else
        pos = Formation_GetPosition(self, num);

    if (pos.x + ENEMY_WIDTH > WINDOW_WIDTH || pos.x < 0) 
        Entity_SetVelocity(&self->entity, (vec2) { .x = -self->entity.vel.x, .y = 0.f });
}

#include "entity/logic/formation.h"
#include "entity/logic/route.h"
#include "entity/logic/path.h"

#include "common/util.h"

#include "game/wave.h"
#include "gfx/window.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define FORM_CENTER_X       ENEMY_SPAWN_X
#define FORM_CENTER_Y       ENEMY_SPAWN_Y
#define FORM_VELOCITY       ENEMY_IDLE_VELOCITY
#define FORM_DISTANCE       15.f
#define MAX_DIST            WINDOW_WIDTH - ENEMY_WIDTH

static inline float _get_x_dist() {
    return (ENEMY_WIDTH + FORM_DISTANCE);
}

static inline float _get_y_dist() {
    return (ENEMY_HEIGHT + FORM_DISTANCE);
}

void Formation_Init(Formation *self) {
    memset(self, 0, sizeof(Formation));
    Entity_Init(&self->entity, TYPE_FORMATION, TEAM_AXIS, 1.f, FORM_CENTER_X, FORM_CENTER_Y, 10, 10, NULL);
    Entity_SetVelocity(&self->entity, (vec2) { .x = 0.f, .y = 0.f });
}

void Formation_Destroy(Formation *self) {
    Queue *q = &self->path;
    for (size_t i = 0; i < q->size; i++) {
        free(queue_front(q));
        dequeue(q);
    }
}

vec2 Formation_GetPosition(Formation *self, uint32_t id) {
    ivec2 offset = {
        .x = id % 2 == 0 ? -ceil(id / 2) : floor(id / 2) + 1,
        .y = 0,
    };

    return (vec2) {
        .x = self->entity.pos.x + (offset.x * _get_x_dist()),
        .y = self->entity.pos.y + (offset.y * _get_y_dist()),
    };
}

void Formation_Update(Formation *self) {
    path_s *path;
    if (!self->path.size) {
        Route_Idle(&self->path, self->entity.pos);
        path = (path_s *)queue_front(&self->path);

        if (path->dst.x < MAX_DIST)
            path->dst.x += (WAVE_COUNT * _get_x_dist());
        else
            path->dst.x -= ((WAVE_COUNT + 1) * _get_x_dist());
    } else
        path = (path_s *)queue_front(&self->path);

    assert(path);

    Path_Update(&self->entity, path);
    if (path->complete) {
        dequeue(&self->path);
        free(path);
    }
}

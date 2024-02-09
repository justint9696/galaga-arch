#include "data/queue.h"

#include "entity/logic/path.h"
#include "entity/logic/route.h"

#include "entity/formation.h"
#include "entity/invader.h"

#include "game/world.h"

void invader_init(Entity *self, World *world) {
    self->dim = (vec2) {
        .width = INVADER_WIDTH,
        .height = INVADER_HEIGHT,
    };
    self->team = TEAM_AXIS;
    self->texture = renderer_texture_handle(TEX_INVADER);
    self->health = 1.f;
    entity_set_state(self, STATE_SPAWN);
    entity_link(self, world->formation);
    entity_set_flag(self, INVADER_FLAGS);
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

static void teleport(Entity *self, World *world) {
    entity_set_position(self, (vec2) { .x = self->pos.x, .y = SCREEN_HEIGHT + 100.f });
    route_merge(self, INVADER_VELOCITY);
    entity_set_state(self, STATE_MERGE);
}

static vec2 get_displacement(Entity *self) {
    return (vec2) { 
        .x = self->pos.x - self->prev_pos.x,
        .y = self->pos.y - self->prev_pos.y,
    };
}

static void charge(Entity *self, World *world) {
    const Entity *player = world->player;
    vec2 vel = get_displacement(self);
    route_start(&self->path, self->pos, (vec2) { .x = self->pos.x, .y = self->pos.y + 100.f }, vel.x < 0.f ? -INVADER_VELOCITY : INVADER_VELOCITY, PATH_CIRCULAR);
    route_append(&self->path, (vec2) { .x = player->pos.x, .y = -50.f }, -INVADER_VELOCITY, PATH_BEZIER);
    entity_set_state(self, STATE_CHARGE);
}

static void update(Entity *self, World *world) {
    if (self->pos.y <= 0.f)
        teleport(self, world);
    else
        charge(self, world);
}

void invader_update(Entity *self, World *world) {
    if (!queue_is_empty(&self->path))
        return;

    vec2 pos = formation_entity_position(world->formation, self);
    if (self->pos.y >= pos.y)
        return;

    switch (self->state) {
        case STATE_IDLE:
            update(self, world);
            break;
        default:
            break;
    }
}

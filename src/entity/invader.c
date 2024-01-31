#include "data/queue.h"

#include "entity/formation.h"
#include "entity/logic/path.h"
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
    self->state = STATE_SPAWN;
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
    entity_set_position(self, (vec2) {
        .x = self->pos.x,
        .y = SCREEN_HEIGHT + 100.f 
    });

    Path *path = path_init();
    path->org = self->pos;
    path->dst = (vec2) { 
        .x = FORMATION_SPAWN_X, 
        .y = FORMATION_SPAWN_Y,
    };
    path->type = PATH_BEZIER;
    path->speed = INVADER_VELOCITY;
    enqueue(&self->path, path);
    
    self->state = STATE_SPAWN;
}

static void charge(Entity *self, World *world) {
    const Entity *player = world->player;
    
    Path *path = path_init();
    path->org = self->pos;
    path->dst = (vec2) { 
        .x = player->pos.x, 
        .y = -100.f,
    };
    path->type = PATH_BEZIER;
    path->speed = INVADER_VELOCITY;
    enqueue(&self->path, path);
    
    self->state = STATE_CHARGE;
}

void invader_update(Entity *self, World *world) {
    if (!queue_is_empty(&self->path))
        return;

    vec2 pos = formation_entity_position(world->formation, self->id);
    if (self->pos.y >= pos.y)
        return;

    switch (self->state) {
        case STATE_IDLE:
            if (self->pos.y <= 0.f)
                teleport(self, world);
            else
                charge(self, world);
            break;
        default:
            break;
    }
}

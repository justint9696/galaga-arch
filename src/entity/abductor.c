#include "common/util.h"

#include "data/queue.h"

#include "entity/logic/enemy.h"
#include "entity/logic/path.h"
#include "entity/logic/route.h"

#include "entity/abductor.h"
#include "entity/tractor_beam.h"

#include "game/world.h"

void abductor_init(Entity *self, World *world) {
    self->dim = (vec2) {
        .width = ABDUCTOR_WIDTH,
        .height = ABDUCTOR_HEIGHT,
    };
    self->team = TEAM_AXIS;
    self->texture = renderer_texture_handle(TEX_ABDUCTOR);
    self->health = 1.f;
    entity_set_state(self, STATE_SPAWN);
    entity_link(self, world->formation);
    entity_set_flag(self, ABDUCTOR_FLAGS);
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

static vec2 get_displacement(Entity *self) {
    return (vec2) { 
        .x = self->pos.x - self->prev_pos.x,
        .y = self->pos.y - self->prev_pos.y,
    };
}

static void move_to_player(Entity *self, World *world) {
    // move x tiles above player.y
    const Entity *player = world->player;
    if (self->pos.y <= player->pos.y + 150.f) {
        entity_set_state(self, STATE_EXPAND);
        entity_clear_flag(self, FLAG_AI_CONTROLLED);
        return;
    }

    vec2 vel = get_displacement(self);
    route_start(&self->path, self->pos, (vec2) { .x = self->pos.x, .y = self->pos.y + 100.f }, vel.x < 0.f ? -ABDUCTOR_VELOCITY : ABDUCTOR_VELOCITY, PATH_CIRCULAR);
    route_append(&self->path, (vec2) { .x = player->pos.x, .y = player->pos.y + 150.f }, vel.x < 0.f ? -ABDUCTOR_VELOCITY : ABDUCTOR_VELOCITY, PATH_BEZIER);
    entity_set_state(self, STATE_CHARGE);
}

static void tractor_beam_present(Entity *self, World *world) {
    if (self->child) {
        entity_set_state(self, STATE_RETRACT);
        return;
    }

    Entity *e = entity_init(E_TRACTOR_BEAM, tractor_beam_init, tractor_beam_destroy, tractor_beam_update, world);
    e->pos = entity_tag(self, TAG_BOTTOM_MIDDLE);
    e->pos = (vec2) {
        .x = e->pos.x - (TRACTOR_BEAM_WIDTH / 2.f),
        .y = e->pos.y - TRACTOR_BEAM_HEIGHT - 5.f,
    };

    self->child = e;
    world_add_entity(world, e);
    entity_set_rotation(self, 0.f);
}

static void teleport(Entity *self, World *world) {
    entity_set_position(self, (vec2) { .x = self->pos.x, .y = SCREEN_HEIGHT + 100.f });
    route_merge(self, ABDUCTOR_VELOCITY);
    entity_set_state(self, STATE_MERGE);
    entity_set_flag(self, FLAG_AI_CONTROLLED);
}

static void tractor_beam_monitor(Entity *self, World *world) {
    // check if tractor beam has been destroyed
    Entity *e = self->child;
    if (e && entity_is_alive(e))
        return;

    LOG("et phone home\n");

    // if so, remove child and return to formation
    self->child = NULL;
    teleport(self, world);
}

static void abduct_player(Entity *self, World *world) {
    tractor_beam_monitor(self, world);
}

void abductor_update(Entity *self, World *world) {
    if (!queue_is_empty(&self->path))
        return;

    if (enemy_in_formation(self, world))
        return;

    // LOG("abductor: state: %i\tprev_state: %i\n", self->state, self->prev_state);
    switch (self->state) {
        case STATE_IDLE:
            move_to_player(self, world);
            break;
        case STATE_EXPAND:
            tractor_beam_present(self, world);
            break;
        case STATE_RETRACT:
            tractor_beam_monitor(self, world);
            break;
        case STATE_ABDUCT:
            abduct_player(self, world);
            break;
        default: break;
    }
}

#include "data/queue.h"

#include "entity/logic/enemy.h"
#include "entity/logic/path.h"
#include "entity/logic/route.h"

#include "entity/abductor.h"
#include "entity/tractor_beam.h"

#include "game/world.h"

void abductor_init(Entity *self, World *world) {
    self->dim = VEC2(ABDUCTOR_WIDTH, ABDUCTOR_HEIGHT);
    self->team = TEAM_AXIS;
    self->texture = renderer_texture_handle(TEX_ABDUCTOR);
    self->health = 1.f;
    entity_link(self, world->formation);
    entity_set_flag(self, ABDUCTOR_FLAGS);
    entity_set_state(self, STATE_SPAWN);
}

void abductor_destroy(Entity *self, World *world) {
    queue_free(&self->path);
}

static void move_to_player(Entity *self, World *world) {
    // move x tiles above player.y
    const Entity *player = world->player;
    if (self->pos.y <= player->pos.y + 150.f) {
        entity_set_state(self, STATE_EXPAND);
        entity_clear_flag(self, FLAG_AI_CONTROLLED);
        return;
    }

    vec2 vel = entity_displacement(self);
    route_start(&self->path, self->pos, VEC2(self->pos.x, self->pos.y + 100.f), vel.x < 0.f ? -ENEMY_VELOCITY : ENEMY_VELOCITY, PATH_CIRCULAR);
    route_append(&self->path, VEC2(player->pos.x, player->pos.y + 150.f), vel.x < 0.f ? -ENEMY_VELOCITY : ENEMY_VELOCITY, PATH_BEZIER);
    entity_set_state(self, STATE_CHARGE);
}

static void tractor_beam_present(Entity *self, World *world) {
    Entity *e = entity_create(E_TRACTOR_BEAM, world);
    e->pos = entity_tag(self, TAG_BOTTOM_MIDDLE);
    e->pos = VEC2(e->pos.x - (TRACTOR_BEAM_WIDTH / 2.f), e->pos.y - TRACTOR_BEAM_HEIGHT - 5.f);
    e->parent = self;

    self->child = e;
    world_add_entity(world, e);
    entity_set_rotation(self, 0.f);

    entity_set_state(self, STATE_RETRACT);
}

static void return_to_formation(Entity *self, World *world) {
    // entity_set_position(self, VEC2(self->pos.x, SCREEN_HEIGHT + 100.f));
    // entity_set_state(self, STATE_MERGE);
    entity_set_flag(self, FLAG_AI_CONTROLLED);
    entity_set_state(self, STATE_CHARGE);
}

static void tractor_beam_monitor(Entity *self, World *world) {
    // check if tractor beam has been destroyed
    Entity *e = self->child;
    if (e && entity_is_alive(e))
        return;

    // if so, remove child and return to formation
    self->child = NULL;
    return_to_formation(self, world);
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

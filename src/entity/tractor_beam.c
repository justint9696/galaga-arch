#include "common/time.h"
#include "common/util.h"

#include "entity/entity.h"
#include "entity/player.h"
#include "entity/tractor_beam.h"

#include "game/world.h"

#define TARGET_WIDTH (PLAYER_WIDTH + 20.f)

// wait a period of time
// after that has passed, expand the width of the beam
// iteratively render each beam based it's height and width
// once the beam has been fully emitted
// check if player is colliding with it
// if not, wait a period of time
// otherwise, change state into abduct state
// if time has passed, destroy the beam

static void render(Entity *self, World *world) {
    // TODO: render a pyramid shaped figure with a rounded bottom
    // added pixels per y = w/(h-1)
    const uint32_t p = floor(self->dim.w / (self->dim.h - 1));
}

void tractor_beam_init(Entity *self, World *world) {
    self->dim = VEC2(TRACTOR_BEAM_WIDTH, TRACTOR_BEAM_HEIGHT);
    self->color = COLOR_CYAN;
    self->team = TEAM_AXIS;
    self->health = 1.f;
    // self->render = render;
    self->tick = NOW();
    self->depth = DEPTH_PROJECTILE;
    entity_set_flag(self, TRACTOR_BEAM_FLAGS);
    entity_set_state(self, STATE_IDLE);
}

void tractor_beam_collide(Entity *self, Entity *entity, World *world) {
    switch (entity->type) {
        case E_PROJECTILE:
            entity_set_state(entity, STATE_DEAD);
            break;
        case E_PLAYER:
            // remove player control and collision
            entity_clear_flag(entity, PLAYER_FLAGS);

            // link the player to tractor beam
            entity_link(entity, self);
            entity_set_flag(entity, FLAG_PARENT_REF);

            // start player abduction
            entity_set_state(self, STATE_ABDUCT);
            entity_set_velocity(entity, VEC2(0.f, 0.1f));
            break;
        default:
            break;
    }
}

void tractor_beam_destroy(Entity *self, World *world) {
    // check if player was abducted
    // if so, pass player to parent
    LOG("tractor beam destroyed\n");
}

static void wait_delay(Entity *self) {
    if (time_since(self->tick) < TRACTOR_BEAM_DELAY)
        return;

    switch (self->prev_state) {
        case STATE_EXPAND:
            entity_set_state(self, STATE_RETRACT);
            break;
        default:
            entity_set_state(self, STATE_EXPAND);
            break;
    }
}

static void expand_beam(Entity *self) {
    if (self->dim.width >= TARGET_WIDTH) {
        self->tick = NOW();
        entity_set_state(self, STATE_IDLE);
        entity_set_flag(self, FLAG_COLLISION);
        return;
    }

    uint32_t delta = time_delta();

    float dist = (TARGET_WIDTH - TRACTOR_BEAM_WIDTH);
    float time = TRACTOR_BEAM_EXPAND_TIME;

    float vel = (dist / time) * delta;

    entity_transform(self, (vec2) { .w = self->dim.w + vel, .h = self->dim.h });
}

static void retract_beam(Entity *self) {
    if (self->dim.width < TRACTOR_BEAM_WIDTH) {
        entity_set_state(self, STATE_DEAD);
        return;
    }

    uint32_t delta = time_delta();

    float dist = (TARGET_WIDTH - TRACTOR_BEAM_WIDTH);
    float time = TRACTOR_BEAM_EXPAND_TIME;

    float vel = (dist / time) * delta;

    entity_transform(self, (vec2) { .w = self->dim.w - vel, .h = self->dim.h });
}

static void abduct_player(Entity *self, Entity *player) {
    // move player to center of beam
    // spin player model
}

void tractor_beam_update(Entity *self, World *world) {
    switch (self->state) {
        case STATE_IDLE:
            wait_delay(self);
            break;
        case STATE_EXPAND:
            // expand the beam
            expand_beam(self);
            break;
        case STATE_ABDUCT:
            // player is colliding with tractor beam
            abduct_player(self, world->player);
            break;
        case STATE_RETRACT:
            // time has passsed and player is not under beam
            retract_beam(self);
            break;
        default: break;
    }
}

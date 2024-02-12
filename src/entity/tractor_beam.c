#include "common/time.h"
#include "common/util.h"

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
    entity_set_flag(self, TRACTOR_BEAM_FLAGS);
    entity_set_state(self, STATE_IDLE);
}

void tractor_beam_destroy(Entity *self, World *world) {
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
            break;
        case STATE_RETRACT:
            // time has passsed and player is not under beam
            retract_beam(self);
            break;
        default: break;
    }
}

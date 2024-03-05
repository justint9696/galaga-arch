#include "common/time.h"
#include "common/util.h"

#include "entity/star.h"

#include "game/world.h"

#include "gfx/window.h"

static float random_x() {
    return 1.f * RAND(SCREEN_WIDTH);
}

static float random_y() {
    return 1.f * RAND(SCREEN_HEIGHT);
}

static state_t random_state() {
    return RAND(2) < 1 ? STATE_FADE_OUT : STATE_FADE_IN;
}

static uint32_t random_color() {
    return STAR_COLOR + RAND(0xFF);
}

static void fade(Entity *self) {
    uint8_t alpha = self->color & 0xFF;
    uint32_t delta = time_delta();

    switch (self->state) {
        case STATE_FADE_OUT:
            if (alpha > 0)
                self->color -= delta;
            else
                self->state = STATE_FADE_IN;
            break;
        case STATE_FADE_IN:
            if (alpha < 0xFF)
                self->color += delta;
            else
                self->state = STATE_FADE_OUT;
            break;
        default: break;
    }
}

void star_init(Entity *self, World *world) {
    self->pos = VEC2(random_x(), random_y());
    self->vel = VEC2(0.f, -STAR_VELOCITY);
    self->dim = VEC2(STAR_WIDTH, STAR_HEIGHT);
    self->color = random_color();
    self->state = random_state();
    self->health = 1.f;
    self->depth = DEPTH_BASE;
}

void star_update(Entity *self, World *world) {
    if (self->pos.y < 0) {
        self->pos.x = random_x();
        self->pos.y = SCREEN_HEIGHT;
        self->state = random_state();
        self->color = random_color();
    }
    fade(self);
}

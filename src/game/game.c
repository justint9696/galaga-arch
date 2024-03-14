#include "game/game.h"
#include "common/time.h"
#include "common/util.h"
#include "entity/entity.h"
#include "game/buttons.h"
#include "game/hud.h"

static bool player_is_alive(Game *self) {
    return entity_is_alive(self->world.player);
}

static void game_set_state(Game *self, game_state_t state) {
    switch (state) {
        case G_PAUSED:
        case G_IDLE:
            time_set_paused(true);
            break;
        case G_PLAYING:
            time_set_paused(false);
            break;
        default:
            break;
    }

    self->state = state;
    game_hud_reset(self);
}

static void game_toggle_ui(Game *self) {
    switch (self->state) {
        case G_PLAYING:
            game_pause(self);
            break;
        case G_PAUSED:
            game_resume(self);
            break;
        default:
            break;
    }
}

static void monitor_input(Game *self) {
    if (button_pressed(SDL_SCANCODE_ESCAPE, true)) {
        game_toggle_ui(self);
    }
    if (button_pressed(SDL_SCANCODE_R, true)) {
        game_restart(self);
    }
    if (button_pressed(SDL_SCANCODE_Q, true)) {
        game_quit(self);
    }
}

static void update(Game *self) {
    if (self->state == G_PLAYING) {
        stage_update(&self->stage, &self->world);
    }

    world_update(&self->world);
    if (!player_is_alive(self) && self->state == G_PLAYING) {
        game_set_state(self, G_DEAD);
    }
}

void game_init(Game *self) {
    buttons_init();
    time_init();

    world_init(&self->world);
    stage_init(&self->stage, &self->world);

    game_set_state(self, G_IDLE);
}

void game_update(Game *self) {
    time_update();

    monitor_input(self);
    update(self);

    game_hud_update(self);

    time_limit();
}

bool game_is_running(Game *self) {
    return self->state != G_QUIT;
}

void game_destroy(Game *self) {
    // stage_destroy(&self->stage, &self->world);
    world_destroy(&self->world);
}

void game_pause(Game *self) {
    game_set_state(self, G_PAUSED);
    LOG("%s paused.\n", GAME_TITLE);
}

void game_resume(Game *self) {
    game_set_state(self, G_PLAYING);
    LOG("%s resumed.\n", GAME_TITLE);
}

void game_restart(Game *self) {
    game_init(self);
    game_set_state(self, G_PLAYING);
    LOG("%s restarted.\n", GAME_TITLE);
}

void game_quit(Game *self) {
    game_set_state(self, G_QUIT);
    LOG("%s terminated.\n", GAME_TITLE);
}

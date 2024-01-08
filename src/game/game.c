#include "common/time.h"
#include "game/buttons.h"
#include "game/game.h"

void game_init(Game *self) {
    buttons_init();
    time_init();

    stage_init(&self->stage);
    world_init(&self->world);

    // set game state to idle
    self->state = G_IDLE;
}

void game_update(Game *self) {
    time_update();
    stage_update(&self->stage, &self->world);
    world_update(&self->world);

    if (!entity_is_alive(self->world.player)) 
        self->state = G_QUIT;

    time_limit();
}

void game_destroy(Game *self) {
    world_destroy(&self->world);
    stage_destroy(&self->stage);
}

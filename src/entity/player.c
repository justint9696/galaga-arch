#include "common/util.h"

#include "entity/player.h"

#include "game/buttons.h"
#include "game/world.h"

#include <string.h>

void player_init(Entity *self, World *world) {
    self->pos = (vec2) {
        .x = PLAYER_SPAWN_X,
        .y = PLAYER_SPAWN_Y,
    };
    self->dim = (vec2) {
        .width = PLAYER_WIDTH,
        .height = PLAYER_HEIGHT,
    };
    self->team = TEAM_ALLY;
    self->texture = renderer_texture_handle(TEX_PLAYER);
    self->health = 1.f;
    self->state = STATE_ALIVE;
    entity_set_flag(self, PLAYER_FLAGS);

    LOG("Player initialized.\n");
}

void player_update(Entity *self, World *world) {
    vec2 vel = { 0 };
    if (button_pressed(SDL_SCANCODE_LEFT, false))
        vel.x = -PLAYER_VELOCITY;
    else if (button_pressed(SDL_SCANCODE_RIGHT, false))
        vel.x = PLAYER_VELOCITY;
    if (button_pressed(SDL_SCANCODE_SPACE, true))
        entity_fire(self, world);

    if (!memcmp(&self->vel, &vel, sizeof(vec2)))
        return;

    entity_set_velocity(self, vel);
}

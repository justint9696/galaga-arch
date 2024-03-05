#include "common/util.h"

#include "entity/player.h"

#include "game/buttons.h"
#include "game/world.h"

#include <string.h>

void player_init(Entity *self, World *world) {
    self->pos = VEC2(PLAYER_SPAWN_X, PLAYER_SPAWN_Y);
    self->dim = VEC2(PLAYER_WIDTH, PLAYER_HEIGHT);
    self->team = TEAM_ALLY;
    self->texture = texture_handle(TEX_PLAYER);
    self->health = 1.f;
    self->depth = DEPTH_CHARACTER;
    entity_set_flag(self, PLAYER_FLAGS);
    entity_set_state(self, STATE_ALIVE);

    LOG("Player initialized.\n");
}

void player_handle_input(Entity *self, World *world) {
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

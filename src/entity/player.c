#include "common/util.h"

#include "entity/player.h"
#include "entity/projectile.h"

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
    self->texture = load_texture(PLAYER_TEXTURE);
    self->health = 1.f;
    self->flags = FLAG_COLLISION;
    self->state = STATE_ALIVE;

    LOG("Player initialized.\n");
}

void player_update(Entity *self, World *world) {
    vec2 vel = { 0 };
    if (button_pressed(SDL_SCANCODE_LEFT, false))
        vel.x = -PLAYER_VELOCITY;
    else if (button_pressed(SDL_SCANCODE_RIGHT, false))
        vel.x = PLAYER_VELOCITY;
    if (button_pressed(SDL_SCANCODE_SPACE, true))
        entity_fire(self, world, PROJECTILE_COOLDOWN);

    if (!memcmp(&self->vel, &vel, sizeof(vec2)))
        return;

    entity_set_velocity(self, vel);
}

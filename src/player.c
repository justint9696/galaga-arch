#include "include/entity.h"
#include "include/player.h"

static Player player;

void Player_Init() {
    player.buttons = 0;

    Entity *self = Entity_Init(PLAYER_SPAWN_X, PLAYER_SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_TEXTURE);
    player.ent = self;
}

void Player_Update(int buttons) {
    player.buttons = buttons;

    if (buttons & BUTTON_UP)
        Entity_SetVelocity(player.ent, 0, PLAYER_VEL);
}
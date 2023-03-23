#include "include/entity.h"
#include "include/player.h"

static Player _player;

void Player_Init() {
    memset(&_player, 0, sizeof(Player));
    _player.buttons = 0;
    _player.ent = Entity_Init(PLAYER_SPAWN_X, PLAYER_SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_TEXTURE);

    printf("Player initialized.\n");
}

void Player_Update(int buttons) {
    _player.buttons = buttons;

    Entity_SetVelocity(_player.ent, 0, 0);

    if (buttons & BUTTON_UP)
        Entity_SetVelocity(_player.ent, 0, PLAYER_VEL);
    if (buttons & BUTTON_DOWN)
        Entity_SetVelocity(_player.ent, 0, -PLAYER_VEL);
    if (buttons & BUTTON_LEFT)
        Entity_SetVelocity(_player.ent, -PLAYER_VEL, 0);
    if (buttons & BUTTON_RIGHT)
        Entity_SetVelocity(_player.ent, PLAYER_VEL, 0);
    // if (buttons & BUTTON_SPACE)
    //     return;
}
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
    if (buttons == _player.buttons) 
        return;

    _player.buttons = buttons;

    float vx = 0.f, vy = 0.f;

    if (buttons & BUTTON_UP)
        vy = PLAYER_VEL;
    else if (buttons & BUTTON_DOWN)
        vy = -PLAYER_VEL;

    if (buttons & BUTTON_LEFT)
        vx = -PLAYER_VEL;
    else if (buttons & BUTTON_RIGHT)
        vx = PLAYER_VEL;
        
    Entity_SetVelocity(_player.ent, vx, vy);

    // if (buttons & BUTTON_SPACE)
    //     return;
}
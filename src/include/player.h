#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity.h"

#define BUTTON_UP           (1 << 0)
#define BUTTON_DOWN         (1 << 1)
#define BUTTON_LEFT         (1 << 2)
#define BUTTON_RIGHT        (1 << 3)
#define BUTTON_SPACE        (1 << 4)

#define PLAYER_SPAWN_X      50.f
#define PLAYER_SPAWN_Y      100.f
#define PLAYER_VEL          15.f
#define PLAYER_WIDTH        15.f
#define PLAYER_HEIGHT       15.f
#define PLAYER_TEXTURE      "images/player.png"

typedef struct {
    int buttons;
    Entity *ent;
} Player;

void Player_Init();

void Player_Update(int buttons);

#endif
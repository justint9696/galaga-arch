#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity.h"
#include "window.h"

#define BUTTON_UP           (1 << 0)
#define BUTTON_DOWN         (1 << 1)
#define BUTTON_LEFT         (1 << 2)
#define BUTTON_RIGHT        (1 << 3)
#define BUTTON_SPACE        (1 << 4)

#define PLAYER_TEXTURE      "images/player.png"

#define PLAYER_WIDTH        66.f
#define PLAYER_HEIGHT       48.f

#define PLAYER_VELOCITY     0.5f
#define PLAYER_SPAWN_X      ((WINDOW_WIDTH - PLAYER_WIDTH) / 2)
#define PLAYER_SPAWN_Y      50.f

typedef struct {
    int buttons;
    Entity *ent;
} Player;

void Player_Init();

void Player_Update(int buttons);

#endif
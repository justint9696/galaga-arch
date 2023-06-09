#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity.h"
#include "window.h"

#include <stdbool.h>

#define PLAYER_TEXTURE	    "assets/images/player.png"	
#define PLAYER_WIDTH        66.7f
#define PLAYER_HEIGHT       48.f

#define PLAYER_VELOCITY     0.35f
#define PLAYER_SPAWN_HEALTH 1.f
#define PLAYER_SPAWN_X      ((WINDOW_WIDTH - PLAYER_WIDTH) / 2)
#define PLAYER_SPAWN_Y      50.f

typedef struct {
    int buttons;
    Entity *ent;
} Player;

bool Player_IsAlive();

vec2 Player_Position();

vec2 Player_Velocity();

void Player_Init(uint64_t tick);

void Player_Update(uint64_t tick);

#endif

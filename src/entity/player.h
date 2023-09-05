#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common/type.h"
#include "gfx/window.h"
#include "entity/entity.h"

#include <stdbool.h>
#include <stdint.h>

#define PLAYER_TEXTURE	    "assets/images/player.png"	
#define PLAYER_WIDTH        66.7f
#define PLAYER_HEIGHT       48.f

#define PLAYER_VELOCITY     0.35f
#define PLAYER_SPAWN_HEALTH 1.f
#define PLAYER_SPAWN_X      ((WINDOW_WIDTH - PLAYER_WIDTH) / 2)
#define PLAYER_SPAWN_Y      50.f

typedef struct {
    uint32_t p_buttons, buttons;
    Entity entity;
} Player;

bool Player_IsAlive(const Player *self);
bool Player_IsMoving(const Player *self);

vec2 Player_Position(const Player *self);
vec2 Player_Velocity(const Player *self);

void Player_Init(Player *self, uint64_t tick);
void Player_Update(Player *self, uint64_t tick, uint64_t deltaTime);

#endif

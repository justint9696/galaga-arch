#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity/entity.h"
#include "gfx/window.h"

#define PLAYER_WIDTH        66.7f * SCREEN_SCALAR
#define PLAYER_HEIGHT       48.f * SCREEN_SCALAR
#define PLAYER_VELOCITY     0.35f
#define PLAYER_SPAWN_X      ((SCREEN_WIDTH - PLAYER_WIDTH) / 2)
#define PLAYER_SPAWN_Y      50.f
#define PLAYER_FLAGS        (FLAG_COLLISION | FLAG_PLAYER_CONTROLLED)

void player_init(Entity *, World *);
void player_handle_input(Entity *, World *);

#endif

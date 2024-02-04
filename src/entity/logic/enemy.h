#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "entity/entity.h"
#include "gfx/window.h"

#include <stdbool.h>

#define ENEMY_VELOCITY          0.3f
#define ENEMY_WIDTH             66.67f * SCREEN_SCALAR
#define ENEMY_HEIGHT            101.3f * SCREEN_SCALAR
#define ENEMY_SPAWN_X           ((SCREEN_WIDTH - ENEMY_WIDTH) / 2)
#define ENEMY_SPAWN_Y           (SCREEN_HEIGHT - ENEMY_HEIGHT - 50.f)
#define ENEMY_COOLDOWN          500
#define ENEMY_IDLE_TIME         1500

void enemy_ai(Entity *, struct World *);
bool enemy_in_formation(Entity *, struct World *);

#endif

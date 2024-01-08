#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "entity/entity.h"
#include "gfx/window.h"

#define ENEMY_TEXTURE           "assets/images/enemy01.png"
#define ENEMY_SPAWN_HEALTH      1.f
#define ENEMY_WIDTH             66.67f * SCREEN_SCALAR
#define ENEMY_HEIGHT            101.3f * SCREEN_SCALAR
#define ENEMY_SPAWN_X           ((SCREEN_WIDTH - ENEMY_WIDTH) / 2)
#define ENEMY_SPAWN_Y           (SCREEN_HEIGHT - ENEMY_HEIGHT - 50.f)
#define ENEMY_IDLE_VELOCITY     0.1f
#define ENEMY_SPAWN_VELOCITY    0.3f
#define ENEMY_IDLE_TIME         1500    
#define ENEMY_COOLDOWN          600

void enemy_init(Entity *, struct World *);
void enemy_update(Entity *, struct World *);
void enemy_destroy(Entity *, struct World *);

#endif

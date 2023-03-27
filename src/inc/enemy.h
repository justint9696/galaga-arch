#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "entity.h"
#include "window.h"

#define ENEMY_TEXTURE       "images/enemy01.png"
#define ENEMY_SPAWN_X       ((WINDOW_WIDTH - ENEMY_WIDTH) / 2)
#define ENEMY_SPAWN_Y       (WINDOW_HEIGHT - ENEMY_HEIGHT - 50.f)
#define ENEMY_WIDTH         66.67f
#define ENEMY_HEIGHT        101.3f

void Enemy_InitAll(const int count);

#endif

#ifndef _FORMATION_H_
#define _FORMATION_H_

#include "entity/entity.h"
#include "entity/enemy.h"

#include "gfx/window.h"

#define FORMATION_VELOCITY  ENEMY_IDLE_VELOCITY
#define FORMATION_DISTANCE  15.f

#define FORMATION_WIDTH     (SCREEN_WIDTH - (SCREEN_WIDTH / 4.f))
#define FORMATION_HEIGHT    (SCREEN_HEIGHT / 4.f)

// maximum entities per row
#define FORMATION_ROW_MAX   ((FORMATION_WIDTH / (ENEMY_WIDTH + FORMATION_DISTANCE)) - 1)

// maximum entities per column
#define FORMATION_COL_MAX   ((FORMATION_HEIGHT / (ENEMY_HEIGHT + FORMATION_DISTANCE)) - 1)

#define FORMATION_SPAWN_X   ((SCREEN_WIDTH - FORMATION_WIDTH) / 2.f)
#define FORMATION_SPAWN_Y   (SCREEN_HEIGHT - (FORMATION_HEIGHT / 2.f) - (SCREEN_HEIGHT / 4.f))

void formation_init(Entity *, struct World *);
void formation_update(Entity *, struct World *);

#endif

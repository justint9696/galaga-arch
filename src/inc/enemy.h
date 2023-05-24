#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "entity.h"
#include "path.h"
#include "queue.h"
#include "window.h"

#define ENEMY_TEXTURE       "assets/images/enemy01.png"
#define ENEMY_START_STATE   STATE_SPAWN
#define ENEMY_SPAWN_HEALTH  1.f
#define ENEMY_WIDTH         66.67f
#define ENEMY_HEIGHT        101.3f
#define ENEMY_SPAWN_X       ((WINDOW_WIDTH - ENEMY_WIDTH) / 2)
#define ENEMY_SPAWN_Y       (WINDOW_HEIGHT - ENEMY_HEIGHT - 50.f)
#define ENEMY_VELOCITY      0.25f

typedef enum {
    STATE_IDLE,
    STATE_SPAWN,
    STATE_TRAVEL,
    STATE_ATTACK,
} estate_t;

typedef struct {
    estate_t state;
    Queue path;
    Entity *ent;
} Enemy;

void Enemy_InitAll(uint64_t tick);

void Enemy_UpdateAll(uint64_t tick);

#endif

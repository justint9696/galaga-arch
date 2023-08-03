#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "data/queue.h"
#include "gfx/window.h"

#include "entity/entity.h"
#include "path.h"

#include <stdint.h>

#define ENEMY_TEXTURE           "assets/images/enemy01.png"
#define ENEMY_SPAWN_HEALTH      1.f
#define ENEMY_WIDTH             66.67f
#define ENEMY_HEIGHT            101.3f
#define ENEMY_SPAWN_X           ((WINDOW_WIDTH - ENEMY_WIDTH) / 2)
#define ENEMY_SPAWN_Y           (WINDOW_HEIGHT - ENEMY_HEIGHT - 50.f)
#define ENEMY_IDLE_VELOCITY     0.2f
#define ENEMY_SPAWN_VELOCITY    0.3f
#define ENEMY_IDLE_TIME         1500    

typedef enum {
    STATE_IDLE,
    STATE_SPAWN,
    STATE_TRAVEL,
    STATE_ATTACK,
} estate_t;

typedef struct {
    estate_t state;
    int64_t idle_tick;
    Queue path;
    Entity *entity;
} Enemy;

void Enemy_InitAll(uint64_t tick, uint32_t count);
void Enemy_Free(Enemy *self);
void Enemy_UpdateAll(uint64_t tick);

#endif

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "data/queue.h"

#include "gfx/window.h"

#include "game/world.h"

#include "entity/entity.h"
#include "entity/logic/path.h"

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

#define MAX_ENEMY               32

typedef enum {
    WAVE_ONE,
    WAVE_TWO,
    WAVE_THREE,
    // WAVE_FOUR,
    WAVE_COMPLETE,
} ewave_t;

typedef enum {
    STATE_IDLE,
    STATE_SPAWN,
    STATE_TRAVEL,
    STATE_ATTACK,
} estate_t;

typedef enum {
    FORMATION_ONE,
    FORMATION_TWO,
    FORMATION_THREE,
    FORMATION_FOUR,
} eformation_t;

typedef struct {
    ewave_t wave;
    estate_t state;
    int64_t idle_tick;
    Queue path;
    Entity entity;
} Enemy;

bool Enemy_IsAlive(const Enemy *self);
void Enemy_Init(Enemy *self, ewave_t wave, eformation_t formation, uint64_t tick);
Entity *Enemy_Update(Enemy *self, const Player *player, uint64_t tick);

#endif

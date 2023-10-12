#ifndef _WAVE_H_
#define _WAVE_H_

#include "entity/enemy.h"
#include "game/world.h"

#include <stdint.h>

#define MAX_WAVE        WAVE_COMPLETE
#define WAVE_COUNT      2
#define SPAWN_TIME      1000

typedef struct {
    uint32_t count;
    Enemy *tail;
    ewave_t current;
    eformation_t formation;
} Wave;

void Wave_Init(Wave *self);
void Wave_Update(Wave *self, World *world, Enemy *enemies, uint64_t tick);

#endif

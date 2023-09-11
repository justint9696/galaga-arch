#ifndef _WAVE_H_
#define _WAVE_H_

#include "data/linked_list.h"
#include "entity/logic/enemy.h"

#include <stdbool.h>
#include <stdint.h>

#define MAX_WAVE        WAVE_COMPLETE

typedef struct {
    uint32_t count;
    eformation_t formation;
    Enemy *tail;
    ewave_t current;
} Wave;

void Wave_Init(Wave *self);
void Wave_Update(Wave *self, Enemy *enemies, LinkedList *entities, uint64_t tick);

#endif

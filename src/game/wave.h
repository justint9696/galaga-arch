#ifndef _WAVE_H_
#define _WAVE_H_

#include "data/linked_list.h"
#include "entity/logic/enemy.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    Enemy *tail;
    ewave_t current;
} Wave;

void Wave_Init(Wave *self);
void Wave_Update(Wave *self, LinkedList *enemies, LinkedList *entities, uint64_t tick, eformation_t formation);

#endif

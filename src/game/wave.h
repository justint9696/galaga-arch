#ifndef _WAVE_H_
#define _WAVE_H_

#include "data/linked_list.h"
#include "entity/ai/enemy.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    Enemy *tail;
    ewave_t current;
} wave_s;

void Wave_Init();
void Wave_Update(uint64_t tick, eformation_t formation);

#endif

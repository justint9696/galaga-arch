#ifndef _STAGE_H_
#define _STAGE_H_

#include "data/queue.h"
#include "entity/logic/enemy.h"

typedef struct {
    uint32_t id;
    float scalar;
    eformation_t formation;
    ewave_t wave;
    uint64_t tick;
    uint32_t enemies;
} stage_s;

void Stage_Init(uint64_t tick);
void Stage_Update(uint64_t tick);
void Stage_Clear();
uint32_t Stage_Next(uint64_t tick);
bool Stage_Complete();

#endif

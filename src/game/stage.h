#ifndef _STAGE_H_
#define _STAGE_H_

#include "data/queue.h"
#include "entity/ai/enemy.h"

#define MAX_ENEMY       1

typedef struct {
    uint32_t id;
    float scalar;
    eformation_t formation;
    ewave_t wave;
    uint64_t tick;
    struct {
        uint32_t total, remaining;
    } enemies;
} stage_s;

void Stage_Init(uint64_t tick);
void Stage_Update(uint64_t tick);
void Stage_Clear();
uint32_t Stage_Next(uint64_t tick);
bool Stage_Complete();

#endif

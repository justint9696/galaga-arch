#ifndef _STAGE_H_
#define _STAGE_H_

#include "data/queue.h"
#include "entity/logic/enemy.h"
#include "game/world.h"
#include "game/wave.h"

typedef struct {
    uint32_t id, count;
    float scalar;
    Wave wave;
    LinkedList enemies;
} Stage;

void Stage_Init(Stage *self, uint64_t tick);
void Stage_Destroy(Stage *self);
void Stage_Update(Stage *self, World *world, uint64_t tick);
void Stage_Clear(Stage *self);
uint32_t Stage_Next(Stage *self, uint64_t tick);
bool Stage_Complete(Stage *self);

#endif

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
    Enemy enemies[MAX_ENEMY];
} Stage;

void Stage_Init(Stage *self, uint32_t id);
void Stage_Update(Stage *self, World *world, uint64_t tick);
void Stage_Next(Stage *self);
bool Stage_Complete(Stage *self);

#endif

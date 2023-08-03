#include "entity/ai/enemy.h"
#include "stage.h"
#include "time.h"

#include <string.h>

static stage_s _stage;

void Stage_Init() {
    memset(&_stage, 0, sizeof(stage_s));
    _stage.enemies.total = MAX_ENEMY;
    _stage.enemies.remaining = _stage.enemies.total;
}

void Stage_Set() {
    uint64_t tick = Time_GetTick();
    // decide formation based on even or odd stage id
    if (_stage.id % 2 == 0)
        _stage.formation = FORMATION_DOUBLE;
    else
        _stage.formation = FORMATION_SINGLE;

    // decide speed scalar based on stage id 
    Enemy_InitAll(tick, _stage.enemies.total);
}

void Stage_Clear() {
    // TODO: clear out enemies and entities (if needed) 
}

uint32_t Stage_Next() {
    return ++_stage.id;
}

bool Stage_Complete() {
    return _stage.enemies.remaining == 0;
}

uint32_t Stage_Current() {
    return _stage.id;
}

uint32_t Stage_Remaining() {
    return _stage.enemies.remaining;
}

float Stage_Scalar() {
    return _stage.scalar;  
} 
 
formation_t Stage_Formation() {
    return _stage.formation;
}

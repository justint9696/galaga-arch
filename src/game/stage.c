#include "stage.h"
#include "entity/ai/enemy.h"
#include "time.h"
#include "wave.h"

#include <string.h>

static stage_s _stage;

static inline uint32_t _Stage_Current() {
    return _stage.id;
}

static inline uint32_t _Stage_Remaining() {
    return _stage.enemies.remaining;
}

static inline float _Stage_Scalar() {
    return _stage.scalar;  
} 

static inline eformation_t _Stage_Formation() {
    return _stage.formation;
}

static void _Stage_Set(uint64_t tick) {
    uint32_t id = _Stage_Current(); 
    if (id % 4 == 0)
        _stage.formation = FORMATION_FOUR;
    else if (id % 3 == 0)
        _stage.formation = FORMATION_THREE;
    else if (id % 2 == 0)
        _stage.formation = FORMATION_TWO;
    else 
        _stage.formation = FORMATION_ONE;
}

void Stage_Init(uint64_t tick) {
    memset(&_stage, 0, sizeof(stage_s));

    _stage.enemies.total = MAX_ENEMY;
    _stage.enemies.remaining = _stage.enemies.total;
    _stage.wave = WAVE_ONE;
    _stage.tick = tick;

    _Stage_Set(tick);
}

void Stage_Update(uint64_t tick) {
    switch (_stage.wave) {
        case WAVE_COMPLETE:
            break;
        default:
            Wave_Update(tick, _stage.formation);
            break;
    }

    int count = Enemy_UpdateAll(tick);
    _stage.enemies.remaining = count;
}

void Stage_Clear() {
    // TODO: clear out enemies and entities (if needed) 
}

uint32_t Stage_Next(uint64_t tick) {
    _stage.id++;

    Stage_Clear();
    _Stage_Set(tick);

    return _stage.id;
}

bool Stage_Complete() {
    return _stage.enemies.remaining == 0;
}

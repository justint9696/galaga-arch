#include "entity/logic/enemy.h"
#include "game/stage.h"
#include "game/time.h"
#include "game/wave.h"

#include <string.h>

static stage_s _stage;

static inline uint32_t _Stage_Current() {
    return _stage.id;
}

static inline uint32_t _Stage_Remaining() {
    return _stage.enemies;
}

static inline float _Stage_Scalar() {
    return _stage.scalar;  
} 

static inline eformation_t _Stage_Formation() {
    return _stage.formation;
}

static void _Stage_Set() {
    uint32_t id = _Stage_Current(); 
    if (id % 4 == 0)
        _stage.formation = FORMATION_ONE;
    else if (id % 3 == 0)
        _stage.formation = FORMATION_THREE;
    else if (id % 2 == 0)
        _stage.formation = FORMATION_TWO;
    else 
        _stage.formation = FORMATION_FOUR;
}

void Stage_Init(uint64_t tick) {
    memset(&_stage, 0, sizeof(stage_s));

    _stage.enemies = 1;
    _stage.tick = tick;

    Wave_Init();
    Enemy_InitAll();
    _Stage_Set();
}

void Stage_Update(uint64_t tick) {
    switch (_stage.wave) {
        case WAVE_COMPLETE:
            break;
        default:
            _stage.wave = Wave_Update(tick, _stage.formation);
            break;
    }

    int count = Enemy_UpdateAll(tick);
    _stage.enemies = count;
}

void Stage_Clear() {
    // TODO: clear out enemies and entities (if needed) 
}

uint32_t Stage_Next(uint64_t tick) {
    _stage.id++;

    Stage_Clear();
    _Stage_Set();

    return _stage.id;
}

bool Stage_Complete() {
    return _stage.enemies == 0;
}

#include "entity/logic/enemy.h"
#include "game/stage.h"
#include "game/time.h"
#include "game/wave.h"
#include "gfx/hud.h"

#include <string.h>

static inline float _Stage_Scalar(Stage *self) {
    return self->scalar;  
} 

static inline eformation_t _Stage_Formation(Stage *self) {
    return self->formation;
}

static void _Stage_Set(Stage *self) {
    uint32_t id = self->id;
    if (id % 4 == 0)
        self->formation = FORMATION_ONE;
    else if (id % 3 == 0)
        self->formation = FORMATION_THREE;
    else if (id % 2 == 0)
        self->formation = FORMATION_TWO;
    else 
        self->formation = FORMATION_FOUR;
}

void Stage_Init(Stage *self, uint64_t tick) {
    memset(self, 0, sizeof(Stage));

    self->tick = tick;

    Wave_Init(&self->wave);
    _Stage_Set(self);
}

void Stage_Update(Stage *self, World *world, uint64_t tick) {
    switch (self->wave.current) {
        case WAVE_COMPLETE:
            break;
        default:
            Wave_Update(&self->wave, &self->enemies, &world->entities, tick, self->formation);
            break;
    }

    Hud_AddText("Wave: %i", self->wave.current);
    self->count = Enemy_UpdateAll(&self->enemies, world, tick);
}

void Stage_Clear(Stage *self) {
    // TODO: clear out enemies and entities (if needed) 
}

uint32_t Stage_Next(Stage *self, uint64_t tick) {
    self->id++;

    Stage_Clear(self);
    _Stage_Set(self);

    return self->id;
}

bool Stage_Complete(Stage *self) {
    return self->count == 0;
}

#include "common/util.h"
#include "entity/logic/enemy.h"
#include "game/stage.h"
#include "game/time.h"
#include "game/wave.h"
#include "gfx/hud.h"

#include <assert.h>
#include <string.h>

static void _Stage_Set(Stage *self) {
    uint32_t id = self->id;
    if (id % 4 == 0)
        self->wave.formation = FORMATION_ONE;
    else if (id % 3 == 0)
        self->wave.formation = FORMATION_THREE;
    else if (id % 2 == 0)
        self->wave.formation = FORMATION_TWO;
    else 
        self->wave.formation = FORMATION_FOUR;
}

void Stage_Init(Stage *self, uint64_t tick) {
    memset(self, 0, sizeof(Stage));

    Wave_Init(&self->wave);
    _Stage_Set(self);
}

void Stage_Destroy(Stage *self) {
    LinkedList_Free(&self->enemies); 
    memset(self, 0, sizeof(Stage));
}

static void _Stage_UpdateAxis(Stage *self, World *world, uint64_t tick) {
    uint32_t count = 0;
    Enemy *enemy, *next;
    Node *tmp = self->enemies.head;

    while (tmp) {
        enemy = (Enemy *)tmp->item;
        assert(enemy);

        tmp = tmp->next;

        if (!Entity_IsAlive(&enemy->entity)) {
            LinkedList_Remove(&self->enemies, enemy);
            free(enemy);
            continue;
        }

        count++;
        if (tmp && enemy->state == STATE_SPAWN) {
            next = (Enemy *)tmp->item;
            if (!next || distance(enemy->entity.pos, next->entity.pos) < 100.f) 
                continue;
        }

        Enemy_Update(enemy, world, tick);
    }

    self->count = count;
}

void Stage_Update(Stage *self, World *world, uint64_t tick) {
    switch (self->wave.current) {
        case WAVE_COMPLETE:
            break;
        default:
            Wave_Update(&self->wave, &world->entities, &self->enemies, tick);
            break;
    }

    Hud_AddText("Wave: %i", self->wave.current);
    Hud_AddText("Enemies: %i", self->count);

    _Stage_UpdateAxis(self, world, tick);
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

#include "common/util.h"
#include "data/linked_list.h"
#include "game/stage.h"
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

void Stage_Init(Stage *self, uint32_t id) {
    memset(self, 0, sizeof(Stage));
    self->id = id;

    Wave_Init(&self->wave);
    _Stage_Set(self);
}

void Stage_Destroy(Stage *self) {
    Queue *q;
    for (size_t i = 0; i < MAX_ENEMY; i++) {
        q = &self->enemies[i].path;
        for (size_t j = 0; j < q->size; j++) {
            free(queue_front(q));
            dequeue(q);
        }
    }
}

static void _Stage_Update(Stage *self, World *world, uint64_t tick) {
    Entity *child;
    uint32_t count = 0;
    Enemy *enemy, *next;
    for (int i = 0; i < self->wave.count; i++) {
        enemy = &self->enemies[i];
        
        if (!Enemy_IsAlive(enemy))
            continue;

        ++count;
        if (i < MAX_ENEMY - 1 && enemy->state == STATE_SPAWN) {
            next = &self->enemies[i+1]; 
            if (distance(enemy->entity.pos, next->entity.pos) < 125.f)
                continue;
        }

        child = Enemy_Update(enemy, world, tick);
        if (child) 
            LinkedList_Add(&world->entities, child);
    }

    self->count = count;
}

void Stage_Update(Stage *self, World *world, uint64_t tick) {
    switch (self->wave.current) {
        case MAX_WAVE:
            if (Stage_Complete(self))
                Stage_Next(self);
            break;
        default:
            Wave_Update(&self->wave, world, self->enemies, tick);
            break;
    }

    // Hud_AddText("Stage: %i", self->id);
    // Hud_AddText("Wave: %i", self->wave.current);
    Hud_AddText("Enemies: %i", self->count);

    _Stage_Update(self, world, tick);
}

void Stage_Next(Stage *self) {
    Stage_Init(self, ++self->id);
}

bool Stage_Complete(Stage *self) {
    return self->count == 0;
}
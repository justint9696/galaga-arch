#include "game/wave.h"
#include "game/time.h"

static inline bool _Wave_Complete(Wave *self) {
    if ((self->current < MAX_WAVE && !self->count) || !self->tail || !self->tail->path.size)
        return true;

    // if tail not found, spawn after SPAWN_TIME ms
    Enemy *enemy = self->tail;
    if (!Enemy_IsAlive(enemy) && Time_Passed(enemy->entity.tick) > SPAWN_TIME)
        return true;

    path_s *path = (path_s *)queue_rear(&enemy->path);
    return self->current && enemy->path.size == 1 && path->complete;
}

static void _Wave_Spawn(Wave *self, World *world, Enemy *enemies, uint32_t count, uint64_t tick) {
    Enemy *enemy;
    for (int i = 0; i < count; i++) {
        enemy = &enemies[self->count];
        Enemy_Init(enemy, self->count++, self->current, tick);
        LinkedList_Add(&world->entities, &enemy->entity);
    }
}

inline void Wave_Init(Wave *self) {
    memset(self, 0, sizeof(Wave));
}

void Wave_Update(Wave *self, World *world, Enemy *enemies, uint64_t tick) {
    if (!_Wave_Complete(self))
        return;

    switch (self->current) {
        case MAX_WAVE:
            break;
        default:
            _Wave_Spawn(self, world, enemies, WAVE_COUNT, tick);
            self->tail = &enemies[self->count - 1];
            ++self->current;
            break;
    }
}

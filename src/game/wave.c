#include "game/wave.h"

#include <assert.h>

static inline bool _Wave_Complete(Wave *self) {
    if ((self->current < MAX_WAVE && !self->count) || !self->tail || !self->tail->path.size)
        return true;

    path_s *path = (path_s *)queue_rear(&self->tail->path);
    return self->current && self->tail->path.size == 1 && path->complete;
}

static void _Wave_Spawn(Wave *self, Enemy *enemies, LinkedList *entities, uint32_t count, uint64_t tick) {
    Enemy *enemy;
    for (int i = 0; i < count; i++) {
        enemy = &enemies[self->count++];
        Enemy_Init(enemy, self->current, self->formation, tick);
        LinkedList_Add(entities, &enemy->entity);
    }
}

inline void Wave_Init(Wave *self) {
    memset(self, 0, sizeof(Wave));
}

void Wave_Update(Wave *self, Enemy *enemies, LinkedList *entities, uint64_t tick) {
    if (!_Wave_Complete(self))
        return;

    switch (self->current) {
        case MAX_WAVE:
            break;
        default:
            _Wave_Spawn(self, enemies, entities, 1, tick);
            self->tail = &enemies[self->count - 1];
            ++self->current;
            break;
    }
}

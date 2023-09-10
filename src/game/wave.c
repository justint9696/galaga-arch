#include "game/wave.h"

#include <assert.h>

static inline bool _Wave_Complete(Wave *self) {
    if ((self->current < MAX_WAVE && !self->count) || !self->tail || !self->tail->path.size)
        return true;

    path_s *path = (path_s *)queue_rear(&self->tail->path);
    return self->current && self->tail->path.size == 1 && path->complete;
}

static void _Wave_Spawn(Wave *self, LinkedList *entities, LinkedList *enemies, uint32_t count, uint64_t tick) {
    Enemy *enemy;
    for (int i = 0; i < count; i++, self->count++) {
        enemy = Enemy_Init(self->current, self->formation, tick);
        LinkedList_Add(enemies, enemy);
        LinkedList_Add(entities, &enemy->entity);
    }
}

inline void Wave_Init(Wave *self) {
    memset(self, 0, sizeof(Wave));
}

void Wave_Update(Wave *self, LinkedList *entities, LinkedList *enemies, uint64_t tick) {
    if (!_Wave_Complete(self))
        return;

    switch (self->current) {
        case MAX_WAVE:
            break;
        default:
            _Wave_Spawn(self, entities, enemies, 1, tick);
            self->tail = (Enemy *)enemies->tail->item;
            ++self->current;
            break;
    }
    
}

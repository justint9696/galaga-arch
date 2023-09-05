#include "game/wave.h"

static inline bool _Wave_Complete(Wave *self) {
    if (!self->tail || !self->tail->path.size)
        return true;

    path_s *path = (path_s *)queue_rear(&self->tail->path);
    return self->current && self->tail->path.size == 1 && path->complete;
}

inline void Wave_Init(Wave *self) {
    memset(self, 0, sizeof(Wave));
}

void Wave_Update(Wave *self, LinkedList *enemies, LinkedList *entities, uint64_t tick, eformation_t formation) {
    if (!_Wave_Complete(self))
        return;

    switch (self->current) {
        case WAVE_COMPLETE:
            break;
        default:
            Enemy_SpawnWave(enemies, entities, tick, 1, self->current, formation);
            self->tail = (Enemy *)enemies->tail->item;
            ++self->current;
            break;
    }
    
}

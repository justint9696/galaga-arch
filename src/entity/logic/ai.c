#include "game/world.h"
#include "entity/logic/ai.h"
#include "entity/logic/enemy.h"

void entity_do_ai(Entity *self, World *world) {
    switch (self->type) {
        case E_ABDUCTOR:
        case E_INVADER:
            enemy_ai(self, world);
            break;
        default:
            break;
    }
}

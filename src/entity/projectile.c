#include "entity/projectile.h"
#include "game/world.h"

void projectile_init(Entity *self, World *world) {
    self->dim = VEC2( PROJECTILE_WIDTH, PROJECTILE_HEIGHT);
    self->color = COLOR_GREEN;
    self->health = 1.f;
    self->depth = DEPTH_PROJECTILE;
    entity_set_flag(self, FLAG_COLLISION);
    entity_set_state(self, STATE_ALIVE);
}

void projectile_collide(Entity *self, Entity *entity, World *world) {
    switch (entity->type) {
        case E_STAR:
        case E_FORMATION:
        case E_PROJECTILE:
        case E_TRACTOR_BEAM:
            break;
        default: 
            entity_damage(entity);
            entity_set_state(self, STATE_DEAD);
            break;
    }
}

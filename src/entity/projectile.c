#include "entity/projectile.h"
#include "game/world.h"

void projectile_init(Entity *self, World *world) {
    self->dim = (vec2) {
        .width = PROJECTILE_WIDTH,
        .height = PROJECTILE_HEIGHT,
    };
    self->color = COLOR_GREEN;
    self->health = 1.f;
    self->state = STATE_ALIVE;
    self->flags = FLAG_COLLISION;
}

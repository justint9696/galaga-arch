#include "include/entity.h"

void initEntity(Entity *ent, float x, float y, char *texture) {
    ent->pos.x = x;
    ent->pos.y = y;

    ent->vel.x = 0.f;
    ent->vel.y = 0.f;
}

void SetPosition(Entity *ent, float x, float y) {
    ent->pos.x = x;
    ent->pos.y = y;
}

void SetVelocity(Entity *ent, float vx, float vy) {
    ent->vel.x = vx;
    ent->vel.y = vy;
}
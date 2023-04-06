#include "inc/path.h"

void Path_Circle(Entity *entity, vec2 dst, float speed) {

}

void Path_Linear(Entity *entity, vec2 dst, float speed) {
    vec2 vel = { 
        entity->pos.x == dst.x ? 0 : entity->pos.x < dst.x ? speed : -speed,
        entity->pos.y == dst.y ? 0 : entity->pos.y < dst.y ? speed : -speed
    };

    if (vel.x == entity->vel.x && vel.y == entity->vel.y) 
        return;

    Entity_SetVelocity(entity, vel);
}

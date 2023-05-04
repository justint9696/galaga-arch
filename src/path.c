#include "inc/path.h"

#include <stdio.h>

static inline float _AngleOffset(float dx, float dy) {
    if (dx < 0 && dy < 0) // Q3
        return 180.f;
    else if (dx > 0 && dy < 0) // Q4
        return 270.f;
    else if (dx < 0 && dy > 0) // Q2
        return 90.f;
    return 0.f; // Q1
}

inline void Path_Circular(Entity *entity, const vec2 org, const vec2 dst, float speed) {
        float
                dx = org.x - dst.x,
                   dy = org.y - dst.y;

    float 
        radius = sqrt(pow(dx, 2) + pow(dy, 2)),
        angle = DEG(acos(dx / radius)); 

    dx = entity->pos.x - dst.x;
    dy = entity->pos.y - dst.y;

    angle = 90.f + DEG(acos(dx / radius));

    vec2 pos = {
        cos(angle),
        sin(angle)
    };

    //pos.x *= speed;
    //pos.y *= speed;

    Entity_SetVelocity(entity, pos);

    printf("radius: %.2f\tangle: %.2f\tvelocity(%.2f, %.2f)\n", radius, angle, pos.x, pos.y);
}

inline void Path_Linear(Entity *entity, const vec2 org, const vec2 dst, float speed) {
    vec2 vel = { 
        entity->pos.x == dst.x ? 0 : entity->pos.x < dst.x ? speed : -speed,
        entity->pos.y == dst.y ? 0 : entity->pos.y < dst.y ? speed : -speed
    };

    if (vel.x == entity->vel.x && vel.y == entity->vel.y) 
        return;

    Entity_SetVelocity(entity, vel);
}

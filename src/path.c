#include "inc/path.h"

#include <stdio.h>

static inline vec2 _midpoint(vec2 pt0, vec2 pt1) {
    vec2 midpoint = {
        (pt0.x + pt1.x) / 2.f,
        (pt0.y + pt1.y) / 2.f
    };

    return midpoint;
}

static inline float _radius(vec2 point, vec2 midpoint) {
    return sqrtf(powf(point.x - midpoint.x, 2) + powf(point.y - midpoint.y, 2));
}

static inline float _angle(vec2 point, vec2 midpoint) {
    float 
        dx = point.x - midpoint.x,
        dy = point.y - midpoint.y;

    if (dx > 0 && dy > 0) 
        return DEG(atan(dy / dx)); 
    else if (dx < 0 && dy > 0)
        return DEG(atan(dy / dx)) + 90.f;
    else if (dx < 0 && dy < 0) 
        return DEG(atan(dy / dx)) + 180.f;
    return DEG(atan(dy / dx)) + 270.f;
}

inline void Path_Circular(Entity *entity, vec2 org, vec2 dst, float speed) {
    vec2 midpoint = _midpoint(org, dst);
    float radius = _radius(org, midpoint);
    float angle = _angle(entity->pos, midpoint) + speed;
    
    vec2 vel = {
        midpoint.x + (radius * cos(RAD(angle))) - entity->pos.x,
        midpoint.y + (radius * sin(RAD(angle))) - entity->pos.y
    };

    Entity_SetVelocity(entity, vel);
}

inline void Path_Linear(Entity *entity, vec2 org, vec2 dst, float speed) {
    vec2 vel = { 
        entity->pos.x == dst.x ? 0 : entity->pos.x < dst.x ? speed : -speed,
        entity->pos.y == dst.y ? 0 : entity->pos.y < dst.y ? speed : -speed
    };

    if (vel.x == entity->vel.x && vel.y == entity->vel.y) 
        return;

    Entity_SetVelocity(entity, vel);
}

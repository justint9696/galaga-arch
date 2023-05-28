#include "inc/path.h"
#include "inc/util.h"

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

/*
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
}*/

static inline float _angle(vec2 point, vec2 midpoint) {
    float 
        dx = point.x - midpoint.x,
        dy = point.y - midpoint.y;

    if (dx > 0 && dy > 0) 
        return atan(dy / dx); 
    else if (dx < 0 && dy > 0)
        return atan(dy / dx) + (M_PI / 2.f);
    else if (dx < 0 && dy < 0) 
        return atan(dy / dx) + M_PI;
    return atan(dy / dx) + ((3 * M_PI) / 2.f);
}

static inline float _bezier_quad(float p0, float p1, float p2, float t) {
    return p1 + powf((1 - t), 2) * (p0 - p1) + powf(t, 2) * (p2 - p1);
}

static inline vec2 _bezier_path(vec2 src, vec2 mid, vec2 dst, float time) {
    return (vec2) { _bezier_quad(src.x, mid.x, dst.x, time), _bezier_quad(src.y, mid.y, dst.y, time) };
}

inline path_s *Path_Init() {
    path_s *path = (path_s *)calloc(1, sizeof(path_s));
    return path;
}

inline void Path_Linear(Entity *entity, path_s *path) {
    const vec2 
        org = path->org,
        dst = path->dst;

    float 
        dx = dst.x - org.x,
        dy = dst.y - org.y;

    float angle = atan(dy / dx);

    float 
        sign_x = dx < 0 ? -1 : 1,
        sign_y = dy < 0 ? -1 : 1;

    vec2 vel = {
        sign_x * path->speed * cos(angle),
        sign_y * path->speed * sin(angle),
    };

    if (vel.x == entity->vel.x && vel.y == entity->vel.y) 
        return;

    Entity_SetVelocity(entity, vel);
}

inline void Path_Circular(Entity *entity, path_s *path) {
    const vec2 
        org = path->org,
        dst = path->dst;

    vec2 midpoint = _midpoint(org, dst);
    float radius = _radius(org, midpoint);
    if (!path->angle)
        path->angle = _angle(entity->pos, midpoint);
    path->angle += RAD(path->speed);

    vec2 pos = {
        (midpoint.x + (radius * cos(path->angle))),
        (midpoint.y + (radius * sin(path->angle)))
    };

    Entity_SetPosition(entity, pos);
}

inline void Path_Bezier(Entity *entity, path_s *path) {
    const vec2 
        org = path->org,
        dst = path->dst;

    float distance = Distance(org, dst);
    path->time += (1.f / (distance / path->speed));

    vec2 midpoint = { org.x, dst.y };
    vec2 pos = _bezier_path(org, midpoint, dst, path->time);

    Entity_SetPosition(entity, pos);
}

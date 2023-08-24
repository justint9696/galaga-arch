#include "gfx/hud.h"
#include "game/fps.h"
#include "common/util.h"

#include "entity/entity.h"
#include "entity/logic/path.h"

#include <stdio.h>

/**
 * Orients an entity based on the direction they are moving.
 * @param entity    entity to orient
 * @param path      path of entity
 * @param dir       direction of movement vector
 */
static inline void _orient(Entity *entity, path_s *path, float dir) {
    switch (path->orientation) {
        case ORIENT_UPRIGHT:
            Entity_SetRotation(entity, 0.f);
            break;
        case ORIENT_DESTINATION:
            if (memcmp(&path->angle, &dir, sizeof(float)))
                Entity_SetRotation(entity, 90.f - dir);
            break;
    }
}

/**
 * Calculates the midpoint between two coordinates.
 * @param p0    point 1
 * @param p1    point 2
 */
static inline vec2 _midpoint(vec2 pt0, vec2 pt1) {
    return (vec2) { (pt0.x + pt1.x) / 2.f, (pt0.y + pt1.y) / 2.f };
}

/**
 * Calculates the radius given the center and any point of a circle.
 * @param point     a point on a circle
 * @param midpoint  midpoint of a circle
 */
static inline float _radius(vec2 point, vec2 midpoint) {
    return sqrtf(powf(point.x - midpoint.x, 2) + powf(point.y - midpoint.y, 2));
}

/**
 *  Calculates the angle in degrees of a point on a circle in reference to its center.
 *  @param point        a point on a circle
 *  @param midpoint     midpoint of a circle
 */
static inline float _angle(vec2 point, vec2 midpoint) {
    float 
        dx = fabs(midpoint.x - point.x),
        dy = fabs(midpoint.y - point.y);

    // offset based on the quadrant in reference to the midpoint.
    float angle = DEG(atan2(dy, dx));
    if (point.x >= midpoint.x && point.y >= midpoint.y)
        return angle; 
    else if (point.x <= midpoint.x && point.y > midpoint.y)
        return angle + DEG(M_PI / 2.f);
    else if (point.x <= midpoint.x && point.y <= midpoint.y)
        return angle + DEG(M_PI);
    return angle + DEG((3 * M_PI) / 2.f);  // quadrant 4
}

/**
 * Returns the length of an arch given two points on a circle.
 * @param point1    a point on a circle
 * @param point2    a point on a circle
 */
static inline float _arc_length(vec2 point1, vec2 point2) {
    const vec2 midpoint = _midpoint(point1, point2);
    const float radius = _radius(point1, midpoint);
    const float angle1 = _angle(point1, midpoint);
    const float angle2 = _angle(point2, midpoint);
    const float d_angle = fabs(angle1 - angle2);

    return (PI_2 * radius) * (d_angle / 360.f);
}

/**
 * Calculates a point on a quadratic bezier curve at time t.
 * @param p0    starting point
 * @param p1    midpoint
 * @param p2    destination point
 * @param t     time interval 
 */
static inline float _bezier_quad(float p0, float p1, float p2, float t) {
    return p1 + powf((1 - t), 2) * (p0 - p1) + powf(t, 2) * (p2 - p1);
}

/**
 * Returns a vector containing x and y coordinates of a bezier curve at time t.
 * @param src       starting coordinates
 * @param mid       midpoint coordinates
 * @param dst       destination coordinates
 */
static inline vec2 _bezier_path(vec2 src, vec2 mid, vec2 dst, float time) {
    return (vec2) { _bezier_quad(src.x, mid.x, dst.x, time), _bezier_quad(src.y, mid.y, dst.y, time) };
}

/**
 * Determines a midpoint on a bezier curve.
 * @param org       starting coordinates
 * @param dst       desination coordinates
 */
static inline vec2 _bezier_midpoint(vec2 org, vec2 dst, float speed) {
    return (speed > 0.f ? (vec2) { dst.x, org.y } : (vec2) { org.x, dst.y });
}

inline path_s *Path_Init() {
    return (path_s *)calloc(1, sizeof(path_s));
}

inline void Path_Linear(Entity *entity, path_s *path) {
    const vec2 
        org = path->org,
        dst = path->dst;

    float 
        dx = dst.x - org.x,
        dy = dst.y - org.y;

    float angle = atan2(dy, dx);

    vec2 vel = {
        fabs(path->speed) * cos(angle),
        fabs(path->speed) * sin(angle),
    };

    vec2 pos = { 
        entity->pos.x + vel.x, 
        entity->pos.y + vel.y 
    };

    switch (path->state) {
        case STATE_INACTIVE: 
            path->state = STATE_ONGOING;
            break;
        default:
            break;
    }

    path->complete = closer(entity->pos, pos, dst);

    if (path->complete) 
        return;

    if (!memcmp(&entity->vel, &vel, sizeof(vec2)))
        return;

    float dir = DEG(atan2(vel.y, vel.x));
    _orient(entity, path, dir);
    Entity_SetVelocity(entity, vel);

}

inline void Path_Circular(Entity *entity, path_s *path) {
    const vec2 
        org = path->org,
        dst = path->dst;

    vec2 midpoint = _midpoint(org, dst);
    float radius = _radius(org, midpoint);
    float distance = _arc_length(org, dst);

    switch (path->state) {
        case STATE_INACTIVE:
            path->angle = _angle(org, midpoint);
            path->state = STATE_ONGOING;
            break;
        default:
            break;
    }

    float circumference = (PI_2 * radius);
    float period = (distance / (path->speed / radius));
    float speed = DEG(circumference / period);

    path->angle += speed * (entity->deltaTime ? entity->deltaTime : 1.f);

    vec2 pos = {
        (midpoint.x + (radius * cos(RAD(path->angle)))),
        (midpoint.y + (radius * sin(RAD(path->angle))))
    };

    path->complete = (distance(pos, dst) < radius && closer(entity->pos, pos, dst));

    if (path->complete) 
        return;

    vec2 vel = {
        (pos.x - entity->pos.x),
        (pos.y - entity->pos.y)
    };

    float dir = DEG(atan2(vel.y, vel.x));
    _orient(entity, path, dir);
    Entity_SetPosition(entity, pos);
}

inline void Path_Bezier(Entity *entity, path_s *path) {
    const vec2 
        org = path->org,
        dst = path->dst;

    // TODO: get actual distance and time estimate (if possible) but approximation works

    vec2 midpoint = _bezier_midpoint(org, dst, path->speed);
    float distance = distance(org, midpoint) + distance(midpoint, dst);
    vec2 pos = _bezier_path(org, midpoint, dst, path->time);

    float time = ((distance / fabs(path->speed)) / 1000.f);
    path->time += (1.f / (time * (entity->deltaTime ? entity->deltaTime : 1.f)));

    switch (path->state) {
        case STATE_INACTIVE: 
            path->state = STATE_ONGOING;
            break;
        default:
            break;
    }

    path->complete = closer(entity->pos, pos, dst);

    if (path->complete) 
        return;

    vec2 vel = {
        (pos.x - entity->pos.x),
        (pos.y - entity->pos.y)
    };

    float dir = DEG(atan2(vel.y, vel.x));
    _orient(entity, path, dir);
    Entity_SetPosition(entity, pos);
}

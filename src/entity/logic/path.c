#include "common/time.h"
#include "common/util.h"
#include "entity/logic/path.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*!
 * Orients an entity based on the direction they are moving.
 * @param entity    entity to orient
 * @param path      path of entity
 * @param vel       velocity of entity
 */
static inline void orient(Path *path, Entity *entity, vec2 vel) {
    float dir = DEG(atan2(vel.y, vel.x));
    switch (path->orientation) {
        case ORIENT_UPRIGHT:
            entity_set_rotation(entity, 0.f);
            break;
        case ORIENT_DESTINATION:
            if (memcmp(&path->angle, &dir, sizeof(float)))
                entity_set_rotation(entity, 90.f - dir);
            break;
    }
}

/*!
 * Calculates the midpoint of two coordinates.
 * @param p0    point 1
 * @param p1    point 2
 */
static inline vec2 midpoint(vec2 pt0, vec2 pt1) {
    return (vec2) { .x = (pt0.x + pt1.x) / 2.f, .y = (pt0.y + pt1.y) / 2.f };
}

/*!
 * Calculates the radius of a circle.
 * @param point     a point on a circle
 * @param midpoint  midpoint of a circle
 */
static inline float radius(vec2 point, vec2 midpoint) {
    return sqrtf(powf(point.x - midpoint.x, 2) + powf(point.y - midpoint.y, 2));
}

/*!
 *  Calculates the angle in degrees of a point on a circle in reference to its center.
 *  @param point        a point on a circle
 *  @param midpoint     midpoint of a circle
 */
static inline float angle(vec2 point, vec2 midpoint) {
    vec2 diff = { 
        .x = fabs(midpoint.x - point.x),
        .y = fabs(midpoint.y - point.y)
    };

    // offset based on the quadrant in reference to the midpoint.
    float angle = DEG(atan2(diff.y, diff.x));
    if (point.x >= midpoint.x && point.y >= midpoint.y)
        return angle; 
    else if (point.x <= midpoint.x && point.y > midpoint.y)
        return angle + DEG(M_PI / 2.f);
    else if (point.x <= midpoint.x && point.y <= midpoint.y)
        return angle + DEG(M_PI);
    return angle + DEG((3 * M_PI) / 2.f);  // quadrant 4
}

/*!
 * Calculates the arc length between two points on a circle.
 * @param point1    a point on a circle
 * @param point2    a point on a circle
 */
static inline float arc_length(vec2 point1, vec2 point2) {
    const vec2 mid = midpoint(point1, point2);
    const float rad = radius(point1, mid);
    const float angle1 = angle(point1, mid);
    const float angle2 = angle(point2, mid);
    const float d_angle = fabs(angle1 - angle2);

    return (PI_2 * rad) * (d_angle / 360.f);
}

/*!
 * Calculates the position of a quadratic bezier curve at time t.
 * @param p0    starting point
 * @param p1    midpoint
 * @param p2    destination point
 * @param t     time interval 
 */
static inline float bezier_quad(float p0, float p1, float p2, float t) {
    return p1 + powf((1 - t), 2) * (p0 - p1) + powf(t, 2) * (p2 - p1);
}

/*!
 * Returns a vector containing x and y coordinates of a bezier curve at time t.
 * @param src       starting coordinates
 * @param mid       midpoint coordinates
 * @param dst       destination coordinates
 */
static inline vec2 bezier_path(vec2 src, vec2 mid, vec2 dst, float time) {
    return (vec2) { .x = bezier_quad(src.x, mid.x, dst.x, time), .y = bezier_quad(src.y, mid.y, dst.y, time) };
}

/*!
 * Determines a midpoint of a bezier curve.
 * @param org       starting coordinates
 * @param dst       desination coordinates
 */
static inline vec2 bezier_midpoint(vec2 org, vec2 dst, float speed) {
    return (speed > 0.f ? (vec2) { .x = dst.x, .y = org.y } : (vec2) { .x = org.x, .y = dst.y });
}

/*!
 * Returns an approximated length of a quadratic bezier curve.
 * @param org       starting coordinates
 * @param mid       midpoint coordinates
 * @param dst       destination coordinates
 */
static inline float bezier_length(vec2 org, vec2 dst, float speed) {
    vec2 mid = bezier_midpoint(org, dst, speed);
    return sqrtf(powf(distance(org, mid), 2) + powf(distance(mid, dst), 2));
}


static void path_linear(Path *self, Entity *entity) {
    vec2 org = self->org, dst = self->dst;

    float dx = dst.x - org.x, dy = dst.y - org.y;

    float angle = atan2(dy, dx);
    vec2 vel = (vec2) {
        .x = self->speed * cos(angle),
            .y = self->speed * sin(angle),
    };

    vec2 pos = { 
        .x = entity->pos.x + vel.x, 
        .y = entity->pos.y + vel.y,
    };

    switch (self->state) {
        case STATE_INACTIVE: 
            self->state = STATE_ONGOING;
            break;
        default:
            break;
    }

    self->complete = closer(entity->pos, pos, dst);
    if (self->complete) 
        return;

    if (!memcmp(&entity->vel, &vel, sizeof(vec2)))
        return;

    orient(self, entity, vel);
    entity_set_velocity(entity, vel);

}

static void path_circular(Path *self, Entity *entity) {
    vec2 org = self->org, dst = self->dst;

    vec2 mid = midpoint(org, dst);
    float rad = radius(org, mid);
    float distance = arc_length(org, dst);

    switch (self->state) {
        case STATE_INACTIVE:
            self->angle = angle(org, mid);
            self->state = STATE_ONGOING;
            break;
        default:
            break;
    }

    float circumference = (PI_2 * rad);
    float period = (distance / (self->speed / rad));
    float speed = DEG(circumference / period);

    uint32_t delta = time_delta();
    self->angle += speed * (delta ? delta : 1.f);

    vec2 pos = {
        .x = (mid.x + (rad * cos(RAD(self->angle)))),
        .y = (mid.y + (rad * sin(RAD(self->angle))))
    };

    self->complete = (distance(pos, dst) < rad && closer(entity->pos, pos, dst));
    if (self->complete) 
        return;

    vec2 vel = {
        .x = (pos.x - entity->pos.x),
        .y = (pos.y - entity->pos.y)
    };

    orient(self, entity, vel);
    entity_set_position(entity, pos);
}

static void path_bezier(Path *self, Entity *entity) {
    vec2 org = self->org, dst = self->dst;

    uint32_t now = NOW();
    switch (self->state) {
        case STATE_INACTIVE: 
            self->time = now;
            self->state = STATE_ONGOING;
            break;
        default:
            break;
    }

    // TODO: get actual distance and time estimate (if possible) but approximation works

    float distance = bezier_length(org, dst, self->speed);
    float time = (distance / fabs(self->speed)) / 2.f;

    vec2 mid = bezier_midpoint(org, dst, self->speed);

    float t = ((now - self->time) / time);
    vec2 pos = bezier_path(org, mid, dst, t);

    self->complete = (t >= 1.f);
    if (self->complete) 
        return;

    vec2 vel = {
        .x = (pos.x - entity->pos.x),
        .y = (pos.y - entity->pos.y)
    };

    orient(self, entity, vel);
    entity_set_position(entity, pos);
}

Path *path_init() {
    Path *self = calloc(1, sizeof(Path));
    assert(self);
    return self;
}

void path_update(Path *self, Entity *entity) {
    switch (self->type) {
        case PATH_LINEAR:
            path_linear(self, entity);
            break;
        case PATH_CIRCULAR:
            path_circular(self, entity);
            break;
        case PATH_BEZIER:
            path_bezier(self, entity);
            break;
    }
}

float path_distance(const Path *);
float path_time(const Path *);

#include "data/queue.h"

#include "entity/logic/route.h"
#include "entity/logic/enemy.h"

#include "gfx/window.h"

#include <assert.h>
#include <string.h>

static inline vec2 rear(Queue *q) {
    return (*(Path *)(queue_rear(q))).dst;
}

void route_start(Queue *q, vec2 org, vec2 dst, float speed, ptype_t type) {
    Path *path = path_init();
    path->org = org;
    path->dst = dst;
    path->type = type;
    path->speed = speed;
    enqueue(q, path);
}

void route_append(Queue *q, vec2 dst, float speed, ptype_t type) {
    route_start(q, rear(q), dst, speed, type); 
}

static void route_spawn_left(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    route_start(q, org, (vec2) { .x = radius, .y = ENEMY_SPAWN_Y - 100.f }, speed, PATH_CIRCULAR);
    route_append(q, (vec2) { .x = rear(q).x, .y = rear(q).y - radius }, speed, PATH_CIRCULAR);
    // route_append(q, (vec2) { .x = ENEMY_SPAWN_X, .y = ENEMY_SPAWN_Y }, speed, PATH_BEZIER);
}

static void route_spawn_right(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    route_start(q, org, (vec2) { .x = org.x - radius - ENEMY_WIDTH, .y = ENEMY_SPAWN_Y - 100.f }, -speed, PATH_CIRCULAR);
    route_append(q, (vec2) { .x = rear(q).x, .y = rear(q).y - radius }, -speed, PATH_CIRCULAR);
    // route_append(q, (vec2) { .x = ENEMY_SPAWN_X, .y = ENEMY_SPAWN_Y }, speed, PATH_BEZIER);
}

static void route_spawn_center(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    float half = (SCREEN_WIDTH / 2.f);
    route_start(q, org, (vec2) { .x = org.x - half + (radius * 2), .y = org.y - (radius * 6.f) }, -speed, PATH_BEZIER);
    route_append(q, (vec2) { .x = rear(q).x, .y = rear(q).y - 3 * radius }, speed, PATH_CIRCULAR);
    // route_append(q, (vec2) { .x = ENEMY_SPAWN_X, .y = ENEMY_SPAWN_Y }, speed, PATH_BEZIER);
}

void route_spawn(Entity *entity, float speed) {
    vec2 org = entity->pos;
    Queue *q = &entity->path;

    if (org.x <= 0.f) 
        route_spawn_left(q, org, speed);
    else if (org.x >= SCREEN_WIDTH)
        route_spawn_right(q, org, speed);
    else
        route_spawn_center(q, org, speed);
}

void route_idle(Entity *entity, float speed) {
    vec2 org = entity->pos;
    Queue *q = &entity->path;

    Path *path = path_init();
    memcpy(&path->org, &org, sizeof(vec2));
    memcpy(&path->dst, &org, sizeof(vec2));

    if (SCREEN_WIDTH - entity->dim.width - org.x < org.x) 
        path->dst.x = 0.f;
    else
        path->dst.x = SCREEN_WIDTH - entity->dim.width;

    path->type = PATH_LINEAR;
    path->speed = speed;
    path->orientation = ORIENT_UPRIGHT;
    enqueue(q, path);
}

static void route_swoop_left(Queue *q, vec2 org, float speed) {
    float radius = 100.f;
    route_start(q, org, (vec2) { .x = org.x, .y = org.y + (radius) }, -speed, PATH_CIRCULAR);
    route_append(q, org, -speed, PATH_CIRCULAR);
    route_append(q, (vec2) { .x = 25.f, .y = SCREEN_HEIGHT - (4.f * radius) }, speed, PATH_BEZIER);
    route_append(q, (vec2) { .x = (SCREEN_WIDTH - ENEMY_WIDTH) / 2.f, .y = SCREEN_HEIGHT - (5.f * radius) }, -speed, PATH_BEZIER);
}

static void route_swoop_right(Queue *q, vec2 org, float speed) {
    float radius = 100.f;
    route_start(q, org, (vec2) { .x = org.x, .y = org.y + (radius) }, speed, PATH_CIRCULAR);
    route_append(q, org, speed, PATH_CIRCULAR);
    route_append(q, (vec2) { .x = SCREEN_WIDTH - ENEMY_WIDTH - 25.f, .y = SCREEN_HEIGHT - (4.f * radius) }, speed, PATH_BEZIER);
    route_append(q, (vec2) { .x = (SCREEN_WIDTH - ENEMY_WIDTH) / 2.f, .y = SCREEN_HEIGHT - (5.f * radius) }, -speed, PATH_BEZIER);
}

void route_swoop(Entity *entity, float speed) {
    vec2 org = entity->pos;
    Queue *q = &entity->path;

    assert(entity->parent);

    if (entity->parent->vel.x < 0) 
        route_swoop_left(q, org, speed);
    else
        route_swoop_right(q, org, speed);

    // keep here to return entity to spawn
    // route_append(q, org, speed, PATH_CIRCULAR);
}

void route_merge(Entity *entity, float speed) {
    vec2 org = entity->pos;
    Queue *q = &entity->path;

    route_start(q, org, (vec2) { .x = ENEMY_SPAWN_X, .y = ENEMY_SPAWN_Y }, speed, PATH_BEZIER);
}

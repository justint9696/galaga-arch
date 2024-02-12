#include "data/queue.h"

#include "entity/logic/enemy.h"
#include "entity/logic/route.h"
#include "entity/logic/path.h"

#include "gfx/window.h"
#include "game/world.h"

#include <assert.h>
#include <string.h>

static inline vec2 rear(Queue *q) {
    return (*(Path *)(queue_rear(q))).dst;
}

static void route_spawn_left(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    route_start(q, org, VEC2(radius, ENEMY_SPAWN_Y - 100.f), speed, PATH_CIRCULAR);
    route_append(q, VEC2(rear(q).x, rear(q).y - radius), speed, PATH_CIRCULAR);
    // route_append(q, VEC2(ENEMY_SPAWN_X, ENEMY_SPAWN_Y), speed, PATH_BEZIER);
}

static void route_spawn_right(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    route_start(q, org, VEC2(org.x - radius - ENEMY_WIDTH, ENEMY_SPAWN_Y - 100.f), -speed, PATH_CIRCULAR);
    route_append(q, VEC2(rear(q).x, rear(q).y - radius), -speed, PATH_CIRCULAR);
    // route_append(q, VEC2(ENEMY_SPAWN_X, ENEMY_SPAWN_Y), speed, PATH_BEZIER);
}

static void route_spawn_center(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    float half = (SCREEN_WIDTH / 2.f);
    route_start(q, org, VEC2(org.x - half + (radius * 2), org.y - (radius * 6.f)), -speed, PATH_BEZIER);
    route_append(q, VEC2(rear(q).x, rear(q).y - 3 * radius), speed, PATH_CIRCULAR);
    // route_append(q, VEC2(ENEMY_SPAWN_X, ENEMY_SPAWN_Y), speed, PATH_BEZIER);
}

static void route_spawn(Entity *entity, World *world, float speed) {
    vec2 org = entity->pos;
    Queue *q = &entity->path;

    if (org.x <= 0.f) 
        route_spawn_left(q, org, speed);
    else if (org.x >= SCREEN_WIDTH)
        route_spawn_right(q, org, speed);
    else
        route_spawn_center(q, org, speed);
}

static void route_idle(Entity *entity, World *world, float speed) {
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
    route_start(q, org, VEC2(org.x, org.y + (radius)), -speed, PATH_CIRCULAR);
    route_append(q, org, -speed, PATH_CIRCULAR);
    route_append(q, VEC2(25.f, SCREEN_HEIGHT - (4.f * radius)), speed, PATH_BEZIER);
    route_append(q, VEC2((SCREEN_WIDTH - ENEMY_WIDTH) / 2.f, SCREEN_HEIGHT - (5.f * radius)), -speed, PATH_BEZIER);
}

static void route_swoop_right(Queue *q, vec2 org, float speed) {
    float radius = 100.f;
    route_start(q, org, VEC2(org.x, org.y + (radius)), speed, PATH_CIRCULAR);
    route_append(q, org, speed, PATH_CIRCULAR);
    route_append(q, VEC2(SCREEN_WIDTH - ENEMY_WIDTH - 25.f, SCREEN_HEIGHT - (4.f * radius)), speed, PATH_BEZIER);
    route_append(q, VEC2((SCREEN_WIDTH - ENEMY_WIDTH) / 2.f, SCREEN_HEIGHT - (5.f * radius)), -speed, PATH_BEZIER);
}

static void route_swoop(Entity *entity, World *world, float speed) {
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

static void route_merge(Entity *entity, World *world, float speed) {
    vec2 org = entity->pos;
    Queue *q = &entity->path;

    route_start(q, org, VEC2(ENEMY_SPAWN_X, ENEMY_SPAWN_Y), speed, PATH_BEZIER);
}

static void route_return(Entity *entity, World *world, float speed) {
    vec2 org = entity->pos;
    Queue *q = &entity->path;

    const Entity *formation = world->formation;
    vec2 vel = entity_displacement(formation);

    float radius = 50.f;
    route_start(q, org, VEC2(org.x, org.y + radius), vel.x < 0 ? -speed : speed, PATH_CIRCULAR);
    route_append(q, VEC2(org.x, org.y - radius), vel.x < 0 ? -speed : speed, PATH_CIRCULAR);
}

static void route_charge(Entity *entity, World *world, float speed) {
    vec2 org = entity->pos;
    Queue *q = &entity->path;

    const Entity *player = world->player;
    vec2 vel = entity_displacement(entity);

    route_start(q, org, VEC2(org.x, org.y + 100.f), vel.x < 0.f ? speed : -speed, PATH_CIRCULAR);
    route_append(q, VEC2(player->pos.x, -50.f), -speed, PATH_BEZIER);
}

void route_create(Entity *self, World *world, route_t route, float speed) {
    switch (route) {
        case ROUTE_IDLE:
            route_idle(self, world, speed);
            break;
        case ROUTE_SPAWN:
            route_spawn(self, world, speed);
            break;
        case ROUTE_SWOOP:
            route_swoop(self, world, speed);
            break;
        case ROUTE_RETURN:
            route_return(self, world, speed);
            break;
        case ROUTE_MERGE:
            route_merge(self, world, speed);
            break;
        case ROUTE_CHARGE:
            route_charge(self, world, speed);
            break;
        default: break;
    }
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

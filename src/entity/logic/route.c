#include "entity/logic/enemy.h"
#include "entity/logic/path.h"
#include "entity/logic/route.h"

static inline vec2 _rear(Queue *q) {
    return (*(path_s *)(queue_rear(q))).dst;
}

static inline void _Route_Start(Queue *q, vec2 org, vec2 dst, float speed, ptype_t type) {
    path_s *path = Path_Init();
    path->org = org;
    path->dst = dst;
    path->type = type;
    path->speed = speed;
    enqueue(q, path);
}

static inline void _Route_Append(Queue *q, vec2 dst, float speed, ptype_t type) {
    _Route_Start(q, _rear(q), dst, speed, type);
}

static inline void _Route_SpawnLeft(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    _Route_Start(q, org, (vec2) { .x = radius, .y = ENEMY_SPAWN_Y - 100.f }, speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { .x = _rear(q).x, .y = _rear(q).y - radius }, speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { .x = ENEMY_SPAWN_X, .y = ENEMY_SPAWN_Y }, speed, PATH_BEZIER);
}

static inline void _Route_SpawnRight(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    _Route_Start(q, org, (vec2) { .x = org.x - radius - ENEMY_WIDTH, .y = ENEMY_SPAWN_Y - 100.f }, -speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { .x = _rear(q).x, .y = _rear(q).y - radius }, -speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { .x = ENEMY_SPAWN_X, .y = ENEMY_SPAWN_Y }, speed, PATH_BEZIER);
}

static inline void _Route_SpawnCenter(Queue *q, vec2 org, float speed) {
    float radius = 50.f;
    float half = (WINDOW_WIDTH / 2.f);
    _Route_Start(q, org, (vec2) { .x = org.x - half + (radius * 2), .y = org.y - (radius * 6.f) }, -speed, PATH_BEZIER);
    _Route_Append(q, (vec2) { .x = _rear(q).x, .y = _rear(q).y - 3*radius }, speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { .x = ENEMY_SPAWN_X, .y = ENEMY_SPAWN_Y }, speed, PATH_BEZIER);
}

inline void Route_Dev(Queue *q, vec2 org) {
    float speed = ENEMY_IDLE_VELOCITY;
    _Route_Start(q, org, (vec2) { .x = org.x - 50.f - ENEMY_WIDTH, .y = org.y - 150.f }, -speed, PATH_CIRCULAR);
    _Route_Append(q, org, -speed, PATH_CIRCULAR);
}

inline void Route_Spawn(Queue *q, vec2 org) {
    float speed = ENEMY_SPAWN_VELOCITY;
    if (org.x <= 0.f) 
        _Route_SpawnLeft(q, org, speed);
    else if (org.x >= WINDOW_WIDTH)
        _Route_SpawnRight(q, org, speed);
    else
        _Route_SpawnCenter(q, org, speed);
}

inline void Route_Idle(Queue *q, vec2 org) {
    path_s *path = Path_Init();

    memcpy(&path->org, &org, sizeof(vec2));
    memcpy(&path->dst, &path->org, sizeof(vec2));

    if (org.x < WINDOW_WIDTH - ENEMY_WIDTH) 
        path->dst.x = WINDOW_WIDTH - ENEMY_WIDTH;
    else
        path->dst.x = 0.f;

    path->type = PATH_LINEAR;
    path->speed = ENEMY_IDLE_VELOCITY;
    path->orientation = ORIENT_UPRIGHT;
    enqueue(q, path);
}

inline void Route_Swoop(Queue *q, vec2 org) {
    float speed = ENEMY_SPAWN_VELOCITY;
    float radius = 100.f;
    _Route_Start(q, org, (vec2) { .x = org.x, .y = org.y + (radius) }, -speed, PATH_CIRCULAR);
    _Route_Append(q, org, -speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { .x = org.x - (2.f * radius), .y = org.y - (1.5 * radius) }, speed, PATH_BEZIER);
    _Route_Append(q, (vec2) { .x = org.x + (2.f * radius), .y = org.y - (3.0 * radius) }, -speed, PATH_BEZIER);

    // keep here
    _Route_Append(q, org, speed, PATH_CIRCULAR);
}


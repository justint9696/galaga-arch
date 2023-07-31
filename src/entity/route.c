#include "enemy.h"
#include "path.h"
#include "route.h"

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
    path_s *path = Path_Init();
    path->org = _rear(q);
    path->dst = dst;
    path->type = type;
    path->speed = speed;
    enqueue(q, path);
}

inline void Route_Spawn(Queue *q, vec2 org) {
    float radius = 50.f;
    float speed = ENEMY_SPAWN_VELOCITY;
    _Route_Start(q, org, (vec2) { 50, ENEMY_SPAWN_Y - 100.f }, speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { _rear(q).x, _rear(q).y - radius }, speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { ENEMY_SPAWN_X, ENEMY_SPAWN_Y }, speed, PATH_BEZIER);
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
    path->orientation = ORIENTATE_UPRIGHT;
    enqueue(q, path);
}

inline void Route_Swoop(Queue *q, vec2 org) {
    float speed = ENEMY_SPAWN_VELOCITY;
    float radius = 100.f;
    _Route_Start(q, org, (vec2) { org.x, org.y + (radius) }, -speed, PATH_CIRCULAR);
    _Route_Append(q, org, -speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { org.x - (2.f * radius), org.y - (1.5 * radius) }, speed, PATH_BEZIER);
    _Route_Append(q, (vec2) { org.x + (2.f * radius), org.y - (3.0 * radius) }, -speed, PATH_BEZIER);

    // keep here
    _Route_Append(q, org, speed, PATH_CIRCULAR);
}


#include "inc/enemy.h"
#include "inc/path.h"
#include "inc/route.h"

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
    path->org = (*(path_s *)(queue_rear(q))).dst;
    path->dst = dst;
    path->type = type;
    path->speed = speed;
    enqueue(q, path);
}

static inline vec2 _rear(Queue *q) {
    return (*(path_s *)(queue_rear(q))).dst;
}

inline void Route_Spawn(Queue *q, vec2 org) {
    float radius = 50.f;
    _Route_Start(q, org, (vec2) { 50, ENEMY_SPAWN_Y - 100.f }, ENEMY_SPAWN_VELOCITY, PATH_LINEAR);
    _Route_Append(q, (vec2) { _rear(q).x, _rear(q).y - radius }, -ENEMY_SPAWN_VELOCITY, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { ENEMY_SPAWN_X, ENEMY_SPAWN_Y }, ENEMY_SPAWN_VELOCITY, PATH_BEZIER);
}

inline void Route_Idle(Queue *q, vec2 org) {
    _Route_Start(q, org, (vec2) { org.x < WINDOW_WIDTH - ENEMY_WIDTH ? WINDOW_WIDTH - ENEMY_WIDTH : 0.f, org.y }, ENEMY_IDLE_VELOCITY, PATH_LINEAR); 
}

inline void Route_Swoop(Queue *q, vec2 org) {
    float radius = 50.f;
    _Route_Start(q, org, (vec2) { org.x, org.y - (2 * radius) }, ENEMY_SPAWN_VELOCITY, PATH_CIRCULAR);
    _Route_Append(q, org, ENEMY_SPAWN_VELOCITY, PATH_CIRCULAR);
}


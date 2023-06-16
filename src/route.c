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
    path_s *path = Path_Init();

    path->org = org;
    if (org.x < WINDOW_WIDTH - ENEMY_WIDTH) 
        path->dst.x = WINDOW_WIDTH - ENEMY_WIDTH;
    else
        path->dst.x = 0.f;

    path->dst.y = path->org.y;
    path->type = PATH_LINEAR;
    path->speed = ENEMY_IDLE_VELOCITY;
    enqueue(q, path);
}

inline void Route_Swoop(Queue *q, vec2 org) {
    float speed = 0.25;
    float radius = 100.f;
    _Route_Start(q, org, (vec2) { org.x, org.y + (radius) }, -speed, PATH_CIRCULAR);
    _Route_Append(q, org, -speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { org.x - (1 * radius), org.y - (1.5 * radius)}, speed, PATH_CIRCULAR);
    _Route_Append(q, (vec2) { org.x + (1 * radius), org.y - (3 * radius)}, speed, PATH_CIRCULAR);

    // keep here
    _Route_Append(q, org, speed, PATH_CIRCULAR);
}


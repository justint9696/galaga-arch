#include "inc/enemy.h"
#include "inc/route.h"

inline void Route_Spawn(Queue *q, vec2 org) {
    path_s *path = Path_Init();
    path->org = (vec2) { -ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f };
    path->dst = (vec2) { 50, ENEMY_SPAWN_Y - 100.f };
    path->type = PATH_LINEAR;
    path->speed = ENEMY_VELOCITY;
    enqueue(q, path);

    float radius = 50.f;
    path = Path_Init();
    path->org = (*(path_s *)(queue_rear(q))).dst;
    path->dst = (vec2) { path->org.x, path->org.y - radius };
    path->type = PATH_CIRCULAR;
    path->speed = -ENEMY_VELOCITY;
    enqueue(q, path);

//    path = Path_Init();
//    path->org = (*(path_s *)(queue_rear(q))).dst;
//    path->dst = (vec2) { path->org.x, path->org.y + radius };
//    path->type = PATH_CIRCULAR;
//    path->speed = -ENEMY_VELOCITY;
//    enqueue(q, path);

    path = Path_Init();
    path->org = (*(path_s *)(queue_rear(q))).dst;
    path->dst = (vec2) { ENEMY_SPAWN_X, ENEMY_SPAWN_Y };
    path->type = PATH_BEZIER;
    path->speed = ENEMY_VELOCITY;
    enqueue(q, path);
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
    path->speed = ENEMY_VELOCITY;
    enqueue(q, path);
}

inline void Route_Swoop(Queue *q, vec2 org) {
    path_s *path = Path_Init();
    path->org = org;
    path->dst = (vec2) { org.x, org.y - 50.f };
    path->type = PATH_CIRCULAR;
    path->speed = ENEMY_VELOCITY;
    enqueue(q, path);

    path = Path_Init();
    path->org = (*(path_s *)(queue_rear(q))).dst;
    path->dst = (vec2) { org.x, org.y - 250.f };
    path->type = PATH_BEZIER;
    path->speed = ENEMY_VELOCITY;
    enqueue(q, path);

    path = Path_Init();
    path->org = (*(path_s *)(queue_rear(q))).dst;
    path->dst = org;
    path->type = PATH_BEZIER;
    path->speed = ENEMY_VELOCITY;
    enqueue(q, path);
}


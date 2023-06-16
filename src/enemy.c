#include "inc/enemy.h"
#include "inc/entity.h"
#include "inc/hud.h"
#include "inc/level.h"
#include "inc/linked_list.h"
#include "inc/path.h"
#include "inc/player.h"
#include "inc/route.h"
#include "inc/time.h"
#include "inc/util.h"

#include <assert.h>

static int _count = 0;
static LinkedList *_enemies;

static inline Enemy *_Enemy_Init() {
    Enemy *self = (Enemy *)calloc(1, sizeof(Enemy));

    // self->entity = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, -ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    // self->state = STATE_SPAWN;
   
    self->entity = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, ENEMY_SPAWN_X, ENEMY_SPAWN_Y, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    self->state = STATE_IDLE;

    return self;
}

static void _Enemy_ThinkPath(Enemy *self) {
    switch (self->state) {
    case STATE_IDLE:
        Route_Swoop(&self->path, self->entity->pos);
        break;
    case STATE_SPAWN:
        Route_Spawn(&self->path, self->entity->pos);
        break;
    default:
        break;
    }

    self->state = STATE_TRAVEL;
    printf("path queued. items remaining in queue: %li.\n", self->path.size);

}

static void _Enemy_TravelPath(Enemy *self, uint64_t tick) {
    path_s *path = (path_s *)queue_front(&self->path);
    assert(path);

    switch (path->type) {
    case PATH_LINEAR:
        Path_Linear(self->entity, path);
        break;
    case PATH_CIRCULAR:
        Path_Circular(self->entity, path);
        break;
    case PATH_BEZIER:
        Path_Bezier(self->entity, path);
        break;
    }

    float distance = Distance(self->entity->pos, path->dst);
    Hud_AddText("Path: %s",
            path->type == PATH_LINEAR ? "Linear" : 
            path->type == PATH_BEZIER ? "Bezier" : "Circular");

    Hud_AddText("Queue: %i", self->path.size);
    Hud_AddText("Distance: %.2f", distance);

    if (distance < 15.f) {
        Entity_SetPosition(self->entity, path->dst);
        Entity_SetVelocity(self->entity, (vec2) { 0.f, 0.f });

        dequeue(&self->path);
        if (!self->path.size) {
            self->state = STATE_IDLE;
            self->tick = tick;
            queue_free(&self->path);
        }

        printf("path dequeued. items remaining in queue: %li.\n", self->path.size);
    }
}

static void _Enemy_ThinkAttack(Enemy *self, uint64_t tick) {
    const vec2 
        pos = Player_Position(),
        vel = Player_Velocity();

    Entity *ent = self->entity;

    const float
        dx = (pos.x - ent->pos.x),
        dy = fabs(pos.y - ent->pos.y);

    // player is not moving
    if (vel.x == 0.f) {
        if (fabs(dx) < 5.f) {
            Entity_Fire(ent, tick);
            self->state = STATE_ATTACK;
        }
        return;
    }

    // player is moving in the same direction
    if ((dx < 0.f && vel.x < 0.f) || (dx > 0.f && vel.x > 0.f)) 
        return;
    
    const float 
        tx = fabs(dx / PLAYER_VELOCITY),
        ty = fabs(dy / BULLET_VELOCITY);

    if (fabs(dx) < 30.f || (fabs(tx - ty) < 1.f)) {
        Entity_Fire(ent, tick);
        self->state = STATE_ATTACK;
    }
}

static void _Enemy_Think(Enemy *self, uint64_t tick) {
    estate_t p_state = self->state;
    // _Enemy_ThinkAttack(self, tick);
    
    if (Time_Passed(self->tick) > 1000.f) {
        switch (self->state) {
            case STATE_IDLE:
            case STATE_SPAWN:
                _Enemy_ThinkPath(self);
                break;
            case STATE_ATTACK:
                // return to state prior attack
                self->state = p_state;
                break;
            case STATE_TRAVEL:
                _Enemy_TravelPath(self, tick);
                break;
            default:
                break;
        }    
    }
    Hud_AddText("State: %s", 
            self->state == STATE_IDLE ? "Idle" :
            self->state == STATE_TRAVEL ? "Travel" :
            self->state == STATE_SPAWN ? "Spawn" : 
            self->state == STATE_SWOOP ? "Swoop" : "Attack");
}

void Enemy_InitAll(uint64_t tick) {
    Enemy *enemy;
    int count = Level_EnemyCount();

    _enemies = LinkedList_Init();
    
    for (int i = 0; i < count; i++) {
        enemy = _Enemy_Init();
        enemy->entity->tick = tick;
        LinkedList_Add(_enemies, (void *)enemy);
    }

    printf("Enemies initialized.\n");
}

void Enemy_UpdateAll(uint64_t tick) {
    Enemy *enemy;
    LinkedList *tmp = _enemies;
    
    while (tmp) {
        enemy = (Enemy *)tmp->item;
        assert(enemy && Entity_IsAlive(enemy->entity));

        _Enemy_Think(enemy, tick);

        tmp = tmp->next;
    }
}

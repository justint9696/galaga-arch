#include "gfx/hud.h"

#include "game/level.h"
#include "game/time.h"

#include "data/linked_list.h"

#include "common/util.h"

#include "entity/player.h"

#include "enemy.h"
#include "route.h"

#include <assert.h>

static int _count = 0;
static LinkedList *_enemies;

static inline Enemy *_Enemy_Init(uint64_t tick) {
    Enemy *self = (Enemy *)calloc(1, sizeof(Enemy));

    self->entity = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, -ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    self->state = STATE_SPAWN;
   
    // self->entity = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, ENEMY_SPAWN_X, ENEMY_SPAWN_Y, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    // self->state = STATE_IDLE;

    self->entity->tick = tick;
    return self;
}

static void _Enemy_ThinkPath(Enemy *self) {
    switch (self->state) {
    case STATE_IDLE:
        Route_Idle(&self->path, self->entity->pos);
        break;
    case STATE_SPAWN:
        self->idle_tick = -1;
        Route_Spawn(&self->path, self->entity->pos);
        break;
    default:
        break;
    }

    self->state = STATE_TRAVEL;
    // printf("path queued. items remaining in queue: %li.\n", self->path.size);

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

    float distance = distance(self->entity->pos, path->dst);
    Hud_AddText("Path: %s",
            path->type == PATH_LINEAR ? "Linear" : 
            path->type == PATH_BEZIER ? "Bezier" : "Circular");

    Hud_AddText("Queue: %i", self->path.size);
    Hud_AddText("Distance: %.2f", distance);

    if (path->complete) {
        Entity_SetPosition(self->entity, path->dst);
        Entity_SetVelocity(self->entity, (vec2) { 0.f, 0.f });

        dequeue(&self->path);
        if (!self->path.size) {
            self->state = STATE_IDLE;
            self->idle_tick = tick;
        }

        // printf("path dequeued. items remaining in queue: %li.\n", self->path.size);
    } 

    if (self->idle_tick == -1)
        return;

    if (Time_Passed(self->idle_tick) > ENEMY_IDLE_TIME) {
        self->idle_tick = -1;

        queue_free(&self->path);
        Route_Swoop(&self->path, self->entity->pos);
    }
}

static void _Enemy_ThinkAttack(Enemy *self, uint64_t tick) {
    const vec2 
        pos = Player_Position(),
        vel = Player_Velocity();

    Entity *ent = self->entity;

    const vec2 diff = {
        (pos.x - ent->pos.x),
        fabs(pos.y - ent->pos.y)
    };

    if (!Player_IsMoving()) {
        if (fabs(diff.x) < 5.f)
            Entity_Fire(ent, tick);

        return;
    }

    // player is moving in the same direction
    if ((diff.x < 0.f && vel.x < 0.f) || (diff.x > 0.f && vel.x > 0.f)) 
        return;
    
    const vec2 time = {
        fabs(diff.x / PLAYER_VELOCITY),
        fabs(diff.y / BULLET_VELOCITY)
    };

    if (fabs(diff.x) < 30.f || (fabs(time.x - time.y) < 1.f))
        Entity_Fire(ent, tick);
}

static void _Enemy_Think(Enemy *self, uint64_t tick) {
    estate_t p_state = self->state;
    _Enemy_ThinkAttack(self, tick);
    
    switch (self->state) {
    case STATE_IDLE:
    case STATE_SPAWN:
        _Enemy_ThinkPath(self);
        break;
    case STATE_ATTACK:
        if (self->state == p_state)
            self->state = STATE_IDLE;
        else
            self->state = p_state;
        break;
    case STATE_TRAVEL:
        _Enemy_TravelPath(self, tick);
        break;
    default:
        break;
    }    

    Hud_AddText("State: %s", 
            self->state == STATE_IDLE ? "Idle" :
            self->state == STATE_TRAVEL ? "Travel" :
            self->state == STATE_SPAWN ? "Spawn" : "Attack");
}

void Enemy_Free(Enemy *self) {
    printf("enemy freed\n");
    Entity_Free(self->entity);
    LinkedList_Remove(&_enemies, (void *)self);
    free(self);
}

void Enemy_InitAll(uint64_t tick) {
    Enemy *enemy;
    int count = Level_EnemyCount();

    _enemies = LinkedList_Init();
    
    for (int i = 0; i < count; i++) {
        enemy = _Enemy_Init(tick);
        LinkedList_Add(_enemies, (void *)enemy);
    }

    printf("Enemies initialized.\n");
}

void Enemy_UpdateAll(uint64_t tick) {
    Enemy *enemy;
    LinkedList *tmp = _enemies;
    
    while (tmp) {
        enemy = (Enemy *)tmp->item;
        assert(enemy);

        if (Entity_IsAlive(enemy->entity))
            _Enemy_Think(enemy, tick);
        else 
            Enemy_Free(enemy);;

        tmp = tmp->next;
    }
}

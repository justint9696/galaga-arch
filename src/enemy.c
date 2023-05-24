#include "inc/enemy.h"
#include "inc/level.h"
#include "inc/linked_list.h"
#include "inc/path.h"
#include "inc/player.h"
#include "inc/util.h"

#include <assert.h>

static int _count;
static Enemy **_enemies;

static inline Enemy *_Enemy_Init() {
    Enemy *self = (Enemy *)malloc(sizeof(Enemy));
    memset(self, 0, sizeof(Enemy));

    self->ent = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, -ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    self->state = STATE_IDLE;

    return self;
}

static void _Enemy_ResetPath(Enemy *self) {
    self->ent->state = STATE_IDLE;
    queue_free(&self->path);
}

static void _Enemy_ThinkPath(Enemy *self) {
    path_s *path = (path_s *)malloc(sizeof(path_s));

    switch (self->state) {
    case STATE_IDLE:
        path->dst = self->ent->pos;

        if (self->ent->pos.x < WINDOW_WIDTH - self->ent->width)
            path->dst.x = WINDOW_WIDTH - self->ent->width;
        else
            path->dst.x = 0.f;

        Path_Linear(self->ent, self->ent->pos, path->dst, ENEMY_VELOCITY);

        path->type = PATH_LINEAR;
        break;
    case STATE_SPAWN:
        path->org = self->ent->pos;

        path->dst.x = ENEMY_SPAWN_X;
        path->dst.y = ENEMY_SPAWN_Y;
            
        Path_Linear(self->ent, path->org, path->dst, ENEMY_VELOCITY);

        path->type = PATH_LINEAR;
        break;
    }

    printf("path queued\n");
    enqueue(&self->path, (void *)path);
    self->ent->state = STATE_TRAVEL;
}

static void _Enemy_TravelPath(Enemy *self) {
    path_s *path = (path_s *)queue_front(&self->path);
    assert(path);

    switch (path->type) {
    case PATH_LINEAR:
        Path_Linear(self->ent, path->org, path->dst, ENEMY_VELOCITY);
        break;
    case PATH_CIRCULAR:
        Path_Circular(self->ent, path->org, path->dst, ENEMY_VELOCITY);
        break;
    case PATH_BEZIER:
        Path_Bezier(self->ent, path->org, path->dst, ENEMY_VELOCITY);
        break;
    }

    if (Distance(self->ent->pos, path->dst) < .5f) {
        dequeue(&self->path);
        self->state = STATE_IDLE;
    }
}

static void _Enemy_ThinkAttack(Enemy *self, uint64_t tick) {
    const vec2 
        pos = Player_Position(),
        vel = Player_Velocity();

    Entity *ent = self->ent;

    const float
        dx = (pos.x - ent->pos.x),
        dy = fabs(pos.y - ent->pos.y);

    // player is not moving
    if (vel.x == 0.f) {
        if (fabs(dx) < 5.f)
            Entity_Fire(ent, tick);

        return;
    }

    // player is moving in the same direction
    if ((dx < 0.f && vel.x < 0.f) || (dx > 0.f && vel.x > 0.f)) 
        return;
    
    const float 
        tx = fabs(dx / PLAYER_VELOCITY),
        ty = fabs(dy / BULLET_VELOCITY);

   if (fabs(dx) < 30.f || (fabs(tx - ty) < 1.f))
        Entity_Fire(ent, tick);
}

static void _Enemy_Think(Enemy *self, uint64_t tick) {
    state_t p_state = self->state;
    _Enemy_ThinkAttack(self, tick);
    
    switch (self->state) {
    case STATE_IDLE:
    case STATE_SPAWN:
        _Enemy_ThinkPath(self);
        self->state = STATE_TRAVEL;
        break;
    case STATE_ATTACK:
        // return to state prior attack
        self->state = p_state;
        break;
    case STATE_TRAVEL:
        _Enemy_TravelPath(self);
        break;
    default:
        break;
    }    
}

void Enemy_InitAll(uint64_t tick) {
    Enemy *enemy;
    _count = Level_EnemyCount();
    _enemies = (Entity **)malloc(sizeof(Entity *) * _count);
    
    for (int i = 0; i < _count; i++) {
        enemy = _Enemy_Init(tick);
        enemy->state = ENEMY_START_STATE;
        enemy->ent->tick = tick;
        _enemies[i] = enemy;
    }

    printf("Enemies initialized.\n");
}

void Enemy_UpdateAll(uint64_t tick) {
    Enemy *enemy;
    
    for (int i = 0; i < _count; i++) {
        enemy = _enemies[i];

        if (!enemy || !Entity_IsAlive(enemy->ent))
            continue;

        _Enemy_Think(enemy, tick);
    }
}

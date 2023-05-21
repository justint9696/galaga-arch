#include "inc/enemy.h"
#include "inc/level.h"
#include "inc/linked_list.h"
#include "inc/path.h"
#include "inc/player.h"
#include "inc/util.h"

static int _count;
static Entity **_entities;

static inline Entity *_Enemy_Init() {
    return Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, -ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
}

static void _Enemy_ResetPath(Entity *self) {
    self->state = STATE_IDLE;
    memset(&self->path, 0, sizeof(path_s));
}

static void _Enemy_ThinkPath(Entity *self) {
    switch (self->state) {
    case STATE_IDLE:
        self->path.dst = self->pos;

        if (self->pos.x < WINDOW_WIDTH - self->width)
            self->path.dst.x = WINDOW_WIDTH - self->width;
        else
            self->path.dst.x = 0.f;

        Path_Linear(self, self->pos, self->path.dst, ENEMY_VELOCITY);

        self->path.type = PATH_LINEAR;
        break;
    case STATE_SPAWN:
        self->path.org = self->pos;

        self->path.dst.x = ENEMY_SPAWN_X;
        self->path.dst.y = ENEMY_SPAWN_Y;
            
        Path_Bezier(self, self->path.org, self->path.dst, ENEMY_VELOCITY);

        self->path.type = PATH_BEZIER;
        break;
    }

    self->state = STATE_TRAVEL;
}

static void _Enemy_TravelPath(Entity *self) {
    // TODO: decide enemy pathing here    
    Hud_AddText("Entity Path: %s", self->path.type == PATH_LINEAR ? "Linear" : self->path.type == PATH_BEZIER ? "Bezier" : "Circular");
    switch (self->path.type) {
    case PATH_LINEAR:
        break;
    case PATH_CIRCULAR:
        Path_Circular(self, self->path.org, self->path.dst, ENEMY_VELOCITY);
        break;
    case PATH_BEZIER:
        Path_Bezier(self, self->path.org, self->path.dst, ENEMY_VELOCITY);
        break;
    }

    if (Distance(self->pos, self->path.dst) < .5f)
        _Enemy_ResetPath(self);
}

static void _Enemy_ThinkAttack(Entity *self, uint64_t tick) {
    const vec2 
        pos = Player_Position(),
        vel = Player_Velocity();

    const float
        dx = (pos.x - self->pos.x),
        dy = fabs(pos.y - self->pos.y);

    // player is not moving
    if (vel.x == 0.f) {
        if (fabs(dx) < 5.f)
            Entity_Fire(self, tick);

        return;
    }

    // player is moving in the same direction
    if ((dx < 0.f && vel.x < 0.f) || (dx > 0.f && vel.x > 0.f)) 
        return;
    
    const float 
        tx = fabs(dx / PLAYER_VELOCITY),
        ty = fabs(dy / BULLET_VELOCITY);

   if (fabs(dx) < 30.f || (fabs(tx - ty) < 1.f))
        Entity_Fire(self, tick);
}

static void _Enemy_Think(Entity *self, uint64_t tick) {
    state_t p_state = self->state;
    _Enemy_ThinkAttack(self, tick);
    
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
        _Enemy_TravelPath(self);
        break;
    default:
        break;
    }    
}

void Enemy_InitAll(uint64_t tick) {
    Entity *entity;
    _count = Level_EnemyCount();
    _entities = (Entity **)malloc(sizeof(Entity *) * _count);
    
    for (int i = 0; i < _count; i++) {
        entity = _Enemy_Init(tick);
        entity->state = ENEMY_START_STATE;
        entity->tick = tick;
        _entities[i] = entity;
    }

    printf("Enemies initialized.\n");
}

void Enemy_UpdateAll(uint64_t tick) {
    Entity *entity;
    
    for (int i = 0; i < _count; i++) {
        entity = _entities[i];

        if (!entity || !Entity_IsAlive(entity))
            continue;

        _Enemy_Think(entity, tick);
    }
}

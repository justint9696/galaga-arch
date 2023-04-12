#include "inc/enemy.h"
#include "inc/level.h"
#include "inc/linked_list.h"
#include "inc/path.h"
#include "inc/player.h"
#include "inc/util.h"

static int _count;
static Entity **_entities;

static inline Entity *_Enemy_Init() {
    return Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, ENEMY_SPAWN_X, ENEMY_SPAWN_Y, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
}

static void _Enemy_ThinkPath(Entity *self) {
    self->dst = self->pos;

    if (self->pos.x < WINDOW_WIDTH - self->width)
        self->dst.x = WINDOW_WIDTH - self->width;
    else
        self->dst.x = 0.f;

    self->prev_state = self->state;
    self->state = STATE_TRAVEL;

    Path_Linear(self, self->dst, ENEMY_VELOCITY);
}

static void _Enemy_ResetPath(Entity *self) {
    self->state = STATE_IDLE;
    memset(&self->dst, 0, sizeof(vec2));    
}

static void _Enemy_ThinkAttack(Entity *self, uint64_t tick) {
    const vec2 pos = Player_Position(),
          vel = Player_Velocity();

    // velocity = distance / time
    // distance / velocity = time
    const float
        dx = (pos.x - self->pos.x),
        dy = fabs(pos.y - self->pos.y);

    // player is not moving
    if (vel.x == 0.f) {
        if (fabs(dx) < 5.f)
            Entity_Fire(self, tick);

        return;
    }

    // player is going in the same direction
    if ((dx < 0.f && vel.x < 0.f) || (dx > 0.f && vel.x > 0.f)) 
        return;
    
    const float 
        tx = fabs(dx / PLAYER_VELOCITY),
        ty = fabs(dy / BULLET_VELOCITY);

   if (fabs(dx) < 30.f || (fabs(tx - ty) < 1.f))
        Entity_Fire(self, tick);
}

static void _Enemy_Think(Entity *self, uint64_t tick) {
    _Enemy_ThinkAttack(self, tick);
    
    switch (self->state) {
    case STATE_IDLE:
        _Enemy_ThinkPath(self);
        break;
    case STATE_SPAWN:
        // TODO: make enemies fly in from off screen
        break;
    case STATE_ATTACK:
        self->state = self->prev_state;
        break;
    case STATE_TRAVEL:
        if (Distance(self->pos, self->dst) < 5.f)
            _Enemy_ResetPath(self);
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
        entity->state = STATE_SPAWN;
        entity->tick = tick;
        _entities[i] = entity;
    }

    printf("Enemies initialized.\n");
}

void Enemy_UpdateAll(uint64_t tick) {
   Entity *entity;
    
    for (int i = 0; i < _count; i++) {
        entity = _entities[i];
        if (!entity)
            break;

        if (!entity->health)
            continue;

        _Enemy_Think(entity, tick);
    }
}

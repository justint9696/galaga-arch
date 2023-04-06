#include "inc/enemy.h"
#include "inc/linked_list.h"
#include "inc/path.h"
#include "inc/player.h"
#include "inc/util.h"

static int _count;
static Entity **_entities;

static Entity *_Enemy_Init() {
    return Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_X, ENEMY_SPAWN_Y, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
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
    const vec2 pos = Player_GetPosition(),
        vel = Player_GetVelocity();

    // velocity = distance / time
    // distance / velocity = time
    const float distance = Distance(pos, self->pos);
    const float time = (distance / BULLET_VELOCITY);

    printf("time: %.2f\n", time / 1000.f);

    if ((time / 1000.f) < 0.5f)
        Entity_Fire(self, tick);

}

static void _Enemy_Think(Entity *self, uint64_t tick) {
    _Enemy_ThinkAttack(self, tick);
    
    switch (self->state) {
    case STATE_IDLE:
        _Enemy_ThinkPath(self);
        break;
    case STATE_ATTACK:
        self->state = self->prev_state;
        break;
    case STATE_TRAVEL:
        if (Distance(self->pos, self->dst) < 15.f)
            _Enemy_ResetPath(self);
        break;
    default:
        break;
    }    
}

void Enemy_InitAll(const int count) {
    Entity *entity;
    _count = count;
    _entities = (Entity **)malloc(sizeof(Entity *) * _count);
    
    for (int i = 0; i < _count; i++) {
        entity = _Enemy_Init();
        _entities[i] = entity;
    }

    printf("Enemies initialized.\n");
}

void Enemy_UpdateAll(uint64_t tick) {
   Entity *entity;
    
    for (int i = 0; i < _count; i++) {
        entity = _entities[i];
        if (!entity)
            continue;

        _Enemy_Think(entity, tick);
    }
}

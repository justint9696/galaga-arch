#include "gfx/hud.h"

#include "game/time.h"

#include "data/linked_list.h"

#include "common/util.h"

#include "entity/player.h"

#include "entity/ai/enemy.h"
#include "entity/ai/route.h"

#include <assert.h>

static int _count = 0;
static LinkedList *_enemies;

/**
 * wave 1: top center
 * wave 2: bottom left
 * wave 3: bottom right
 * wave 4: top center
 */
static inline vec2 _formation_one(ewave_t wave) {
    switch (wave) {
        case WAVE_ONE:
            return (vec2) { -ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f };
        case WAVE_TWO:
            return (vec2) { WINDOW_WIDTH, ENEMY_SPAWN_Y - 250.f };
        // case WAVE_THREE:
        // case WAVE_FOUR:
        //     break;
        default:
            return (vec2) { -ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f };
    }
    return (vec2) { 0, 0 };
}

static inline vec2 _formation_two(ewave_t wave) {}

static inline vec2 _formation_three(ewave_t wave) {}

/**
 * based on formation, enemies should spawn at different points off-screen
 * in formation one: 8 enemies in a line on one side of the screen
 * in formation two: 2 rows of 4 enemies in a line on one side of the screen
 * in formation three: 8 enemies in a line on both sides of the screen
 * in formation four: same as formation three, and back to formation one
 * where on the next four, there is a speed scalar
 */
static inline vec2 _Enemy_Spawnpoint(ewave_t wave, eformation_t formation) {
    vec2 spawnpoint;

    switch (formation) {
        case FORMATION_ONE:
            return _formation_one(wave);
        case FORMATION_TWO:
            return _formation_two(wave);
        case FORMATION_THREE:
        case FORMATION_FOUR:
            return _formation_three(wave);
        default:
            return _formation_one(wave);
    }

    return spawnpoint;
}

static inline Enemy *_Enemy_Init(uint64_t tick, ewave_t wave, eformation_t formation) {
    Enemy *self = (Enemy *)calloc(1, sizeof(Enemy));

    // Spawn Right
    // self->entity = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, WINDOW_WIDTH, ENEMY_SPAWN_Y - 250.f, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    // self->state = STATE_SPAWN;

    // Spawn Left
    // self->entity = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, -ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    // self->state = STATE_SPAWN;

    // Spawn Center
    // self->entity = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, ENEMY_SPAWN_X, ENEMY_SPAWN_Y, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    // self->state = STATE_IDLE;

    vec2 spawnpoint = _Enemy_Spawnpoint(wave, formation);
    self->entity = Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_HEALTH, spawnpoint.x, spawnpoint.y, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
    self->entity->tick = tick;
    self->state = STATE_SPAWN;

    LinkedList_Add(_enemies, (void *)self);

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
    // Hud_AddText("Path: %s",
    //   path->type == PATH_LINEAR ? "Linear" : 
    //   path->type == PATH_BEZIER ? "Bezier" : "Circular");

    // Hud_AddText("Queue: %i", self->path.size);
    // Hud_AddText("Distance: %.2f", distance);

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

    /*Hud_AddText("State: %s", 
      self->state == STATE_IDLE ? "Idle" :
      self->state == STATE_TRAVEL ? "Travel" :
      self->state == STATE_SPAWN ? "Spawn" : "Attack");*/
}

void Enemy_InitAll() {
    _enemies = LinkedList_Init();
    printf("Enemies initialized.\n");
}

void Enemy_Free(Enemy *self) {
    LinkedList_Remove(_enemies, (void *)self);
    free(self);

    _count--;
}

void Enemy_InitCount(uint64_t tick, uint32_t count, ewave_t wave, eformation_t formation) {
    Enemy *enemy;

    for (int i = 0; i < count; i++)
        enemy = _Enemy_Init(tick, wave, formation);

    _count = count;
}

int Enemy_UpdateAll(uint64_t tick) {
    int count = 0;
    Enemy *enemy;
    Node *tmp = _enemies->head;

    while (tmp) {
        enemy = (Enemy *)tmp->item;
        assert(enemy);

        tmp = tmp->next;
        if (!enemy->entity)
            Enemy_Free(enemy);
        else if (Entity_IsAlive(enemy->entity)) {
            count++;
            _Enemy_Think(enemy, tick);
        }
    }

    Hud_AddText("Enemies: %i", _count);

    return count;
}

Enemy *Enemy_SpawnWave(uint64_t tick, uint32_t count, ewave_t wave, eformation_t formation) {
    Enemy_InitCount(tick, count, wave, formation);
    Enemy *enemy = (Enemy *)_enemies->tail->item;

    printf("Enemy wave %i spawned.\n", wave);

    return enemy;
}

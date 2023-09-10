#include "gfx/hud.h"

#include "game/time.h"

#include "common/util.h"

#include "entity/player.h"

#include "entity/logic/enemy.h"
#include "entity/logic/route.h"
#include "entity/logic/spawn.h"

#include <assert.h>

static void _Enemy_ThinkPath(Enemy *self) {
    switch (self->state) {
        case STATE_IDLE:
            Route_Idle(&self->path, self->entity.pos);
            break;
        case STATE_SPAWN:
            self->idle_tick = -1;
            Route_Spawn(&self->path, self->entity.pos);
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
            Path_Linear(&self->entity, path);
            break;
        case PATH_CIRCULAR:
            Path_Circular(&self->entity, path);
            break;
        case PATH_BEZIER:
            Path_Bezier(&self->entity, path);
            break;
    }

    // float distance = distance(self->entity.pos, path->dst);
    // Hud_AddText("Path: %s",
    //   path->type == PATH_LINEAR ? "Linear" : 
    //   path->type == PATH_BEZIER ? "Bezier" : "Circular");

    // Hud_AddText("Queue: %i", self->path.size);
    // Hud_AddText("Distance: %.2f", distance);

    if (path->complete) {
        Entity_SetPosition(&self->entity, path->dst);
        Entity_SetVelocity(&self->entity, (vec2) { .x = 0.f, .y = 0.f });

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
        Route_Swoop(&self->path, self->entity.pos);
    }
}

static void _Enemy_ThinkAttack(Enemy *self, World *world, uint64_t tick) {
    const vec2 
        pos = Player_Position(&world->player),
        vel = Player_Velocity(&world->player);

    Entity *ent = &self->entity;

    const vec2 diff = {
        .x = (pos.x - ent->pos.x),
        .y = fabs(pos.y - ent->pos.y)
    };

    if (!Player_IsMoving(&world->player)) {
        if (fabs(diff.x) < 5.f)
            Entity_Fire(ent, &world->entities, tick);

        return;
    }

    // player is moving in the same direction
    if ((diff.x < 0.f && vel.x < 0.f) || (diff.x > 0.f && vel.x > 0.f)) 
        return;

    const vec2 time = {
        .x = fabs(diff.x / PLAYER_VELOCITY),
        .y = fabs(diff.y / BULLET_VELOCITY)
    };

    if (fabs(diff.x) < 30.f || (fabs(time.x - time.y) < 1.f))
        Entity_Fire(ent, &world->entities, tick);
}

static void _Enemy_Think(Enemy *self, World *world, uint64_t tick) {
    estate_t p_state = self->state;
    // _Enemy_ThinkAttack(self, world, tick);

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

Enemy *Enemy_Init(ewave_t wave, eformation_t formation, uint64_t tick) {
    Enemy *self = calloc(1, sizeof(Enemy));
    vec2 pos = Spawn_GetPosition(wave, formation);

    Entity_Init(&self->entity, TYPE_AXIS, TEAM_AXIS, ENEMY_SPAWN_HEALTH, pos.x, pos.y, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);

    self->entity.tick = tick;
    self->state = STATE_SPAWN;

    return self;
}

void Enemy_Update(Enemy *self, World *world, uint64_t tick) {
    _Enemy_Think(self, world, tick);
}


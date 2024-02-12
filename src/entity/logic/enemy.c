#include "common/time.h"
#include "common/util.h"

#include "entity/logic/enemy.h"
#include "entity/entity.h"
#include "entity/logic/route.h"

#include "entity/abductor.h"
#include "entity/formation.h"
#include "entity/invader.h"
#include "entity/player.h"
#include "entity/projectile.h"

#include "game/world.h"

#include <assert.h>

static void enemy_attack(Entity *self, World *world) {
    if (time_since(world->tick) < ENEMY_COOLDOWN)
        return;

    const Entity *player = world->player;

    vec2 diff = VEC2((player->pos.x - self->pos.x), fabs(player->pos.y - self->pos.y));

    if (!entity_is_moving(player)) {
        if (fabs(diff.x) < 5.f) {
            world->tick = NOW();
            entity_fire(self, world);
        }
        return;
    }

    // player is moving in the same direction
    if ((diff.x < 0.f && player->vel.x < 0.f) || (diff.x > 0.f && player->vel.x > 0.f)) 
        return;

    vec2 time = VEC2(fabs(diff.x / PLAYER_VELOCITY), fabs(diff.y / PROJECTILE_VELOCITY));

    if (fabs(diff.x) < 30.f || (fabs(time.x - time.y) < 1.f)) {
        world->tick = NOW();
        entity_fire(self, world);
    }
}

static void enemy_teleport(Entity *self, World *world) {
    if (self->pos.y > 0.f) {
        entity_set_state(self, STATE_IDLE);
        return;
    }

    entity_set_position(self, VEC2(self->pos.x, SCREEN_HEIGHT + 100.f));
    entity_set_state(self, STATE_MERGE);
}

static void enemy_determine_path(Entity *self, World *world) {
    switch (self->state) {
        case STATE_SPAWN:
            entity_clear_flag(self, FLAG_PARENT_REF);
            route_create(self, world, ROUTE_SPAWN, ENEMY_VELOCITY);
            break;
        case STATE_SWOOP:
            entity_set_flag(self, FLAG_PARENT_REF);
            route_create(self, world, ROUTE_SWOOP, ENEMY_VELOCITY);
            break;
        case STATE_MERGE:
            entity_set_flag(self, FLAG_PARENT_REF);
            route_create(self, world, ROUTE_MERGE, ENEMY_VELOCITY);
            break;
        case STATE_RETURN:
            entity_set_flag(self, FLAG_PARENT_REF);
            route_create(self, world, ROUTE_RETURN, ENEMY_VELOCITY);
            break;
        case STATE_CHARGE:
            entity_set_flag(self, FLAG_PARENT_REF);
            route_create(self, world, ROUTE_CHARGE, ENEMY_VELOCITY);
            break;
        default: break;
    }

    // LOG("path queued. items remaining: %li\n", self->path.size);
}

static void enemy_merge(Entity *self, World *world) {
    if (self->state != STATE_MERGE)
        return;

    Path *path = (Path *)queue_front(&self->path);
    assert(path);

    path->dst = formation_entity_position(world->formation, self);
}

static void enemy_travel_path(Entity *self, World *world) {
    Path *path = (Path *)queue_front(&self->path);
    assert(path);

    path_update(path, self);

    switch (self->state) {
        case STATE_MERGE:
            enemy_merge(self, world);
            break;
        default: break;
    }

    if (path->complete) {
        // LOG("path dequeued. items remaining: %li\n", self->path.size);
        entity_set_position(self, path->dst);
        entity_set_velocity(self, (vec2) { .x = 0.f, .y = 0.f });

        dequeue(&self->path);
        free(path);

        if (queue_is_empty(&self->path)) {
            queue_clear(&self->path);

            if (enemy_in_formation(self, world)) {
                assert(self->parent);

                entity_set_rotation(self, self->parent->angle);
                entity_clear_flag(self, FLAG_PARENT_REF);
            }

            switch (self->state) {
                case STATE_SPAWN:
                case STATE_RETURN:
                    entity_set_state(self, STATE_MERGE);
                    break;
                case STATE_CHARGE:
                    enemy_teleport(self, world);
                    break;
                default:
                    entity_set_state(self, STATE_IDLE);
                    break;
            }
        }
    }
}

static void enemy_path(Entity *self, World *world) {
    if (queue_is_empty(&self->path)) {
        enemy_determine_path(self, world);
    } else {
        enemy_travel_path(self, world);
    }
}

void enemy_ai(Entity *self, World *world) {
    enemy_attack(self, world);

    // TODO: cleanup code
    switch (self->state) {
        default:
            enemy_path(self, world);
            break;
    }
}

bool enemy_in_formation(Entity *self, World *world) {
    vec2 pos = formation_entity_position(world->formation, self);
    return distance(pos, self->pos) < 15.f;
}

float enemy_width(const Entity *self) {
    switch (self->type) {
        case E_ABDUCTOR:
            return ABDUCTOR_WIDTH;
        case E_INVADER:
            return INVADER_WIDTH;
        default:
            return ENEMY_WIDTH;
    }
}

float enemy_height(const Entity *self) {
    switch (self->type) {
        case E_ABDUCTOR:
            return ABDUCTOR_HEIGHT;
        case E_INVADER:
            return INVADER_HEIGHT;
        default:
            return ENEMY_HEIGHT;
    }
}

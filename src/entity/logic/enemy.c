#include "common/time.h"
#include "common/util.h"

#include "entity/logic/enemy.h"
#include "entity/entity.h"
#include "entity/logic/path.h"
#include "entity/logic/route.h"

#include "entity/formation.h"
#include "entity/player.h"
#include "entity/projectile.h"

#include "game/world.h"

#include <assert.h>
#include <math.h>

static void enemy_determine_path(Entity *self, World *world) {
    switch (self->state) {
        case STATE_SPAWN:
            route_spawn(self, ENEMY_VELOCITY);
            break;
        case STATE_SWOOP:
            entity_set_flag(self, FLAG_PARENT_REF);
            route_swoop(self, ENEMY_VELOCITY);
            break;
        case STATE_CHARGE:
        case STATE_ABDUCT:
            break;
        default:
            break;
    }

    // self->state = STATE_TRAVEL;
    // LOG("path queued. items remaining: %li\n", self->path.size);
}

static void enemy_attack(Entity *self, World *world) {
    if (time_since(world->tick) < ENEMY_COOLDOWN)
        return;

    const Entity *player = world->player;

    vec2 diff = {
        .x = (player->pos.x - self->pos.x),
        .y = fabs(player->pos.y - self->pos.y)
    };

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

    vec2 time = {
        .x = fabs(diff.x / PLAYER_VELOCITY),
        .y = fabs(diff.y / PROJECTILE_VELOCITY)
    };

    if (fabs(diff.x) < 30.f || (fabs(time.x - time.y) < 1.f)) {
        world->tick = NOW();
        entity_fire(self, world);
    }
}

static void enemy_travel_path(Entity *self, World *world) {
    Path *path = (Path *)queue_front(&self->path);
    assert(path);

    if (self->path.size == 1 && self->state == STATE_SPAWN) {
        if (!entity_has_flag(self, FLAG_PARENT_REF)) {
            // link entity to formation as reference
            entity_set_flag(self, FLAG_PARENT_REF);
        } else {
            // return to proper place in formation
            path->dst = formation_entity_position(world->formation, self->id);
        }
    }

    path_update(path, self);

    if (path->complete) {
        // LOG("path dequeued. items remaining: %li\n", self->path.size);
        entity_set_position(self, path->dst);
        entity_set_velocity(self, (vec2) { .x = 0.f, .y = 0.f });

        dequeue(&self->path);
        free(path);

        if (queue_is_empty(&self->path)) {
            queue_clear(&self->path);

            if (self->state == STATE_SPAWN) {
                assert(self->parent);

                entity_set_rotation(self, self->parent->angle);

                // clear parent reference flag
                entity_clear_flag(self, FLAG_PARENT_REF);
            }

            self->state = STATE_IDLE;
        }
    }
}

static void enemy_merge(Entity *self, World *world) {
    Path *path = (Path *)queue_front(&self->path);
    assert(path);

    if (self->path.size == 1) {
        if (!entity_has_flag(self, FLAG_PARENT_REF)) {
            // link entity to formation as reference
            entity_set_flag(self, FLAG_PARENT_REF);
        } else {
            // return to proper place in formation
            path->dst = formation_entity_position(world->formation, self->id);
        }
    }

    enemy_travel_path(self, world);
}

void enemy_ai(Entity *self, World *world) {
    state_t prev_state = self->state;
    enemy_attack(self, world);

    switch (self->state) {
        case STATE_IDLE:
            break;
        case STATE_ATTACK:
            self->state = prev_state;
            break;
        case STATE_MERGE:
            enemy_merge(self, world);
            break;
        default:
            if (!queue_is_empty(&self->path))
                enemy_travel_path(self, world);
            else
                enemy_determine_path(self, world);
            break;
    }
}

bool enemy_in_formation(Entity *self, World *world) {
    vec2 pos = formation_entity_position(world->formation, self->id);
    return distance(pos, self->pos) < 15.f;
}

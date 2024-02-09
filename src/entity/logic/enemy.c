#include "common/time.h"
#include "common/util.h"

#include "entity/logic/enemy.h"
#include "entity/logic/path.h"
#include "entity/logic/route.h"

#include "entity/abductor.h"
#include "entity/formation.h"
#include "entity/invader.h"
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
        default: break;
    }

    // entity_set_state(self, STATE_TRAVEL);
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

    path_update(path, self);

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

                // clear parent reference flag
                entity_clear_flag(self, FLAG_PARENT_REF);
            }

            switch (self->state) {
                case STATE_SPAWN:
                    route_merge(self, ENEMY_VELOCITY);
                    entity_set_state(self, STATE_MERGE);
                    break;
                default:
                    entity_set_state(self, STATE_IDLE);
                    break;
            }
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
            path->dst = formation_entity_position(world->formation, self);
        }
    }

    enemy_travel_path(self, world);
}

void enemy_ai(Entity *self, World *world) {
    enemy_attack(self, world);

    switch (self->state) {
        case STATE_IDLE:
            break;
        case STATE_ATTACK:
            entity_set_state(self, self->prev_state);
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

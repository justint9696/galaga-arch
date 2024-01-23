#include "common/time.h"

#include "entity/logic/enemy.h"
#include "common/util.h"
#include "entity/entity.h"
#include "entity/logic/path.h"
#include "entity/logic/route.h"

#include "entity/formation.h"
#include "entity/player.h"
#include "entity/projectile.h"

#include "game/stage.h"
#include "game/world.h"

#include <assert.h>
#include <math.h>

static void enemy_determine_path(Entity *self, World *world) {
    switch (self->state) {
        case STATE_SPAWN:
            route_spawn(self, ENEMY_VELOCITY);
            break;
        case STATE_SWOOP:
            entity_unlink(self);
            route_swoop(self, ENEMY_VELOCITY);
            break;
        case STATE_CHARGE:
        case STATE_ABDUCT:
            break;
        default:
            break;
    }

    self->state = STATE_TRAVEL;
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

static void enemy_travel(Entity *self, World *world) {
    Path *path = (Path *)queue_front(&self->path);
    assert(path);

    // when queue is empty, determine next path based on enemy type
    // abductor will go into the abduct state 
    // invader will go into charge state
    // if (queue_is_empty(&self->path)) {
    //     switch (self->type) {
    //         case E_ABDUCTOR:
    //             self->state = STATE_ABDUCT;
    //             break;
    //         case E_INVADER:
    //             self->state = STATE_CHARGE;
    //             break;
    //         default: break;
    //     }
    // }

    if (self->path.size == 1) {
        if (!self->parent) {
            // link entity to formation as reference
            entity_link(self, world->formation);
            entity_set_flag(self, FLAG_PARENT_REF);
        } else {
            // return to proper place in formation
            path->dst = entity_tag(self->parent, TAG_TOP_LEFT);
            path->dst.x += ((int)fmod(self->id, FORMATION_ROW_MAX) * (self->dim.width + FORMATION_DISTANCE)) + (self->dim.width / 2.f);
            path->dst.y -= ((int)(self->id / FORMATION_ROW_MAX) * (self->dim.height + FORMATION_DISTANCE)) + (self->dim.height / 2.f);
        }
    }

    path_update(path, self);

    if (path->complete) {
        // LOG("path dequeued. items remaining: %li\n", self->path.size);
        entity_set_position(self, path->destintation);
        entity_set_velocity(self, (vec2) { .x = 0.f, .y = 0.f });

        dequeue(&self->path);
        free(path);

        if (queue_is_empty(&self->path)) {
            queue_clear(&self->path);

            self->state = STATE_IDLE;

            assert(self->parent);
            entity_set_rotation(self, self->parent->angle);

            // clear parent reference flag
            entity_clear_flag(self, FLAG_PARENT_REF);
        }
    }
}

static bool enemy_waves_spawned(World *world) {
    return (world->enemies.size >= (WAVE_COUNT * WAVE_MAX));
}

static void enemy_randomize(Entity *self, World *world) {
    // ensure all enemies are spawned
    if (!enemy_waves_spawned(world))
        return;

    // ensure enough time has passed since last swoop
    if (time_since(world->idle_tick) < ENEMY_IDLE_TIME)
        return;

    // ensure there is a delay after 2 entities swoop
    if (world->swoop_count && time_since(world->idle_tick) < (world->swoop_count * ENEMY_IDLE_TIME))
        return;

    world->swoop_count = (world->swoop_count % 2);
    world->swoop_count++;

    world->idle_tick = NOW();

    // TODO: look into a more optimizing this
    uint32_t index = RAND(world->enemies.size);
    Entity *e = (Entity *)list_get_index(&world->enemies, index);
    e->state = STATE_SWOOP;
}

void enemy_ai(Entity *self, World *world) {
    state_t prev_state = self->state;
    enemy_attack(self, world);

    switch (self->state) {
        case STATE_IDLE:
            // enemy_randomize(self, world);
            break;
        case STATE_ATTACK:
            self->state = prev_state;
            break;
        case STATE_TRAVEL:
            enemy_travel(self, world);
            break;
        default:
            enemy_determine_path(self, world);
            break;
    }
}

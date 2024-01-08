#include "common/util.h"

#include "entity/logic/path.h"
#include "entity/logic/route.h"

#include "entity/enemy.h"
#include "entity/formation.h"
#include "entity/player.h"
#include "entity/projectile.h"

#include "game/world.h"

#include <assert.h>
#include <math.h>

static void enemy_spawn(Entity *self) {
    route_spawn(self, ENEMY_SPAWN_VELOCITY); 
    self->state = STATE_TRAVEL;
    // LOG("path queued. items remaining: %li\n", self->path.size);
}

static void enemy_attack(Entity *self, World *world) {
    const Entity *player = world->player;

    vec2 diff = {
        .x = (player->pos.x - self->pos.x),
        .y = fabs(player->pos.y - self->pos.y)
    };

    if (!entity_is_moving(player)) {
        if (fabs(diff.x) < 5.f)
            entity_fire(self, world, PROJECTILE_COOLDOWN);

        return;
    }

    // player is moving in the same direction
    if ((diff.x < 0.f && player->vel.x < 0.f) || (diff.x > 0.f && player->vel.x > 0.f)) 
        return;

    vec2 time = {
        .x = fabs(diff.x / PLAYER_VELOCITY),
        .y = fabs(diff.y / PROJECTILE_VELOCITY)
    };

    if (fabs(diff.x) < 30.f || (fabs(time.x - time.y) < 1.f))
        entity_fire(self, world, PROJECTILE_COOLDOWN);
}

static void enemy_travel(Entity *self, World *world) {
    Path *path = (Path *)queue_front(&self->path);
    assert(path);

    if (self->path.size == 1) {
        if (!self->parent) {
            // link entity to formation as reference
            entity_link(self, world->formation);
            self->flags |= FLAG_PARENT_REF;
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

        if (!self->path.size) {
            queue_clear(&self->path);

            self->state = STATE_IDLE;

            assert(self->parent);
            entity_set_rotation(self, self->parent->angle);

            // clear parent reference flag
            self->flags &= ~FLAG_PARENT_REF;
        }
    }
}

static void enemy_swoop(Entity *self);

void enemy_init(Entity *self, World *world) {
    self->dim = (vec2) {
        .width = ENEMY_WIDTH,
        .height = ENEMY_HEIGHT,
    };
    self->team = TEAM_AXIS;
    self->texture = load_texture(ENEMY_TEXTURE);
    self->health = 1.f;
    self->state = STATE_SPAWN;
    self->flags = FLAG_COLLISION;

    LOG("enemy initialized\n");
}

static void destroy_queue(Queue *q) {
    Path *p;
    size_t size = q->size;
    for (size_t i = 0; i < size; i++) {
        p = (Path *)queue_front(q);
        dequeue(q);
        free(p);
    }
}

void enemy_destroy(Entity *self, World *world) {
    destroy_queue(&self->path);
}

void enemy_update(Entity *self, World *world) {
    state_t prev_state = self->state;
    // enemy_attack(self);

    switch (self->state) {
        case STATE_SPAWN:
            enemy_spawn(self);
            break;
        case STATE_ATTACK:
            self->state = prev_state;
            break;
        case STATE_TRAVEL:
            enemy_travel(self, world);
            break;
        default:
            break;
    }
}

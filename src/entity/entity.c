#include "common/util.h"
#include "common/time.h"

#include "game/world.h"
#include "gfx/window.h"

#include "entity/entity.h"
#include "entity/pool.h"

#include "entity/abductor.h"
#include "entity/formation.h"
#include "entity/invader.h"
#include "entity/player.h"
#include "entity/projectile.h"
#include "entity/star.h"
#include "entity/tractor_beam.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static void entity_render_rect(Entity *self) {
    draw_rect(VEC2(round(self->pos.x), round(self->pos.y)), self->dim, self->color, self->depth);
}

static void entity_render_texture(Entity *self) {
    draw_texture(self->texture, VEC2(round(self->pos.x), round(self->pos.y)), self->dim, self->angle, self->depth);
}

Entity *entity_create(entity_t type, World *world) {
    Entity *self = entity_request();
    assert(self);

    switch ((self->type = type)) {
        case E_PLAYER:
            entity_init(self, player_init, NULL, NULL, NULL, world);
            break;
        case E_INVADER:
            entity_init(self, invader_init, invader_destroy, invader_update, NULL, world);
            break;
        case E_ABDUCTOR:
            entity_init(self, abductor_init, abductor_destroy, abductor_update, NULL, world);
            break;
        case E_PROJECTILE:
            entity_init(self, projectile_init, NULL, NULL, projectile_collide, world);
            break;
        case E_STAR:
            entity_init(self, star_init, NULL, star_update, NULL, world);
            break;
        case E_FORMATION:
            entity_init(self, formation_init, NULL, formation_update, NULL, world);
            break;
        case E_TRACTOR_BEAM:
            entity_init(self, tractor_beam_init, tractor_beam_destroy, tractor_beam_update, tractor_beam_collide, world);
            break;
    }

    return self;
}

void entity_init(Entity *self, entity_f init, entity_f destroy, entity_f update, entity_collide_f collide, World *world) {
    if ((self->init = init) != NULL)
        self->init(self, world);

    self->collide = collide;
    self->destroy = destroy;
    self->update = update;

    if (!self->render) {
        if (self->texture)
            self->render = entity_render_texture;
        else if (self->color)
            self->render = entity_render_rect;
    }
}

void entity_destroy(Entity *self, World *world) {
    if (self->destroy != NULL)
        self->destroy(self, world);

    entity_release(self);
}

// out of bounds check
bool entity_oob(Entity *self) {
    return ((self->pos.x > (SCREEN_WIDTH + SCREEN_BUFFER) || self->pos.x < -SCREEN_BUFFER) || 
            (self->pos.y > (SCREEN_HEIGHT + SCREEN_BUFFER) || self->pos.y < -SCREEN_BUFFER));
}

void entity_update(Entity *self, World *world) {
    uint32_t delta = time_delta();
    if (self->update != NULL)
        self->update(self, world);

    // store entity position before update
    self->prev_pos = self->pos;

    // update entity position
    // if entity parent defined && parent reference flag not set
    if (self->parent && !entity_has_flag(self, FLAG_PARENT_REF)) {
        self->pos.x += round(1.f * delta * self->parent->vel.x);
        self->pos.y += round(1.f * delta * self->parent->vel.y);
    } else {
        self->pos.x += round(1.f * delta * self->vel.x);
        self->pos.y += round(1.f * delta * self->vel.y);
    }

    // clamp player within scene
    switch (self->type) {
        case E_PLAYER:
            self->pos.x = clamp(0, self->pos.x, SCREEN_WIDTH - self->dim.w);
            self->pos.y = clamp(0, self->pos.y, SCREEN_HEIGHT - self->dim.h);
            break;
        default: 
            break;
    } 

    // if entity is outside of the scene
    if (entity_oob(self))
        entity_set_state(self, STATE_DEAD);

    if (self->render != NULL)
        self->render(self);
}

bool entity_collide(Entity *self, Entity *entity, World *world) {
    entity_collide_f f;
    if ((f = self->collide) != NULL)
        f(self, entity, world);

    return f != NULL;
}

bool entity_is_alive(const Entity *self) {
    return (self->health && self->state != STATE_DEAD);
}

bool entity_is_moving(const Entity *self) {
    vec2 zero = { 0 };
    return memcmp(&self->vel, &zero, sizeof(vec2));
}

bool entity_collision(const Entity *e0, const Entity *e1) {
    return ((e0->pos.x + e0->dim.width) >= e1->pos.x 
            && (e0->pos.y + e0->dim.height) >= e1->pos.y)
            && e0->pos.x <= (e1->pos.x + e1->dim.width)
            && e0->pos.y <= (e1->pos.y + e1->dim.height);
}

vec2 entity_tag(const Entity *self, tag_t tag) {
    switch (tag) {
        case TAG_CENTER:
            return VEC2(self->pos.x + (self->dim.width / 2.f), self->pos.y + (self->dim.height / 2.f));
        case TAG_TOP_LEFT:
            return VEC2(self->pos.x, self->pos.y + self->dim.height);
        case TAG_TOP_RIGHT:
            return VEC2(self->pos.x + self->dim.width, self->pos.y + self->dim.height);
        case TAG_TOP_MIDDLE:
            return VEC2(self->pos.x + (self->dim.width / 2.f), self->pos.y + self->dim.height);
        case TAG_BOTTOM_LEFT:
            return VEC2(self->pos.x, self->pos.y);
        case TAG_BOTTOM_RIGHT:
            return VEC2(self->pos.x + self->dim.width, self->pos.y);
        case TAG_BOTTOM_MIDDLE:
            return VEC2(self->pos.x + (self->dim.width / 2.f), self->pos.y);
        case TAG_MIDDLE_LEFT:
            return VEC2(self->pos.x, self->pos.y + (self->dim.height / 2.f));
        case TAG_MIDDLE_RIGHT:
            return VEC2(self->pos.x + self->dim.width, self->pos.y + (self->dim.height / 2.f));
        default:
            return self->pos;
    }
}

vec2 entity_displacement(const Entity *self) {
    return VEC2(self->pos.x - self->prev_pos.x, self->pos.y - self->prev_pos.y);
}

inline void entity_link(Entity *self, Entity *parent) {
    self->parent = parent;
    parent->child = self;
}

inline void entity_unlink(Entity *self) {
    self->parent->child = NULL;
    self->parent = NULL;
}

inline void entity_set_flag(Entity *self, flag_t flag) {
    self->flags |= flag;
}

inline void entity_clear_flag(Entity *self, flag_t flag) {
    self->flags &= ~flag;
}

inline bool entity_has_flag(Entity *self, flag_t flag) {
    return self->flags & flag;
}

void entity_damage(Entity *self) {
    switch (self->type) {
        case E_PROJECTILE:
            entity_set_state(self, STATE_DEAD);
            break;
        default:
            if (--self->health <= 0.f)
                entity_set_state(self, STATE_DEAD);
            break;
    }
}

void entity_fire(Entity *self, World *world) {
    if (time_since(self->tick) < PROJECTILE_COOLDOWN)
        return;

    self->tick = NOW();

    Entity *e = entity_create(E_PROJECTILE, world);
    e->team = self->team;
    e->pos = entity_tag(self, self->type == E_PLAYER ? TAG_TOP_MIDDLE : TAG_BOTTOM_MIDDLE);
    e->vel = VEC2(0.f, self->team == TEAM_ALLY ? PROJECTILE_VELOCITY : -PROJECTILE_VELOCITY);

    world_add_entity(world, e);
}

void entity_transform(Entity *self, vec2 dim) {
    vec2 diff = {
        .x = dim.x - self->dim.x,
        .y = dim.y - self->dim.y
    };
    self->dim = dim;

    self->pos.x -= (diff.x / 2.f);
    self->pos.y -= (diff.y / 2.f);
}

void entity_set_position(Entity *self, vec2 pos) {
    memcpy(&self->pos, &pos, sizeof(vec2));
}

void entity_set_velocity(Entity *self, vec2 vel) {
    memcpy(&self->vel, &vel, sizeof(vec2));
}

void entity_set_rotation(Entity *self, float angle) {
    self->angle = angle;
}

void entity_set_state(Entity *self, state_t state) {
    if (self->prev_state != self->state)
        self->prev_state = self->state;

    self->state = state;
}

#include "common/util.h"
#include "common/time.h"

#include "game/world.h"
#include "gfx/window.h"

#include "entity/entity.h"
#include "entity/pool.h"
#include "entity/projectile.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static void entity_render_rect(Entity *self, World *world) {
    draw_rect(round(self->pos.x), round(self->pos.y), self->dim.width, self->dim.height, self->color);
}

static void entity_render_texture(Entity *self, World *world) {
    draw_texture(self->texture, round(self->pos.x), round(self->pos.y), self->dim.width, self->dim.height, self->angle);
}

Entity *entity_init(entity_t type, entity_f init, entity_f destroy, entity_f update, World *world) {
    Entity *self = entity_request();
    assert(self);

    self->init = init;
    self->init(self, world);

    self->destroy = destroy;
    self->update = update;

    if (self->texture != NULL)
        self->render = entity_render_texture;
    else if (self->color)
        self->render = entity_render_rect;
    else 
        self->render = NULL;

    self->type = type;

    return self;
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
    if (entity_oob(self)) {
        self->state = STATE_DEAD;
    }

    if (self->render != NULL)
        self->render(self, world);
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
            return (vec2) {
                .x = self->pos.x + (self->dim.width / 2.f),
                .y = self->pos.y + (self->dim.height / 2.f),
            };
        case TAG_TOP_LEFT:
            return (vec2) {
                .x = self->pos.x,
                .y = self->pos.y + self->dim.height,
            };
        case TAG_TOP_RIGHT:
            return (vec2) {
                .x = self->pos.x + self->dim.width,
                .y = self->pos.y + self->dim.height,
            };
        case TAG_TOP_MIDDLE:
            return (vec2) {
                .x = self->pos.x + (self->dim.width / 2.f),
                .y = self->pos.y + self->dim.height,
            };
        case TAG_BOTTOM_LEFT:
            return (vec2) {
                .x = self->pos.x,
                .y = self->pos.y,
            };
        case TAG_BOTTOM_RIGHT:
            return (vec2) {
                .x = self->pos.x + self->dim.width,
                .y = self->pos.y,
            };
        case TAG_BOTTOM_MIDDLE:
            return (vec2) {
                .x = self->pos.x + (self->dim.width / 2.f),
                .y = self->pos.y,
            };
        case TAG_MIDDLE_LEFT:
            return (vec2) {
                .x = self->pos.x,
                .y = self->pos.y + (self->dim.height / 2.f),
            };
        case TAG_MIDDLE_RIGHT:
            return (vec2) {
                .x = self->pos.x + self->dim.width,
                .y = self->pos.y + (self->dim.height / 2.f),
            };
        default:
            return self->pos;
    }
}

void entity_link(Entity *self, Entity *parent) {
    self->parent = parent;
}

void entity_unlink(Entity *self) {
    self->parent = NULL;
}

void entity_set_flag(Entity *self, flag_t flag) {
    self->flags |= flag;
}

void entity_clear_flag(Entity *self, flag_t flag) {
    self->flags &= ~flag;
}

bool entity_has_flag(Entity *self, flag_t flag) {
    return self->flags & flag;
}

void entity_damage(Entity *self) {
    switch (self->type) {
        case E_PROJECTILE:
            self->state = STATE_DEAD;
            break;
        default:
            if (--self->health <= 0.f)
                self->state = STATE_DEAD;
            break;
    }
}

void entity_fire(Entity *self, World *world, uint32_t delay) {
    if (time_since(self->tick) < PROJECTILE_COOLDOWN)
        return;

    self->tick = NOW();

    Entity *e = entity_init(E_PROJECTILE, projectile_init, NULL, NULL, world);
    e->team = self->team;
    e->pos = entity_tag(self, self->type == E_PLAYER ? TAG_TOP_MIDDLE : TAG_BOTTOM_MIDDLE);
    e->vel = (vec2) {
        .x = 0.f,
        .y = self->team == TEAM_ALLY ? PROJECTILE_VELOCITY : -PROJECTILE_VELOCITY,
    };

    world_add_entity(world, e);
}

void entity_set_position(Entity *self, vec2 pos) {
    memcpy(&self->pos, &pos, sizeof(vec2));
}

void entity_set_velocity(Entity *self, vec2 vel) {
    memcpy(&self->vel, &vel, sizeof(vec2));
}

void entity_set_rotation(Entity *self, float angle) {
    memcpy(&self->angle, &angle, sizeof(float));
}

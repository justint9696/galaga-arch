#include "entity/entity.h"
#include "gfx/app.h"
#include "gfx/renderer.h"
#include "gfx/window.h"
#include "common/util.h"
#include "game/time.h"

#include <assert.h>

static size_t _count = 0;

static inline void _Entity_Render_Rect(const Entity *self) {
    DrawRect(round(self->pos.x), round(self->pos.y), self->dim.width, self->dim.height, self->color);
}

static inline void _Entity_Render_Texure(const Entity *self) {
    DrawTexture(self->texture, round(self->pos.x), round(self->pos.y), self->dim.width, self->dim.height, self->rotation);
}

// out of bounds check
static inline bool _Entity_OOB(const Entity *self) {
    return ((self->pos.x > (WINDOW_WIDTH + WINDOW_BUFFER) || self->pos.x < -WINDOW_BUFFER) || 
            (self->pos.y > (WINDOW_HEIGHT + WINDOW_BUFFER) || self->pos.y < -WINDOW_BUFFER));
}

static inline vec2 _Entity_Midpoint(const Entity *self) {
    return (vec2) {
        .x = self->pos.x + (self->dim.width / 2.f),
        .y = self->pos.y + (self->type == TYPE_PLAYER ? self->dim.height : 0.f)
    };
}

team_t Entity_GetOtherTeam(const team_t team) {
    return (team == TEAM_ALLY ? TEAM_AXIS : TEAM_ALLY);
}

bool Entity_IsAlive(const Entity *self) {
    return (self->state == STATE_ALIVE && self->health);
}

bool Entity_Collision(const Entity *e0, const Entity *e1) {
    return ((e0->pos.x + e0->dim.width) >= e1->pos.x 
            && (e0->pos.y + e0->dim.height) >= e1->pos.y)
            && e0->pos.x <= (e1->pos.x + e1->dim.width)
            && e0->pos.y <= (e1->pos.y + e1->dim.height);        
}

static void _collision_handler(Entity *self) {
    switch (self->type) {
        case TYPE_PROJECTILE:
            self->state = STATE_DEAD;
            break;
        default:
            if (!--self->health)
                self->state = STATE_DEAD;
            break;
    }
}

void Entity_Collide(Entity *self, Entity *entity) {
    _collision_handler(self);
    _collision_handler(entity);
}

void Entity_Init(Entity *self, type_t type, team_t team, float health, float x, float y, int width, int height, const char *texture) {
    self->id = _count++;
    self->health = health;

    self->type = type;
    self->team = team;
    self->state = STATE_ALIVE;

    self->pos = (vec2) { .x = x, .y = y };
    self->dim = (vec2) { .width = width, .height = height };

    self->texture = texture == NULL ? NULL : LoadTexture(texture);

    switch (type) {
        case TYPE_PLAYER:
            self->color = COLOR_PLAYER;
            break;
        case TYPE_ENEMY:
            self->color = COLOR_ENEMY;
            break;
        case TYPE_PROJECTILE:
            self->color = COLOR_PROJECTILE;
            break;
        case TYPE_FORMATION:
            self->color = COLOR_RED;
            break;
        default:
            fprintf(stderr, "ERROR: Unknown entity type: %i.\n", type);
            exit(1);
            break;
    }

    self->render = texture == NULL ? _Entity_Render_Rect : _Entity_Render_Texure;
}

void Entity_Update(Entity *self, uint64_t deltaTime) {
    if (_Entity_OOB(self)) {
        self->state = STATE_DEAD;
        return;
    }

    // update entity position
    if (self->parent) {
        self->pos.x += (1.f * deltaTime * self->parent->vel.x);
        self->pos.y += (1.f * deltaTime * self->parent->vel.y);
    } else {
        self->pos.x += (1.f * deltaTime * self->vel.x);
        self->pos.y += (1.f * deltaTime * self->vel.y);
    }

    self->delta = deltaTime;

    // clamp player within scene
    switch (self->type) {
        case TYPE_PLAYER:
            self->pos.x = clamp(0, self->pos.x, WINDOW_WIDTH - self->dim.width);
            self->pos.y = clamp(0, self->pos.y, WINDOW_HEIGHT - self->dim.height);
            break;
        default:
            break;
    }

    self->render(self);
}

Entity *Entity_Fire(Entity *self, uint64_t tick) {
    const uint64_t time = Time_Passed(self->tick);
    if (time < BULLET_DELAY)
        return NULL;

    vec2 pos = _Entity_Midpoint(self);

    vec2 vel = {
        .x = 0.f,
        .y = self->team == TEAM_ALLY ? BULLET_VELOCITY : -BULLET_VELOCITY,
    };

    Entity *entity = calloc(1, sizeof(Entity));

    Entity_Init(entity, TYPE_PROJECTILE, self->team, 1.f, pos.x, pos.y, BULLET_WIDTH, BULLET_HEIGHT, BULLET_TEXTURE);
    Entity_SetVelocity(entity, vel);

    self->tick = tick;

    return entity;
}

void Entity_LinkTo(Entity *self, Entity *entity) {
    self->parent = entity; 
}

void Entity_Unlink(Entity *self) {
    self->parent = NULL;
}

void Entity_SetPosition(Entity *self, vec2 pos) {
    memcpy(&self->pos, &pos, sizeof(vec2));
}

void Entity_SetVelocity(Entity *self, vec2 vel) {
    memcpy(&self->vel, &vel, sizeof(vec2));
}

void Entity_SetRotation(Entity *self, float angle) {
    memcpy(&self->rotation, &angle, sizeof(float)); 
}

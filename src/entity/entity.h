#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "common/type.h"
#include "data/queue.h"

#include <SDL2/SDL_image.h>

#include <stdbool.h>
#include <stdint.h>

#define ENTITY_MAX 640

typedef struct World World;

typedef enum {
    E_PLAYER,
    E_INVADER,
    E_ABDUCTOR,
    E_PROJECTILE,
    E_STAR,
    E_FORMATION,
    E_TRACTOR_BEAM,
} entity_t;

typedef enum {
    FLAG_ACTIVE             = 1 << 0,
    FLAG_COLLISION          = 1 << 1,
    FLAG_PARENT_REF         = 1 << 2,
    FLAG_AI_CONTROLLED      = 1 << 3,
    FLAG_PLAYER_CONTROLLED  = 1 << 4
} flag_t;

typedef enum {
    STATE_DEAD = 0,
    STATE_ALIVE,
    STATE_FADE_IN,
    STATE_FADE_OUT,
    STATE_IDLE,
    STATE_SPAWN,
    STATE_MERGE,
    STATE_RETURN,
    STATE_TRAVEL,
    STATE_SWOOP,
    STATE_CHARGE,
    STATE_ABDUCT,
    STATE_EXPAND,
    STATE_RETRACT,
} state_t;

typedef enum {
    TAG_CENTER,
    TAG_TOP_LEFT,
    TAG_TOP_RIGHT,
    TAG_TOP_MIDDLE,
    TAG_BOTTOM_LEFT,
    TAG_BOTTOM_RIGHT,
    TAG_BOTTOM_MIDDLE,
    TAG_MIDDLE_LEFT,
    TAG_MIDDLE_RIGHT,
} tag_t;

typedef enum {
    TEAM_ALLY,
    TEAM_AXIS,
} team_t;

typedef struct Entity Entity;

typedef void(*entity_f)(Entity *, World *);
typedef void(*entity_collide_f)(Entity *, Entity *, World *);
typedef void(*entity_render_f)(Entity *);

struct Entity {
    // arbitrary id used to determine enemy location on formation
    uint32_t id;

    // index of entity within the entity pool
    uint32_t index;

    // depth of sprite within the renderer
    uint32_t depth;

    entity_t type;
    state_t state, prev_state;
    team_t team;
    vec2 prev_pos;
    vec2 dim, pos, vel;
    float health, angle;
    Entity *parent, *child;
    uint32_t color;
    SDL_Texture *texture;
    Queue path;
    uint32_t flags;
    uint32_t tick;

    entity_f init, destroy, update;
    entity_collide_f collide;
    entity_render_f render;
};

Entity *entity_create(entity_t, World *);
void entity_init(Entity *, entity_f init, entity_f destroy, entity_f update, entity_collide_f collide, World *);

// Entity *entity_init(entity_t, entity_f init, entity_f destroy, entity_f update, World *);
void entity_destroy(Entity *, World *);
void entity_update(Entity *, World *);
bool entity_collide(Entity *, Entity *, World *);

bool entity_is_alive(const Entity *);
bool entity_is_moving(const Entity *);
bool entity_collision(const Entity *, const Entity *);

vec2 entity_tag(const Entity *, tag_t);
vec2 entity_displacement(const Entity *);

void entity_link(Entity *, Entity *);
void entity_unlink(Entity *);

void entity_set_flag(Entity *, flag_t);
void entity_clear_flag(Entity *, flag_t);
bool entity_has_flag(Entity *, flag_t);

void entity_damage(Entity *);
void entity_fire(Entity *, World *);

void entity_transform(Entity *, vec2);

void entity_set_position(Entity *, vec2);
void entity_set_velocity(Entity *, vec2);
void entity_set_rotation(Entity *, float);
void entity_set_state(Entity *, state_t);

#endif

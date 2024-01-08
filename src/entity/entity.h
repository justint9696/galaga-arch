#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "common/type.h"
#include "data/queue.h"

#include <SDL2/SDL_image.h>

#include <stdbool.h>
#include <stdint.h>

struct World;

typedef enum {
    E_PLAYER,
    E_ENEMY,
    E_PROJECTILE,
    E_STAR,
    E_FORMATION,
    E_MAX,
} entity_t;

#define ENTITY_MAX 640

typedef enum {
    TEAM_ALLY,
    TEAM_AXIS,
} team_t;

typedef enum {
    STATE_DEAD,
    STATE_ALIVE,
    STATE_FADE_IN,
    STATE_FADE_OUT,
    STATE_IDLE,
    STATE_SPAWN,
    STATE_TRAVEL,
    STATE_ATTACK,
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

struct Entity;
typedef void(*entity_f)(struct Entity *, struct World *);

typedef enum {
    FLAG_COLLISION = 1 << 0,
    FLAG_PARENT_REF = 1 << 1,
} flag_t;

typedef struct Entity {
    // arbitrary id used to determine enemy location on formation
    uint32_t id;

    // index of entity within the entity pool
    uint32_t index;

    entity_t type;
    state_t state;
    team_t team;
    vec2 dim, pos, vel;
    float health, angle;
    const struct Entity *parent;
    struct Entity *child;
    uint32_t color;
    SDL_Texture *texture;
    Queue path;
    uint32_t flags;
    uint32_t tick;

    entity_f init, destroy, render, update;
} Entity;

Entity *entity_init(entity_t, entity_f init, entity_f destroy, entity_f update, struct World *);
void entity_destroy(Entity *, struct World *);
void entity_update(Entity *, struct World *);

bool entity_is_alive(const Entity *);
bool entity_is_moving(const Entity *);
bool entity_collision(const Entity *, const Entity *);

vec2 entity_tag(const Entity *, tag_t);

void entity_link(Entity *, Entity *);
void entity_unlink(Entity *);

void entity_damage(Entity *);
void entity_fire(Entity *, struct World *, uint32_t delay);

void entity_set_position(Entity *, vec2);
void entity_set_velocity(Entity *, vec2);
void entity_set_rotation(Entity *, float);

#endif

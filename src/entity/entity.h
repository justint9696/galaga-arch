#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "common/type.h"
#include "data/linked_list.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

#define BULLET_DELAY        250
#define BULLET_DISTANCE     50.f
#define BULLET_TEXTURE      NULL
#define BULLET_VELOCITY     1.f
#define BULLET_WIDTH        5.f
#define BULLET_HEIGHT       15.f

#define COLOR_PLAYER        COLOR_WHITE
#define COLOR_ENEMY         COLOR_BLUE
#define COLOR_PROJECTILE    COLOR_GREEN

typedef enum {
    TYPE_PLAYER,
    TYPE_ENEMY,
    TYPE_PROJECTILE,
    TYPE_FORMATION,
} type_t;

typedef enum {
    TEAM_ALLY,
    TEAM_AXIS,
} team_t;

typedef enum {
    STATE_DEAD,
    STATE_ALIVE,
} state_t;

typedef struct Entity_s {
    uint32_t id;
    float health, rotation;
    type_t type;
    team_t team;
    state_t state;
    vec2 vel, pos, dim;
    uint32_t color;
    uint64_t tick, delta;
    SDL_Texture *texture;
    struct Entity_s *parent;
    void(*render)(const struct Entity_s *);
} Entity;

team_t Entity_GetOtherTeam(const team_t);
bool Entity_IsAlive(const Entity *self);
bool Entity_Collision(const Entity *e0, const Entity *e1);
void Entity_Collide(Entity *self, Entity *entity);

void Entity_Init(Entity *self, type_t type, team_t team, float health, float x, float y, int width, int height, const char *texture);

void Entity_Update(Entity *self, uint64_t deltaTime);
Entity *Entity_Fire(Entity *self, uint64_t tick);

void Entity_LinkTo(Entity *self, Entity *entity);
void Entity_Unlink(Entity *self);

void Entity_SetVelocity(Entity *self, vec2 vel);
void Entity_SetPosition(Entity *self, vec2 pos);
void Entity_SetRotation(Entity *self, float angle);

#endif

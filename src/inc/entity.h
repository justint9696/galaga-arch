#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "type.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

#define BULLET_DELAY        250 
#define BULLET_DISTANCE     50.f
#define BULLET_TEXTURE      NULL
#define BULLET_VELOCITY     1.f
#define BULLET_WIDTH        5.f
#define BULLET_HEIGHT       15.f

#define COLOR_PLAYER        0xFFFFFFFF
#define COLOR_ENEMY         0xFF0000FF
#define COLOR_PROJECTILE    0x00FF00FF

typedef enum {
    TYPE_PLAYER,
    TYPE_ENEMY,
    TYPE_PROJECTILE,
} type_t;

typedef enum {
    TEAM_ALLY,
    TEAM_ENEMY,
} team_t;

typedef struct {
    int id;
    type_t type;
    team_t team;
    int width, height;
    uint64_t tick;
    vec2 pos, vel;
    SDL_Texture *texture;
    uint32_t color;
    void *render;
} Entity;

float Distance(const vec2 pt1, const vec2 pt2);

void Entity_InitList();

Entity *Entity_Init(type_t type, team_t team, float x, float y, int width, int height, const char *texture);

void Entity_UpdateAll(uint64_t deltaTime);

void Entity_SetPosition(Entity *self, vec2 pos);

void Entity_SetVelocity(Entity *self, vec2 vel);

void Entity_Fire(Entity *self, uint64_t tick);

#endif

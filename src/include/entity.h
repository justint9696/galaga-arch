#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SDL2/SDL.h>

typedef struct {
    float x, y;
} vec2;

typedef struct {
    int index;
    int width, height;
    vec2 pos, vel;
    SDL_Texture *texture;
    uint32_t color;
} Entity;

Entity *Entity_Init(float x, float y, int width, int height, const char *texture);

void Entity_Update(Entity *self);

void Entity_SetPosition(Entity *self, float x, float y);

void Entity_SetVelocity(Entity *self, float vx, float vy);

#endif
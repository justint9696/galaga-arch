#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SDL2/SDL.h>

typedef struct {
    float x, y;
} vec2;

typedef struct {
    int width, height;
    vec2 pos, vel;
    SDL_Texture *texture;
    uint32_t color;
} Entity;

void initEntity(Entity *ent, float x, float y, char *texture);

void SetPosition(Entity *ent, float x, float y);

void SetVelocity(Entity *ent, float vx, float vy);

SDL_Texture *loadTexture(char *filename);

#endif
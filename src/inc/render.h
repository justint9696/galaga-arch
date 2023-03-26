#ifndef _RENDER_H_
#define _RENDER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

void Renderer_Init(SDL_Renderer *renderer);

void Renderer_Prepare();

void Renderer_Update();

void DrawRect(float x, float y, int width, int height, uint64_t color);

void DrawTexture(SDL_Texture *texture, float x, float y, int width, int height);

#endif

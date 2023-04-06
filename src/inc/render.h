#ifndef _RENDER_H_
#define _RENDER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

void Renderer_Init(SDL_Renderer *renderer);

void Renderer_SetFont(TTF_Font *font);

void Renderer_Prepare();

void Renderer_Update();

void DrawRect(int x, int y, int width, int height, uint32_t color);

void DrawTexture(SDL_Texture *texture, int x, int y, int width, int height);

void DrawText(const char *text, int x, int y, uint32_t color);

#endif

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED)

typedef struct {
    SDL_Renderer *handle;
    TTF_Font *font;
} Renderer;

Renderer *renderer_init(SDL_Window *);
void renderer_destroy();

void renderer_prepare();
void renderer_present();
void renderer_set_font(TTF_Font *);

void draw_text();
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_texture(SDL_Texture *, int x, int y, int w, int h, float angle);

SDL_Texture *load_texture(const char *path);
TTF_Font *load_font(const char *path, uint32_t sizefont);

#endif

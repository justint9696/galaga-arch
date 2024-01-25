#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED)

#define FONT_ONE    "assets/fonts/press-start-2p-font/font.ttf"
#define FONT_TWO    "assets/fonts/galaxus-font/font.ttf"

#define TEX_ONE     "assets/images/player.png"
#define TEX_TWO     "assets/images/enemy01.png"
#define TEX_THREE   "assets/images/enemy02.png"

typedef enum {
    FONT_NORMAL,
    FONT_SMALL,
    FONT_BIG,
} font_t;

#define FONT_MAX FONT_BIG + 1

typedef enum {
    TEX_PLAYER,
    TEX_INVADER,
    TEX_ABDUCTOR,
} texture_t;

#define TEX_MAX TEX_ABDUCTOR + 1

typedef struct {
    SDL_Renderer *handle;
    TTF_Font *fonts[FONT_MAX];
    SDL_Texture *textures[TEX_MAX];
} Renderer;

Renderer *renderer_init(SDL_Window *);
void renderer_destroy();

void renderer_prepare();
void renderer_present();

TTF_Font *renderer_font_handle(font_t);
SDL_Texture *renderer_texture_handle(texture_t);

void draw_text(const char *text, int x, int y, uint32_t color, TTF_Font *font);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_texture(SDL_Texture *, int x, int y, int w, int h, float angle);

uint32_t font_width(const char *text, font_t);
uint32_t font_height(font_t);

#endif

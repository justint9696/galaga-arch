#ifndef _GFX_RENDERER_H_
#define _GFX_RENDERER_H_

#include "common/type.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#define LAYER_BASE          0
#define LAYER_PROJECTILE    1
#define LAYER_CHARACTER     2
#define LAYER_UI            3

#define RENDERER_FLAGS      (SDL_RENDERER_ACCELERATED)

#define FONT_ONE            "assets/fonts/press-start-2p-font/font.ttf"
#define FONT_TWO            "assets/fonts/galaxus-font/font.ttf"

#define TEX_ONE             "assets/images/player.png"
#define TEX_TWO             "assets/images/enemy01.png"
#define TEX_THREE           "assets/images/enemy02.png"

typedef enum {
    FONT_NORMAL,
    FONT_SMALL,
    FONT_BIG,
} font_t;

#define FONT_MAX FONT_BIG

typedef enum {
    TEX_PLAYER,
    TEX_INVADER,
    TEX_ABDUCTOR,
} texture_t;

#define TEX_MAX TEX_ABDUCTOR

typedef struct {
    SDL_Renderer *handle;
    TTF_Font *fonts[FONT_MAX + 1];
    SDL_Texture *textures[TEX_MAX + 1];
} Renderer;

Renderer *renderer_init(SDL_Window *);
void renderer_destroy();

void renderer_clear();
void renderer_present();

TTF_Font *renderer_font_handle(font_t);
SDL_Texture *renderer_texture_handle(texture_t);

void draw_text(const char *text, vec2 pos, uint32_t color, TTF_Font *font, uint32_t depth);
void draw_rect(vec2 pos, vec2 dim, uint32_t color, uint32_t depth);
void draw_texture(SDL_Texture *, vec2 pos, vec2 dim, float angle, uint32_t depth);

uint32_t font_width(const char *text, font_t);
uint32_t font_height(font_t);

#endif

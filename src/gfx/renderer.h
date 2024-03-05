#ifndef _GFX_RENDERER_H_
#define _GFX_RENDERER_H_

#include "common/type.h"
#include "data/array_list.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#define RENDERER_FLAGS  (SDL_RENDERER_ACCELERATED)

#define FONT_ONE        "assets/fonts/press-start-2p-font/font.ttf"
#define FONT_TWO        "assets/fonts/galaxus-font/font.ttf"

#define TEX_ONE         "assets/images/player.png"
#define TEX_TWO         "assets/images/enemy01.png"
#define TEX_THREE       "assets/images/enemy02.png"

#define STR_LEN_MAX 32

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

typedef enum {
    DEPTH_BASE,
    DEPTH_PROJECTILE,
    DEPTH_CHARACTER,
    DEPTH_UI,
} depth_t;

#define DEPTH_MAX DEPTH_UI

typedef enum {
    C_FONT,
    C_RECT,
    C_TEXTURE,
} component_t;

typedef struct {
    component_t type;
    SDL_Rect rect;
    SDL_Color color;
    float angle;
    SDL_Texture *texture;
    char text[STR_LEN_MAX];
} RenderComponent;

typedef struct {
    SDL_Renderer *handle;
    TTF_Font *fonts[FONT_MAX + 1];
    SDL_Texture *textures[TEX_MAX + 1];
    ArrayList depth[DEPTH_MAX + 1];
} Renderer;

Renderer *renderer_init(SDL_Window *);
void renderer_destroy();

void renderer_clear();
void renderer_present();

TTF_Font *font_handle(font_t);
SDL_Texture *texture_handle(texture_t);

void draw_text(const char *text, vec2 pos, uint32_t color, font_t, depth_t);
void draw_rect(vec2 pos, vec2 dim, uint32_t color, depth_t);
void draw_texture(SDL_Texture *, vec2 pos, vec2 dim, float angle, depth_t);

uint32_t font_width(const char *text, font_t);
uint32_t font_height(font_t);

#endif

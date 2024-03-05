#ifndef _GFX_HUD_H_
#define _GFX_HUD_H_

#include "common/type.h"
#include "gfx/renderer.h"

#define HUD_MAX 16
#define STR_LEN_MAX 32

typedef enum {
    HUD_FONT,
    HUD_TEXTURE,
} hud_t;

typedef struct {
    hud_t type;
    uint32_t depth;
    vec2 pos;
    vec2 dim;
    uint32_t color;
    union {
        SDL_Texture *texture;
        TTF_Font *font;
    };
    char text[STR_LEN_MAX];
} HudElem;

typedef struct {
    size_t size;
    HudElem items[HUD_MAX];
} Hud;

void hud_add_text(const char *text, int x, int y, uint32_t color, font_t);
void hud_add_texture(int x, int y, int width, int height, texture_t);

void hud_init();
void hud_update();

#endif

#include "gfx/hud.h"

#include <assert.h>
#include <string.h>

static Hud hud;

void hud_add_text(const char *text, int x, int y, uint32_t color, font_t font) {
    assert(hud.size < HUD_MAX);
    HudElem *e = &hud.items[hud.size++];
    e->color = color;
    e->pos = (vec2) { .x = x, .y = y };
    e->font = renderer_font(font);

    size_t size = strnlen(text, STR_LEN_MAX);
    strncpy(e->text, text, size + 1);

    e->type = HUD_TEXT;
}

void hud_add_texture(int x, int y, int width, int height, texture_t texture) {
    assert(hud.size < HUD_MAX);
    HudElem *e = &hud.items[hud.size++];
    e->pos = (vec2) { .x = x, .y = y };
    e->pos = (vec2) { .w = width, .h = height };
    e->texture = renderer_texture(texture);
    e->type = HUD_TEXTURE;
}

void hud_init() {
    memset(&hud, 0, sizeof(Hud));
}

void hud_destroy() {}

void hud_update() {
    HudElem *e;
    for (size_t i = 0; i < hud.size; i++) {
        e = &hud.items[i];
        switch (e->type) {
            case HUD_TEXT:
                draw_text(e->text, e->pos.x, e->pos.y, e->color, e->font); 
                break;
            case HUD_TEXTURE:
                draw_texture(e->texture, e->pos.x, e->pos.y, e->dim.w, e->dim.height, 0);
                break;
        }
    }

    hud.size = 0;
}

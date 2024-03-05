#include "gfx/hud.h"
#include "gfx/renderer.h"

#include <assert.h>
#include <string.h>

static Hud hud;

void hud_add_text(const char *text, int x, int y, uint32_t color, font_t font) {
    assert(hud.size < HUD_MAX);
    HudElem *e = &hud.items[hud.size++];
    e->color = color;
    e->pos = (vec2) { .x = x, .y = y };
    e->font = renderer_font_handle(font);

    size_t size = strnlen(text, STR_LEN_MAX);
    strncpy(e->text, text, size + 1);

    e->type = HUD_FONT;
}

void hud_add_texture(int x, int y, int width, int height, texture_t texture) {
    assert(hud.size < HUD_MAX);
    HudElem *e = &hud.items[hud.size++];
    e->pos = (vec2) { .x = x, .y = y };
    e->pos = (vec2) { .w = width, .h = height };
    e->texture = renderer_texture_handle(texture);
    e->type = HUD_TEXTURE;
}

void hud_init() {
    memset(&hud, 0, sizeof(Hud));
}

void hud_update() {
    HudElem *e;
    for (size_t i = 0; i < hud.size; i++) {
        e = &hud.items[i];
        switch (e->type) {
            case HUD_FONT:
                draw_text(e->text, e->pos, e->color, e->font, LAYER_UI);
                break;
            case HUD_TEXTURE:
                draw_texture(e->texture, e->pos, e->dim, 0, LAYER_UI);
                break;
        }
    }

    hud.size = 0;
}

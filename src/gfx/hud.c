#include "game/fps.h"

#include "gfx/app.h"
#include "gfx/hud.h"
#include "gfx/renderer.h"
#include "gfx/window.h"

#include <assert.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>

static int _font_height;
static hud_s _hud;

void Hud_Init() {
    TTF_Font *font = LoadFont(FONT_PATH, FONT_SIZE);

    Renderer_SetFont(font);

    _font_height = GetFontHeight(font);

    _hud.count = 0;

    printf("Hud initialized.\n");
}

void Hud_Draw() {
    int pos = WINDOW_HEIGHT - _font_height - 5;
    DrawText("GALAGA", 5, pos, 0xFFFFFFFF);

    char *text;
    for (int i = 0; i < _hud.count; i++) {
        text = _hud.text[i];
        pos -= _font_height + 5;
        DrawText(text, 5, pos, 0xFFFFFFFF);
        free(text);
    }

    _hud.count = 0;
}

void Hud_DrawFPS(const fps_s *fps) {
    Hud_AddText("FPS: %.2f", FPS_Get(fps));
}

void Hud_AddText(const char *format, ...) {
    int *count = &_hud.count;
    assert(*count < MAX_NUM_STR);

    va_list argptr;
    char text[MAX_STR_LEN];
    va_start(argptr, format);
    vsnprintf(text, MAX_STR_LEN, format, argptr);
    va_end(argptr);

    size_t size = strlen(text) + 1;
    assert(size < MAX_STR_LEN);

    _hud.text[*count] = (char *)malloc(sizeof(char) * size);
    strncpy(_hud.text[*count], text, size);

    ++*count;
}

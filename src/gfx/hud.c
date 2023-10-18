#include "common/type.h"
#include "game/fps.h"

#include "gfx/app.h"
#include "gfx/hud.h"
#include "gfx/renderer.h"
#include "gfx/window.h"

#include <assert.h>
#include <string.h>

static Hud _hud;

void Hud_Init() {
    TTF_Font *font = LoadFont(FONT_PATH, FONT_SIZE);
    Renderer_SetFont(font);
    memcpy(&_hud.font, &font, sizeof(TTF_Font *));

    _hud.height = GetFontHeight(font);

    memset(&_hud.text, 0, sizeof(text_s));
    memset(&_hud.debug, 0, sizeof(text_s));

    printf("Hud initialized.\n");
}

void Hud_Destroy() {
    for (size_t i = 0; i < _hud.text.count; i++)
        free(_hud.text.text[i]);

    for (size_t i = 0; i < _hud.debug.count; i++)
        free(_hud.debug.text[i]);
}

void Hud_Draw(bool debug) {
    int pos = WINDOW_HEIGHT - _hud.height - 5;
    DrawText("GALAGA", 5, pos, COLOR_WHITE);

    char *text;
    for (size_t i = 0; i < _hud.text.count; i++) {
        text = _hud.text.text[i];
        pos -= _hud.height + 5;
        DrawText(text, 5, pos, COLOR_WHITE);
        free(text);
    }

    if (debug) {
        pos = WINDOW_HEIGHT - _hud.height - 5;
        for (size_t i = 0; i < _hud.debug.count; i++) {
            text = _hud.debug.text[i];
            pos -= _hud.height + 5;
            DrawText(text, WINDOW_WIDTH - GetFontWidth(_hud.font, text) - 5, pos, COLOR_WHITE);
            free(text);
        }
    } else {
        for (size_t i = 0; i < _hud.debug.count; i++) {
            text = _hud.debug.text[i];
            free(text);
        }
    }

    _hud.text.count = 0;
    _hud.debug.count = 0;
}

void Hud_DrawFPS(const fps_s *fps) {
    Hud_AddDebugText("FPS: %.2f", FPS_Get(fps));
}

void Hud_AddText(const char *format, ...) {
    int *count = &_hud.text.count;
    assert(*count < MAX_NUM_STR);

    va_list argptr;
    char text[MAX_STR_LEN];
    va_start(argptr, format);
    vsnprintf(text, MAX_STR_LEN, format, argptr);
    va_end(argptr);

    size_t size = strlen(text) + 1;
    assert(size < MAX_STR_LEN);

    _hud.text.text[*count] = (char *)malloc(sizeof(char) * size);
    strncpy(_hud.text.text[*count], text, size);

    ++*count;
}

void Hud_AddDebugText(const char *format, ...) {
    int *count = &_hud.debug.count;
    assert(*count < MAX_NUM_STR);

    va_list argptr;
    char text[MAX_STR_LEN];
    va_start(argptr, format);
    vsnprintf(text, MAX_STR_LEN, format, argptr);
    va_end(argptr);

    size_t size = strlen(text) + 1;
    assert(size < MAX_STR_LEN);

    _hud.debug.text[*count] = (char *)malloc(sizeof(char) * size);
    strncpy(_hud.debug.text[*count], text, size);

    ++*count;
}

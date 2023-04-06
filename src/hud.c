#include "inc/app.h"
#include "inc/hud.h"
#include "inc/render.h"
#include "inc/window.h"

#include <SDL2/SDL_ttf.h>

static int _font_height;

void Hud_Init() {
    TTF_Font *font = LoadFont(FONT_PATH, 12);

    Renderer_SetFont(font);

    _font_height = GetFontHeight(font);

    printf("Hud initialized.\n");
}

void Hud_Draw() {
    DrawText("GALAGA", 5, WINDOW_HEIGHT - _font_height - 5, 0xFFFFFFFF);
}

#ifndef _HUD_H_
#define _HUD_H_

#include "game/fps.h"

#include <stdbool.h>

#include <SDL2/SDL_ttf.h>

#define FONT_SIZE           12
#define FONT_PATH           "assets/fonts/press-start-2p-font/font.ttf"

#define MAX_NUM_STR         12
#define MAX_STR_LEN         32

typedef struct {
    int count;
    char *text[MAX_NUM_STR];
} text_s;

typedef struct {
    TTF_Font *font;
    uint32_t height;
    text_s text, debug;
} Hud;

void Hud_Init();
void Hud_Destroy();

void Hud_Draw(bool debug);
void Hud_DrawFPS(const fps_s *fps);
void Hud_AddText(const char *format, ...);
void Hud_AddDebugText(const char *format, ...);

#endif

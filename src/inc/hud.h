#ifndef _HUD_H_
#define _HUD_H_

#define FONT_SIZE           12
#define FONT_PATH           "assets/fonts/press-start-2p-font/font.ttf"

#define MAX_NUM_STR         12
#define MAX_STR_LEN         1024

typedef struct {
    int count;
    char *text[MAX_NUM_STR];
} hud_s;

void Hud_Init();

void Hud_Draw();

void Hud_DrawFPS();

void Hud_AddText(const char *format, ...);

#endif

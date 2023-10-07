#ifndef _APP_H_
#define _APP_H_

#include "game/game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define APP_TITLE       GAME_TITLE

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} App;

void initSDL();

SDL_Texture *LoadTexture(const char *filename);

TTF_Font *LoadFont(const char *filename, int size);

int GetFontWidth(TTF_Font *font);

int GetFontHeight(TTF_Font *font);

#endif

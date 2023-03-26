#ifndef _APP_H_
#define _APP_H_

#include <SDL2/SDL.h>

#define APP_TITLE			"GALAGA"

typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
} App;

void initSDL();

SDL_Texture *LoadTexture(const char *filename);

#endif
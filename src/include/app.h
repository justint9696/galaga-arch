#ifndef _APP_H_
#define _APP_H_

#include <SDL2/SDL.h>

#define APP_TITLE			"GALAGA"
#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		480
#define WINDOW_FLAGS		0
#define RENDERER_FLAGS		SDL_RENDERER_ACCELERATED


typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
} App;

void initSDL();

void drawRect(float x, float y, int width, int height, uint32_t color);

#endif
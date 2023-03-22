#include <assert.h>

#include "include/app.h"

static App app;

void initSDL() {
	assert(!SDL_Init(SDL_INIT_VIDEO));

	app.window = SDL_CreateWindow(
			APP_TITLE,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			WINDOW_FLAGS);

	assert(app.window);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, RENDERER_FLAGS);

	assert(app.renderer);
}

void drawRect(float x, float y, int width, int height, uint32_t color) {
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
	SDL_RenderClear(app.renderer);
	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
	for (int i = x; i < x + width; i++) {
		for (int j = y; j < y + height; j++) {
			SDL_RenderDrawPoint(app.renderer, i, j);
		}
	}
	SDL_RenderPresent(app.renderer);
}
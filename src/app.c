#include "include/app.h"
#include "include/util.h"

#include <assert.h>
#include <SDL2/SDL_image.h>

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
	// make origin at bottom of screen
	y += WINDOW_HEIGHT;

	const int x0 = clamp(0, x, WINDOW_WIDTH - width),
		y0 = clamp(0, y, WINDOW_HEIGHT - height);

	const int xf = clamp(0, x0 + width, WINDOW_WIDTH),
		yf = clamp(0, y0 + height, WINDOW_HEIGHT);

	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
	SDL_RenderClear(app.renderer);
	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
	for (int i = x0; i < xf; i++) {
		for (int j = y0; j < yf; j++) {
			SDL_RenderDrawPoint(app.renderer, i, j);
		}
	}
	SDL_RenderPresent(app.renderer);
}

SDL_Texture *LoadTexture(const char *filename) {
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(app.renderer, filename);

	return texture;
}
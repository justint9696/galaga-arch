#include "include/app.h"
#include "include/render.h"
#include "include/util.h"
#include "include/window.h"

#include <assert.h>
#include <SDL2/SDL_image.h>

static App _app;

void initSDL() {
	assert(!SDL_Init(SDL_INIT_VIDEO));

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	_app.window = SDL_CreateWindow(
			APP_TITLE,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			WINDOW_FLAGS);

	assert(_app.window);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	_app.renderer = SDL_CreateRenderer(_app.window, -1, RENDERER_FLAGS);

	assert(_app.renderer);

	Renderer_Init(_app.renderer);
}

SDL_Texture *LoadTexture(const char *filename) {
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(_app.renderer, filename);

	return texture;
}
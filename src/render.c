#include "inc/render.h"
#include "inc/util.h"
#include "inc/window.h"

#include <assert.h>
#include <SDL2/SDL_render.h>

static SDL_Renderer *_renderer;

void Renderer_Init(SDL_Renderer *renderer) {
    _renderer = renderer;

    assert(_renderer);
}

void Renderer_Prepare() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Renderer_Update() {
    SDL_RenderPresent(_renderer);
}

void DrawRect(float x, float y, int width, int height, uint64_t color) {
    // make origin at bottom of screen
	y = WINDOW_HEIGHT - y - height;

	const int x0 = x, y0 = y;

	const int 
		xf = x0 + width,
		yf = y0 + height;

    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	for (int i = x0; i < xf; i++) {
		for (int j = y0; j < yf; j++) {
			SDL_RenderDrawPoint(_renderer, i, j);
		}
	}
}

void DrawTexture(SDL_Texture *texture, float x, float y, int width, int height) {
    // make origin at bottom of screen
	y = WINDOW_HEIGHT - y - height;

    const int 
		x0 = clamp(0, x, WINDOW_WIDTH - width),
        y0 = clamp(0, y, WINDOW_HEIGHT - height);

    SDL_Rect dst;
    dst.x = x0;
    dst.y = y0;
    dst.w = width;
    dst.h = height;

    // SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(_renderer, texture, NULL, &dst);
}

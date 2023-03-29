#include "inc/render.h"
#include "inc/type.h"
#include "inc/util.h"
#include "inc/window.h"

#include <assert.h>
#include <math.h>
#include <SDL2/SDL_render.h>

static Color _color;
static SDL_Renderer *_renderer;

static void RGBA(uint32_t color, Color *dst) {
    dst->r = (color >> 24) & 0xFF;
    dst->g = (color >> 16) & 0xFF;
    dst->b = (color >> 8) & 0xFF;
    dst->a = (color) & 0xFF;
}

void Renderer_Init(SDL_Renderer *renderer) {
    _renderer = renderer;

    assert(_renderer);

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
}

void Renderer_Prepare() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Renderer_Update() {
    SDL_RenderPresent(_renderer);
}

void DrawRect(int x, int y, int width, int height, uint32_t color) {
    RGBA(color, &_color);

    // make origin at bottom of the screen
    y = WINDOW_HEIGHT - y - height;

    const int x0 = x, y0 = y;
    const int xf = x0 + width, yf = y0 + width;

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
    SDL_RenderFillRect(_renderer, &rect);
}

void DrawTexture(SDL_Texture *texture, int x, int y, int width, int height) {
    // make origin at bottom of screen
	y = WINDOW_HEIGHT - y - height;

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = width;
    dst.h = height;

    // SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(_renderer, texture, NULL, &dst);
}

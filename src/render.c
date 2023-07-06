#include "inc/render.h"
#include "inc/type.h"
#include "inc/util.h"
#include "inc/window.h"

#include <assert.h>
#include <math.h>
#include <SDL2/SDL_render.h>

static color_s _color;
static SDL_Renderer *_renderer;
static TTF_Font *_font;

void Renderer_Init(SDL_Renderer *renderer) {
    _renderer = renderer;

    assert(_renderer);

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
}

void Renderer_SetFont(TTF_Font *font) {
    _font = font;
}

void Renderer_Prepare() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Renderer_Update() {
    SDL_RenderPresent(_renderer);
}

void DrawRect(int x, int y, int width, int height, uint32_t color) {
    _color = rgba(color);

    // make origin at bottom of the screen
    y = WINDOW_HEIGHT - y - height;

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
    SDL_RenderFillRect(_renderer, &rect);
}

void DrawTexture(SDL_Texture *texture, int x, int y, int width, int height, float angle) {
    // make origin at bottom of screen
    y = WINDOW_HEIGHT - y - height;

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = width;
    dst.h = height;

    // SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    // SDL_RenderCopy(_renderer, texture, NULL, &dst);
    SDL_RenderCopyEx(_renderer, texture, NULL, &dst, angle, NULL, false);
}

void DrawText(const char *text, int x, int y, uint32_t color) {
    SDL_Color textColor;
    memcpy(&textColor, &rgba(color), sizeof(uint8_t) * 4);
    SDL_Surface *surface = TTF_RenderText_Solid(_font, text, textColor);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);

    const int width = surface->w, height = surface->h;
    SDL_FreeSurface(surface);

    DrawTexture(texture, x, y, width, height, 0.f);
}

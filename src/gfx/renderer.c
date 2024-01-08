#include "common/util.h"
#include "gfx/renderer.h"
#include "gfx/window.h"

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static Renderer renderer;

Renderer *renderer_init(SDL_Window *window) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    renderer.handle = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
    ASSERT(renderer.handle, "Failed to create renderer: %s\n", SDL_GetError());
    SDL_SetRenderDrawBlendMode(renderer.handle, SDL_BLENDMODE_BLEND);

    return &renderer;
}

void renderer_destroy() {
    SDL_DestroyRenderer(renderer.handle);
}

void renderer_prepare() {
    SDL_SetRenderDrawColor(renderer.handle, 0, 0, 0, 255);
    SDL_RenderClear(renderer.handle);
}

void renderer_present() {
    SDL_RenderPresent(renderer.handle);
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    color_s c = RGBA(color);

    // make origin at bottom of the screen
    y = SCREEN_HEIGHT - y - h;

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(renderer.handle, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer.handle, &rect);
}

void draw_texture(SDL_Texture *texture, int x, int y, int w, int h, float angle) {
    // make origin at bottom of screen
    y = SCREEN_HEIGHT - y - h;

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;

    // SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    // SDL_RenderCopy(_renderer, texture, NULL, &dst);
    SDL_RenderCopyEx(renderer.handle, texture, NULL, &dst, angle, NULL, false);
}

SDL_Texture *load_texture(const char *path) {
    // SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", path);

    SDL_Texture *texture = IMG_LoadTexture(renderer.handle, path);
    ASSERT(texture, "Could not load texture: %s\n", path);

    return texture;
}

TTF_Font *load_font(const char *path, uint32_t sizefont) {
    return NULL;
}


#include "common/util.h"
#include "gfx/renderer.h"
#include "gfx/window.h"

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static Renderer renderer;

static SDL_Texture *load_texture(const char *path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer.handle, path);
    ASSERT(texture, "Could not load texture: %s\n", path);

    return texture;
}

static TTF_Font *load_font(const char *path, uint32_t size) {
    TTF_Font *font = TTF_OpenFont(path, size);
    ASSERT(font, "Could not load font: %s\n", path);

    return font;
}

static void prepare_fonts() {
    renderer.fonts[0] = load_font(FONT_ONE, 14);
    renderer.fonts[1] = load_font(FONT_ONE, 12);
    renderer.fonts[2] = load_font(FONT_ONE, 16);
}

static void prepare_textures() {
    renderer.textures[0] = load_texture(TEX_ONE);
    renderer.textures[1] = load_texture(TEX_TWO);
    renderer.textures[2] = load_texture(TEX_THREE);
}

Renderer *renderer_init(SDL_Window *window) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    renderer.handle = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
    ASSERT(renderer.handle, "Failed to create renderer: %s\n", SDL_GetError());
    SDL_SetRenderDrawBlendMode(renderer.handle, SDL_BLENDMODE_BLEND);

    TTF_Init();
    prepare_fonts();

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    prepare_textures();

    return &renderer;
}

void renderer_destroy() {
    SDL_DestroyRenderer(renderer.handle);
}

void renderer_clear() {
    SDL_SetRenderDrawColor(renderer.handle, 0, 0, 0, 255);
    SDL_RenderClear(renderer.handle);
}

void renderer_present() {
    SDL_RenderPresent(renderer.handle);
}

TTF_Font *renderer_font_handle(font_t font) {
    return renderer.fonts[font];
}

SDL_Texture *renderer_texture_handle(texture_t texture) {
    return renderer.textures[texture];
}

void draw_text(const char *text, vec2 pos, uint32_t color, TTF_Font *font, uint32_t depth) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, RGBA(color));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer.handle, surface);

    const vec2 dim = VEC2(surface->w, surface->h);
    SDL_FreeSurface(surface);

    draw_texture(texture, pos, dim, 0.f, depth); 
}

void draw_rect(vec2 pos, vec2 dim, uint32_t color, uint32_t depth) {
    SDL_Color c = RGBA(color);

    // make origin at bottom of the screen
    pos.y = (SCREEN_HEIGHT - pos.y - dim.h);

    SDL_Rect rect = (SDL_Rect) {
        .x = pos.x, .y = pos.y,
        .w = dim.w, .h = dim.h,
    };

    // SDL_SetRenderDrawColor(renderer.handle, c.r, c.g, c.b, c.a);
    // SDL_RenderFillRect(renderer.handle, &rect);
}

void draw_texture(SDL_Texture *texture, vec2 pos, vec2 dim, float angle, uint32_t depth) {
    // make origin at bottom of screen
    pos.y = (SCREEN_HEIGHT - pos.y - dim.h);

    SDL_Rect rect = (SDL_Rect) {
        .x = pos.x, .y = pos.y,
        .w = dim.w, .h = dim.h,
    };

    // SDL_RenderCopyEx(renderer.handle, texture, NULL, &rect, angle, NULL, false);
}

uint32_t font_width(const char *text, font_t font) {
    int width, height;
    TTF_Font *handle = renderer_font_handle(font);
    TTF_SizeText(handle, text, &width, &height);
    return width;
}

uint32_t font_height(font_t font) {
    int width, height;
    TTF_Font *handle = renderer_font_handle(font);
    TTF_SizeText(handle, "", &width, &height);
    return height;
}

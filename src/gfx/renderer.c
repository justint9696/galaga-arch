#include "gfx/renderer.h"
#include "gfx/window.h"

#include "common/util.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

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

    // allocate appropriate memory for each renderer layer
    array_list_init(&renderer.depth[DEPTH_BASE], RenderComponent, 1024);
    array_list_init(&renderer.depth[DEPTH_PROJECTILE], RenderComponent, 64);
    array_list_init(&renderer.depth[DEPTH_CHARACTER], RenderComponent, 64);
    array_list_init(&renderer.depth[DEPTH_UI], RenderComponent, 64);

    return &renderer;
}

void renderer_destroy() {
    // free allocated memory for each renderer layer
    for (size_t i = 0; i < DEPTH_MAX + 1; i++) {
        array_list_destroy(&renderer.depth[i]);
    }

    SDL_DestroyRenderer(renderer.handle);
}

static void render_rect(RenderComponent *comp) {
    SDL_Color c = comp->color;
    SDL_SetRenderDrawColor(renderer.handle, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer.handle, &comp->rect);
}

static void render_texture(RenderComponent *comp) {
    SDL_RenderCopyEx(renderer.handle, comp->texture, NULL, &comp->rect, comp->angle, NULL, false);
}

static void renderer_prepare() {
    ArrayList *list;
    RenderComponent *comp;
    for (size_t i = 0; i < DEPTH_MAX + 1; i++) {
        list = &renderer.depth[i];
        for (size_t j = 0; j < list->size; j++) {
            comp = (RenderComponent *)array_list_get(list, j);
            switch (comp->type) {
                case C_FONT:
                case C_TEXTURE:
                    render_texture(comp);
                    break;
                case C_RECT:
                    render_rect(comp);
                    break;
            }

            array_list_remove(list, j);
        }
    }

    // clear components from each layer of renderer
    // for (size_t i = 0; i < DEPTH_MAX + 1; i++)
    // 	array_list_clear(&renderer.depth[i]);
}

void renderer_clear() {
    SDL_SetRenderDrawColor(renderer.handle, 0, 0, 0, 255);
    SDL_RenderClear(renderer.handle);
}

void renderer_present() {
    renderer_prepare();
    SDL_RenderPresent(renderer.handle);
}

inline TTF_Font *font_handle(font_t font) {
    return renderer.fonts[font];
}

inline SDL_Texture *texture_handle(texture_t texture) {
    return renderer.textures[texture];
}

void draw_text(const char *text, vec2 pos, uint32_t color, font_t font, depth_t depth) {
    SDL_Surface *surface = TTF_RenderText_Solid(font_handle(font), text, RGBA(color));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer.handle, surface);

    const vec2 dim = VEC2(surface->w, surface->h);
    SDL_FreeSurface(surface);

    // make the origin at bottom of the screen
    pos.y = (SCREEN_HEIGHT - pos.y - dim.h);

    // construct the render component
    RenderComponent comp = {
		.angle = 0.f,
		.type = C_TEXTURE,
		.rect = (SDL_Rect) {
				.x = pos.x, .y = pos.y,
				.w = dim.w, .h = dim.h,
			},
		.texture = texture,
	};

    size_t size = strnlen(text, STR_LEN_MAX);
    strncpy(comp.text, text, size + 1);

    // add the component to the corresponding layer
    array_list_append(&renderer.depth[depth], &comp);
}

void draw_rect(vec2 pos, vec2 dim, uint32_t color, depth_t depth) {
    // make the origin at bottom of the screen
    pos.y = (SCREEN_HEIGHT - pos.y - dim.h);

    // construct the render component
    RenderComponent comp = {
		.angle = 0.f,
		.type = C_RECT,
		.color = RGBA(color),
		.rect = (SDL_Rect) {
				.x = pos.x, .y = pos.y,
				.w = dim.w, .h = dim.h,
			},
	};

    // add the component to the corresponding layer
    array_list_append(&renderer.depth[depth], &comp);

    // SDL_SetRenderDrawColor(renderer.handle, c.r, c.g, c.b, c.a);
    // SDL_RenderFillRect(renderer.handle, &rect);
}

void draw_texture(SDL_Texture *texture, vec2 pos, vec2 dim, float angle, depth_t depth) {
    // make the origin at bottom of the screen
    pos.y = (SCREEN_HEIGHT - pos.y - dim.h);

    // construct the render component
    RenderComponent comp = {
		.angle = angle,
		.type = C_TEXTURE,
		.rect = (SDL_Rect) {
				.x = pos.x, .y = pos.y,
				.w = dim.w, .h = dim.h,
			},
		.texture = texture,
	};

    // add the component to the corresponding layer
    array_list_append(&renderer.depth[depth], &comp);

    // SDL_RenderCopyEx(renderer.handle, texture, NULL, &rect, angle, NULL, false);
}

uint32_t font_width(const char *text, font_t font) {
    int width, height;
    TTF_Font *handle = font_handle(font);
    TTF_SizeText(handle, text, &width, &height);
    return width;
}

uint32_t font_height(font_t font) {
    int width, height;
    TTF_Font *handle = font_handle(font);
    TTF_SizeText(handle, "", &width, &height);
    return height;
}

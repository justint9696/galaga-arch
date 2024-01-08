#include "common/util.h"
#include "gfx/window.h"

static Window window;

void window_init(const char *title) {
    ASSERT(!SDL_Init(SDL_INIT_VIDEO), "Could not initialize SDL: %s\n", SDL_GetError());

    window.handle = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAGS);
    ASSERT(window.handle, "Failed to open window: %s\n", SDL_GetError());

    window.renderer = renderer_init(window.handle);
}

void window_destroy() {
    renderer_destroy();

    SDL_DestroyWindow(window.handle);
    SDL_Quit();
}

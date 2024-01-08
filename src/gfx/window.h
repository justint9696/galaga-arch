#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "gfx/renderer.h"

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

// amount entities are scaled
#define SCREEN_SCALAR .85

// buffer region outside of window borders
#define SCREEN_BUFFER 250

#define WINDOW_FLAGS 0

typedef struct {
    SDL_Window *handle;
    Renderer *renderer;
} Window;

void window_init(const char *title);
void window_destroy();

#endif

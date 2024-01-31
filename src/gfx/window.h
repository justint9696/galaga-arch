#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "gfx/renderer.h"

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 576
#define SCREEN_HEIGHT 720

// amount entity sprites are scaled by
#define SCREEN_SCALAR .75

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

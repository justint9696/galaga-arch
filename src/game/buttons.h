#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_scancode.h>

#include <stdbool.h>
#include <stdint.h>

#define BUTTON_COOLDOWN     250

typedef struct {
    bool is_pressed;
    uint32_t tick;
} button_t;

typedef struct {
    button_t keys[SDL_NUM_SCANCODES];
} Buttons;

void buttons_init();
void buttons_update(SDL_Event event);
bool button_pressed(SDL_Scancode button, bool cooldown);

#endif

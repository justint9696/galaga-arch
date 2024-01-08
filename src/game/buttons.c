#include "game/buttons.h"
#include "common/time.h"

static Buttons buttons;

void buttons_init() {
    memset(&buttons, 0, sizeof(Buttons));
}

void buttons_update(SDL_Event event) {
    button_t *key;
    switch (event.type) {
        case SDL_KEYDOWN:
            key = &buttons.keys[event.key.keysym.scancode];
            key->is_pressed = true;
            break;
        case SDL_KEYUP:
            key = &buttons.keys[event.key.keysym.scancode];
            key->is_pressed = false;
            break;
        default:
            break;
    }
}

bool button_pressed(SDL_Scancode button, bool cooldown) {
    uint32_t now = NOW();
    button_t *key = &buttons.keys[button];
    if (!key->is_pressed || (cooldown && (now - key->tick < BUTTON_COOLDOWN)))
        return false;
    key->tick = now;
    return true;
}

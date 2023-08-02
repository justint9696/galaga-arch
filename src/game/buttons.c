#include "entity/player.h"
#include "buttons.h"

int _buttons = 0;

int Buttons_Get() {
    return _buttons;
}

void Buttons_Update(SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_UP:
                    _buttons |= BUTTON_UP;
                    break;
                case SDL_SCANCODE_DOWN:
                    _buttons |= BUTTON_DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    _buttons |= BUTTON_LEFT;
                    break;
                case SDL_SCANCODE_RIGHT:
                    _buttons |= BUTTON_RIGHT;
                    break;
                case SDL_SCANCODE_SPACE:
                    _buttons |= BUTTON_SPACE;
                    break;
                default:
                    break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_UP:
                    _buttons &= ~BUTTON_UP;
                    break;
                case SDL_SCANCODE_DOWN:
                    _buttons &= ~BUTTON_DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    _buttons &= ~BUTTON_LEFT;
                    break;
                case SDL_SCANCODE_RIGHT:
                    _buttons &= ~BUTTON_RIGHT;
                    break;
                case SDL_SCANCODE_SPACE:
                    _buttons &= ~BUTTON_SPACE;
                    break;
                default:
                    break;
            }
    }
}

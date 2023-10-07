#include "entity/player.h"
#include "game/buttons.h"

void Buttons_Update(uint32_t *buttons, SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_UP:
                    *buttons |= BUTTON_UP;
                    break;
                case SDL_SCANCODE_DOWN:
                    *buttons |= BUTTON_DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    *buttons |= BUTTON_LEFT;
                    break;
                case SDL_SCANCODE_RIGHT:
                    *buttons |= BUTTON_RIGHT;
                    break;
                case SDL_SCANCODE_SPACE:
                    *buttons |= BUTTON_SPACE;
                    break;
                default:
                    break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_UP:
                    *buttons &= ~BUTTON_UP;
                    break;
                case SDL_SCANCODE_DOWN:
                    *buttons &= ~BUTTON_DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    *buttons &= ~BUTTON_LEFT;
                    break;
                case SDL_SCANCODE_RIGHT:
                    *buttons &= ~BUTTON_RIGHT;
                    break;
                case SDL_SCANCODE_SPACE:
                    *buttons &= ~BUTTON_SPACE;
                    break;
                default:
                    break;
            }
    }
}

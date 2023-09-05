#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <SDL2/SDL_events.h>
#include <stdint.h>

#define BUTTON_UP           (1 << 0)
#define BUTTON_DOWN         (1 << 1)
#define BUTTON_LEFT         (1 << 2)
#define BUTTON_RIGHT        (1 << 3)
#define BUTTON_SPACE        (1 << 4)

void Buttons_Update(uint32_t *buttons, SDL_Event event);

#endif

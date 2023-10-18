#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <SDL2/SDL_events.h>
#include <stdint.h>

#define BUTTON_DELAY        250

#define BUTTON_UP           (1 << 0)
#define BUTTON_DOWN         (1 << 1)
#define BUTTON_LEFT         (1 << 2)
#define BUTTON_RIGHT        (1 << 3)
#define BUTTON_SPACE        (1 << 4)
#define BUTTON_F12          (1 << 5)

typedef struct {
    uint32_t previous, current;
} Buttons;

void Buttons_Update(Buttons *self, SDL_Event event);

#endif

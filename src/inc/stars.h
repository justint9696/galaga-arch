#ifndef _STARS_H_
#define _STARS_H_

#include "type.h"
#include "window.h"

#include <stdint.h>

#define STAR_WIDTH          5.f
#define STAR_HEIGHT         5.f
#define STAR_MAX_COUNT      50

#define COLOR_STAR          0xFFFFFFFF

typedef struct {
    vec2 pos, vel;
} Star;

void Stars_Init();

void Stars_Update(uint64_t tick, uint64_t deltaTime);

#endif

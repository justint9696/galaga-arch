#ifndef _STARS_H_
#define _STARS_H_

#include "common/type.h"
#include "window.h"

#include <stdint.h>

#define STAR_WIDTH          1.f
#define STAR_HEIGHT         STAR_WIDTH
#define STAR_MAX_COUNT      150

#define COLOR_STAR          0xFFFFFF00

typedef enum {
    STATE_NORMAL,
    STATE_FADEIN,
    STATE_FADEOUT,
} star_state_t;

typedef struct {
    star_state_t state;
    uint32_t color;
    vec2 pos, vel;
} Star;

void Stars_Init();

void Stars_Update(uint64_t deltaTime);

#endif

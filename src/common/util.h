#ifndef _UTIL_H_
#define _UTIL_H_

#include "common/type.h"

#include <SDL2/SDL_pixels.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ASSERT(_e, ...) { if (!(_e)) { fprintf(stderr, __VA_ARGS__); exit(1); }}
#define LOG(...) { fprintf(stdout, "[DEBUG] "); fprintf(stdout, __VA_ARGS__); }

#define SRAND() srand(time(NULL));
#define RAND(n) ({ rand() % n; })

#define clamp(mn, x, mx) ((mn > x) ? mn : (mx < x) ? mx : x)

#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)

#define RGBA(c) ((SDL_Color) {\
        (c >> 24) & 0xff,\
        (c >> 16) & 0xff,\
        (c >>  8) & 0xff,\
        (c >>  0) & 0xff\
    })

#define distance(a, b) ((sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2))))

#define closer(a, b, c) (distance(a, c) < distance(b, c))

#endif

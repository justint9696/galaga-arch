#ifndef _UTIL_H_
#define _UTIL_H_

#include "type.h"

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define clamp(mn, x, mx) ((mn > x) ? mn : (mx < x) ? mx : x)

#define RGBA(c) ((color_s) {\
        (c >> 24) & 0xff,\
        (c >> 16) & 0xff,\
        (c >>  8) & 0xff,\
        (c >>  0) & 0xff\
    })

#define distance(a, b) ({ \
        __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        (sqrtf(powf(_a.x - _b.x, 2) + powf(_a.y - _b.y, 2))); \
    })

#define closer(a, b, c) (distance(a, c) < distance(b, c))

#endif

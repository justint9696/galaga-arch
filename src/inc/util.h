#ifndef _UTIL_H_
#define _UTIL_H_

#include "type.h"

#include <stdbool.h>
#include <stdint.h>

// #define DEBUG
#define clamp(a, b, c) ((a > b) ? a : (c < b) ? c : b)

#define rgba(c) ((color_s) {\
        (c >> 24) & 0xff,\
        (c >> 16) & 0xff,\
        (c >>  8) & 0xff,\
        (c >>  0) & 0xff\
    })

float Distance(const vec2 pt1, const vec2 pt2);

#endif

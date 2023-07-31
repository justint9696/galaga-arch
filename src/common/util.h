#ifndef _UTIL_H_
#define _UTIL_H_

#include "type.h"

#include <stdbool.h>
#include <stdint.h>

// #define DEBUG
#define CLAMP(mn, x, mx) ((mn > x) ? mn : (mx < x) ? mx : x)

#define RGBA(c) ((color_s) {\
        (c >> 24) & 0xff,\
        (c >> 16) & 0xff,\
        (c >>  8) & 0xff,\
        (c >>  0) & 0xff\
    })

#define DISTANCE(pt0, pt1) {\
        p0 = (vec2) pt0;\
        p1 = (vec2) pt1;\
        sqrtf(powf(p0.x - p1.x) + powf(p0.y - p1.y))\
    }

#define CLOSER(pt0, pt1, dst) {\
        p0 = (vec2) pt0;\
        p1 = (vec2) pt1;\
        dest = (vec2) dst; \
        DISTANCE(p0, dst) < DISTANCE(p1, dst)\
    }

float Distance(const vec2 pt1, const vec2 pt2);
float Closer(const vec2 pt1, const vec2 pt2, const vec2 dst);

#endif

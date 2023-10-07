#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdint.h>

#define COLOR_WHITE     0xffffffff
#define COLOR_BLUE      0x0000ffff
#define COLOR_RED       0xff0000ff
#define COLOR_GREEN     0x00ff00ff
#define COLOR_YELLOW    0xffff00ff

typedef struct {
    union {
        float x, w, width;
    };
    union {
        float y, h, height;
    };
} vec2, v2;

typedef struct {
    int x, y;
} ivec2, iv2;

typedef struct {
    uint8_t r, g, b, a;
} color_s;

#endif

#ifndef _TYPE_H_
#define _TYPE_H_

#include <stdint.h>

#define COLOR_WHITE     0xffffffff
#define COLOR_BLUE      0x0000ffff
#define COLOR_RED       0xff0000ff
#define COLOR_GREEN     0x00ff00ff
#define COLOR_YELLOW    0xffff00ff

typedef union {
    struct {
        float x, y;
    };
    struct {
        float w, h;
    };
    struct {
        float width, height;
    };
} vec2, v2;

typedef struct {
    uint8_t r, g, b, a;
} color_s;

#endif

#ifndef _COMMON_TYPE_H_
#define _COMMON_TYPE_H_

#include <stdint.h>

#define COLOR_WHITE     0xffffffff
#define COLOR_BLUE      0x0000ffff
#define COLOR_RED       0xff0000ff
#define COLOR_GREEN     0x00ff00ff
#define COLOR_YELLOW    0xffff00ff
#define COLOR_CYAN      0x00ffffff

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#ifdef ENVIRONMENT64
typedef uint64_t data_t;
#else
typedef uint32_t data_t;
#endif

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

#define VEC2(_x, _y) ((vec2) { .x = _x, .y = _y })

#endif

#include "inc/util.h"

float clamp(int min, float current, int max) {
    return (current < min ? min : current > max ? max : current);
}

void RGBA(uint32_t src, Color *dst) {
    dst->r = ((src >> 24) & 0xFF);
    dst->g = ((src >> 16) & 0xFF);
    dst->b = ((src >> 8) & 0xFF);
    dst->a = ((src) & 0xFF);
}

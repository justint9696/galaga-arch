#include "inc/util.h"

#include <math.h>

float Distance(const vec2 pt1, const vec2 pt2) {
    return sqrtf(powf(pt1.x - pt2.x, 2) + powf(pt1.y - pt2.y, 2));
}


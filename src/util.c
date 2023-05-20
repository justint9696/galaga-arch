#include "inc/util.h"

#include <math.h>

float Distance(const vec2 pt1, const vec2 pt2) {
    return fabs(pt1.x - pt2.x) + fabs(pt1.y - pt2.y);
}


#ifndef _UTIL_H_
#define _UTIL_H_

#include "type.h"

#include <stdbool.h>
#include <stdint.h>

// #define DEBUG

float clamp(int min, float current, int max);

void RGBA(uint32_t src, Color *dst);

float Distance(const vec2 pt1, const vec2 pt2);

#endif

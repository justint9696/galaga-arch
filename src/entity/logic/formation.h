#ifndef _FORMATION_H_
#define _FORMATION_H_

#include "common/type.h"

typedef struct {
    uint32_t id;
    vec2 center, vel;
} Formation;

void Formation_Init(Formation *self);
vec2 Formation_GetPosition(Formation *self, uint32_t id);
void Formation_Update(Formation *self, uint64_t deltaTime);

#endif

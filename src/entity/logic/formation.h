#ifndef _FORMATION_H_
#define _FORMATION_H_

#include "common/type.h"
#include "entity/entity.h"

typedef struct {
    uint32_t id;
    Entity entity;
} Formation;

void Formation_Init(Formation *self);
vec2 Formation_GetPosition(Formation *self, uint32_t id);
vec2 Formation_ApproxPosition(Formation *self, uint32_t id, float time);
void Formation_Update(Formation *self);

#endif

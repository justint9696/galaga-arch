#ifndef _FORMATION_H_
#define _FORMATION_H_

#include "common/type.h"
#include "data/queue.h"
#include "entity/entity.h"
#include "entity/logic/path.h"

typedef struct {
    uint32_t id;
    Queue path;
    Entity entity;
} Formation;

void Formation_Init(Formation *self);
vec2 Formation_GetPosition(Formation *self, uint32_t id);
void Formation_Update(Formation *self);

#endif

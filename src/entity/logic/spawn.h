#ifndef _SPAWN_H_
#define _SPAWN_H_

#include "common/type.h"
#include "entity/logic/enemy.h"

vec2 Spawn_GetOrigin(ewave_t wave);
vec2 Spawn_GetDestination(uint64_t *data);

#endif

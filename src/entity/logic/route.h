#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "entity/entity.h"
#include "entity/logic/path.h"

void route_idle(Entity *, float speed);
void route_spawn(Entity *, float speed);
void route_swoop(Entity *, float speed);
void route_merge(Entity *, float speed);

void route_start(Queue *q, vec2 org, vec2 dst, float speed, ptype_t type);
void route_append(Queue *q, vec2 dst, float speed, ptype_t);

#endif

#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "entity/entity.h"

void route_idle(Entity *, float speed);
void route_spawn(Entity *, float speed);
void route_swoop(Entity *, float speed);

#endif

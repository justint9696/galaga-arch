#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "entity/entity.h"
#include "entity/logic/path.h"

struct World;

typedef enum {
    ROUTE_IDLE,
    ROUTE_SPAWN,
    ROUTE_SWOOP,
    ROUTE_RETURN,
    ROUTE_CHARGE,
    ROUTE_MERGE,
} route_t;

void route_create(Entity *, struct World *, route_t, float speed);
void route_start(Queue *, vec2 org, vec2 dst, float speed, ptype_t);
void route_append(Queue *, vec2 dst, float speed, ptype_t);

#endif

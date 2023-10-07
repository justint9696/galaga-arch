#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "common/type.h"
#include "data/queue.h"

void Route_Spawn(Queue *q, vec2 org);
void Route_Idle(Queue *q, vec2 org);
void Route_Swoop(Queue *q, vec2 org);

#endif

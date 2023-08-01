#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "../data/queue.h"
#include "path.h"

void Route_Spawn(Queue *, vec2);
void Route_Idle(Queue *, vec2);
void Route_Swoop(Queue *, vec2);

#endif

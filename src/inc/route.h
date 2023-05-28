#ifndef _ROUTE_H_
#define _ROUTE_H_

#include "path.h"
#include "queue.h"

void Route_Spawn(Queue *, vec2);
void Route_Idle(Queue *, vec2);
void Route_Swoop(Queue *, vec2);

#endif

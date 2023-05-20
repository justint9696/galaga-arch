#ifndef _PATH_H_
#define _PATH_H_

#include "entity.h"
#include "type.h"

#include <math.h>

#define DEG(rad) (rad * (180.f / M_PI)) 
#define RAD(deg) (deg * (M_PI / 180.f))

void Path_Circular(Entity *entity, vec2 org, vec2 dst, float speed);

void Path_Linear(Entity *entity, vec2 org, vec2 dst, float speed);

#endif

#ifndef _PATH_H_
#define _PATH_H_

#include "entity.h"
#include "type.h"

#include <math.h>

#define DEG(rad) (rad * (180.f / M_PI)) 

void Path_Circular(Entity *entity, const vec2 org, const vec2 dst, float speed);

void Path_Linear(Entity *entity, const vec2 org, const vec2 dst, float speed);

#endif

#ifndef _PATH_H_
#define _PATH_H_

#include "entity.h"
#include "type.h"

void Path_Circle(Entity *entity, vec2 dst, float speed);

void Path_Linear(Entity *entity, vec2 dst, float speed);

#endif

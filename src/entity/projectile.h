#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "entity/entity.h"

#define PROJECTILE_WIDTH 1.f
#define PROJECTILE_HEIGHT 15.f
#define PROJECTILE_VELOCITY 1.f
#define PROJECTILE_COOLDOWN 250

void projectile_init(Entity *, struct World *);

#endif

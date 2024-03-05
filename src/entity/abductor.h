#ifndef _ABDUCTOR_H_
#define _ABDUCTOR_H_

#include "entity/entity.h"
#include "gfx/window.h"

#define ABDUCTOR_SPAWN_HEALTH       1.f
#define ABDUCTOR_WIDTH              66.f * SCREEN_SCALAR 
#define ABDUCTOR_HEIGHT             62.67f * SCREEN_SCALAR
#define ABDUCTOR_FLAGS              (FLAG_COLLISION | FLAG_AI_CONTROLLED | FLAG_PARENT_REF)

void abductor_init(Entity *, World *);
void abductor_destroy(Entity *, World *);
void abductor_update(Entity *, World *);

#endif

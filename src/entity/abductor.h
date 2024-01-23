#ifndef _ABDUCTOR_H_
#define _ABDUCTOR_H_

#include "entity/entity.h"
#include "gfx/window.h"

#define ABDUCTOR_SPAWN_HEALTH       1.f
#define ABDUCTOR_IDLE_TIME          1500    
#define ABDUCTOR_WIDTH              66.f * SCREEN_SCALAR 
#define ABDUCTOR_HEIGHT             62.67f * SCREEN_SCALAR

void abductor_init(Entity *, struct World *);
void abductor_destroy(Entity *, struct World *);

#endif

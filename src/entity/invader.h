#ifndef _INVADER_H_
#define _INVADER_H_

#include "entity/entity.h"
#include "gfx/window.h"

#define INVADER_SPAWN_HEALTH        1.f
#define INVADER_IDLE_TIME           1500    
#define INVADER_WIDTH               66.67f * SCREEN_SCALAR
#define INVADER_HEIGHT              101.3f * SCREEN_SCALAR
#define INVADER_FLAGS               (FLAG_COLLISION | FLAG_AI_CONTROLLED)
#define INVADER_VELOCITY            0.3f

void invader_init(Entity *, struct World *);
void invader_destroy(Entity *, struct World *);
void invader_update(Entity *, struct World *);

#endif

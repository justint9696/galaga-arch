#ifndef _TRACTOR_BEAM_H
#define _TRACTOR_BEAM_H

#include "entity/entity.h"
#include "gfx/window.h"

#define TRACTOR_BEAM_SPAWN_HEALTH       1.f
#define TRACTOR_BEAM_WIDTH              10.f * SCREEN_SCALAR
#define TRACTOR_BEAM_HEIGHT             225.f * SCREEN_SCALAR
#define TRACTOR_BEAM_FLAGS              (FLAG_COLLISION)
#define TRACTOR_BEAM_VELOCITY           0.3f
#define TRACTOR_BEAM_DELAY              1250.f
#define TRACTOR_BEAM_EXPAND_TIME        100.f
#define TRACTOR_BEAM_TIME               (TRACTOR_BEAM_DELAY + TRACTOR_BEAM_EXPAND_TIME)

void tractor_beam_init(Entity *, struct World *);
void tractor_beam_update(Entity *, struct World *);
void tractor_beam_destroy(Entity *, struct World *);

#endif

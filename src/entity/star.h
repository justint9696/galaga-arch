#ifndef _STAR_H_
#define _STAR_H_

#include "entity/entity.h"

#define STAR_VELOCITY   0.4f
#define STAR_MAX        500
#define STAR_WIDTH      1.f
#define STAR_HEIGHT     STAR_WIDTH
#define STAR_COLOR      0xffffff00

void star_init(Entity *, World *);
void star_update(Entity *, World *);

#endif

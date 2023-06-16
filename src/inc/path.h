#ifndef _PATH_H_
#define _PATH_H_

#include "entity.h"
#include "type.h"

#include <math.h>

#define PI_2        (M_PI * 2)

#define DEG(rad)    (rad * (180.f / M_PI)) 
#define RAD(deg)    (deg * (M_PI / 180.f))

typedef enum {
    PATH_LINEAR,
    PATH_CIRCULAR,
    PATH_BEZIER,
} ptype_t;

typedef struct {
    float angle, time, speed;
    ptype_t type;
    vec2 org, dst;
} path_s;

path_s *Path_Init();
void Path_Linear(Entity *, path_s *);
void Path_Circular(Entity *, path_s *);
void Path_Bezier(Entity *, path_s *);

#endif

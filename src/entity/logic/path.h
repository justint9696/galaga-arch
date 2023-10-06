#ifndef _PATH_H_
#define _PATH_H_

#include "common/type.h"
#include "entity/entity.h"

#include <math.h>

#define PI_2        (M_PI * 2)

#define DEG(rad)    (rad * (180.f / M_PI)) 
#define RAD(deg)    (deg * (M_PI / 180.f))

typedef enum {
    PATH_LINEAR,
    PATH_CIRCULAR,
    PATH_BEZIER,
} ptype_t;

typedef enum {
    STATE_INACTIVE,
    STATE_ONGOING,
    STATE_COMPLETE,
} pstate_t;

typedef enum {
    ORIENT_DESTINATION,
    ORIENT_UPRIGHT,
} porient_t;

typedef struct {
    pstate_t state;
    porient_t orientation;
    float angle, time, speed;
    uint64_t tick;
    ptype_t type;
    vec2 org, dst;
    bool complete;
} path_s;

path_s *Path_Init();
void Path_Update(Entity *, path_s *);
float Path_Distance(const path_s *);
float Path_Time(const path_s *);

void Path_Linear(Entity *, path_s *);
void Path_Circular(Entity *, path_s *);
void Path_Bezier(Entity *, path_s *);

#endif

#ifndef _PATH_H_
#define _PATH_H_

#include "common/type.h"
#include "entity/entity.h"

#include <stdbool.h>
#include <math.h>

#define DEG(rad)    (rad * (180.f / M_PI)) 
#define RAD(deg)    (deg * (M_PI / 180.f))

#define PI_2        (M_PI * 2)

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
    ptype_t type;
    pstate_t state;
    porient_t orientation;
    float angle, speed;
    uint32_t tick;
    union {
        vec2 org, origin;
    };
    union {
        vec2 dst, destintation;
    };
    bool complete;
} Path;

Path *path_init();
void path_update(Path *, Entity *);
float path_distance(const Path *);
float path_time(const Path *);

#endif

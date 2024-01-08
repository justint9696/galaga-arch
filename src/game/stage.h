#ifndef _STAGE_H_
#define _STAGE_H_

#include "data/queue.h"

struct World; 

// number of enemies per wave
#define WAVE_COUNT 5

typedef enum {
    WAVE_ONE = 0,
    WAVE_TWO,
    WAVE_THREE,
    // WAVE_FOUR,
    WAVE_COMPLETE,
} wave_t;

#define WAVE_MAX WAVE_COMPLETE

typedef struct {
    // id for current entity to determine place in formation
    uint32_t id;

    // current stage
    uint32_t current;

    // current wave of the stage
    wave_t wave;

    // entity spawn queue
    Queue queue;
} Stage;

void stage_init(Stage *);
void stage_destroy(Stage *);
void stage_update(Stage *, struct World *);

#endif

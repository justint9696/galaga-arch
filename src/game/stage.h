#ifndef _STAGE_H_
#define _STAGE_H_

#include "data/queue.h"

#include <stdbool.h>

struct World; 

// number of enemies per wave
#define WAVE_COUNT 6

// cooldown after each wave of enemy swoops
#define SWOOP_COOLDOWN 1500

// delay between each enemy swoop
#define SWOOP_DELAY 1000

// maximum number of enemies to swoop
#define SWOOP_COUNT 2

typedef enum {
    WAVE_ONE = 0,
    WAVE_TWO,
    WAVE_THREE,
    // WAVE_FOUR,
} wave_t;

#define WAVE_MAX WAVE_THREE + 1

typedef enum {
    S_IDLE = 0,
    S_WAIT,
    S_SPAWN,
    S_ATTACK,
} stage_t;

#define S_MAX S_ATTACK + 1

typedef struct {
    // id for current entity to determine place in formation
    uint32_t id;

    // current stage
    uint32_t current;

    // current stage state
    stage_t state;

    // state to transition to after delay
    stage_t next_state;

    // current wave of the stage
    wave_t wave;

    // tick used to track delays
    uint32_t tick;

    // duration to delay after tick
    uint32_t delay;

    // entity spawn queue
    Queue queue;
} Stage;

void stage_init(Stage *, struct World *);
void stage_destroy(Stage *, struct World *);
void stage_update(Stage *, struct World *);

#endif

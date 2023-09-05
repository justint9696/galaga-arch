#include "entity/logic/spawn.h"

/**
 * wave 1: top center
 * wave 2: bottom left
 * wave 3: bottom right
 * wave 4: top center
 */
static vec2 _formation_one(ewave_t wave) {
    switch (wave) {
        case WAVE_ONE:
            return (vec2) { .x = WINDOW_WIDTH / 2.f, .y = WINDOW_HEIGHT };
        case WAVE_TWO:
            return (vec2) { .x = -ENEMY_WIDTH, .y = ENEMY_SPAWN_Y - 250.f };
        case WAVE_THREE:
            return (vec2) { .x = WINDOW_WIDTH, .y =  ENEMY_SPAWN_Y - 250.f };
        // case WAVE_FOUR:
        //     break;
        default:
            return (vec2) { .x = -ENEMY_WIDTH, .y =  ENEMY_SPAWN_Y - 250.f };
    }

    return (vec2) { .x = 0, .y = 0 };
}

static inline vec2 _formation_two(ewave_t wave) {}

static inline vec2 _formation_three(ewave_t wave) {}

/**
 * based on formation, enemies should spawn at different points off-screen
 * in formation one: 8 enemies in a line on one side of the screen
 * in formation two: 2 rows of 4 enemies in a line on one side of the screen
 * in formation three: 8 enemies in a line on both sides of the screen
 * in formation four: same as formation three, and back to formation one
 * where on the next four, there is a speed scalar
 */
vec2 Spawn_GetPosition(ewave_t wave, eformation_t formation) {
    switch (formation) {
        case FORMATION_ONE:
            return _formation_one(wave);
        case FORMATION_TWO:
            return _formation_two(wave);
        case FORMATION_THREE:
        case FORMATION_FOUR:
            return _formation_three(wave);
        default:
            return _formation_one(wave);
    }
}

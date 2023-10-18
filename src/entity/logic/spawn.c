#include "entity/logic/spawn.h"

/**
 * wave 1: top center
 * wave 2: bottom left
 * wave 3: bottom right
 * wave 4: top center
 */

/**
 * based on formation, enemies should spawn at different points off-screen
 * in formation one: 8 enemies in a line on one side of the screen
 * in formation two: 2 rows of 4 enemies in a line on one side of the screen
 * in formation three: 8 enemies in a line on both sides of the screen
 * in formation four: same as formation three, and back to formation one
 * where on the next four, there is a speed scalar
 */
vec2 Spawn_GetOrigin(ewave_t wave) {
    switch (wave) {
        case WAVE_ONE:
        // case WAVE_FOUR:
            return (vec2) { .x = WINDOW_WIDTH / 2.f, .y = WINDOW_HEIGHT };
        case WAVE_TWO:
            return (vec2) { .x = -ENEMY_WIDTH, .y = ENEMY_SPAWN_Y - 250.f };
        case WAVE_THREE:
            return (vec2) { .x = WINDOW_WIDTH, .y =  ENEMY_SPAWN_Y - 250.f };
        default:
            return (vec2) { .x = -ENEMY_WIDTH, .y =  ENEMY_SPAWN_Y - 250.f };
    }
}

vec2 Spawn_GetDestination(uint64_t *data) {
    return (v2) { .x = 0.f, .y = 0.f };
}
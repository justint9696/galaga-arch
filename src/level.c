#include "inc/level.h"

/**
 * TODO: 
 *     - dynamically decide enemy count and positions
 *     - dynamically decide enemy types
 *     - dynamic difficulty
 *     - speed scalar based on level
 */

static int _level;

void Level_Init() {
    _level = 1;
}

int Level_EnemyCount() {
    return 1;
}

int Level_Increment() {
    return ++_level;
}

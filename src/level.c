#include "inc/level.h"

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

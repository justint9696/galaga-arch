#include "inc/enemy.h"

static void _Enemy_Init() {
    Entity_Init(TYPE_ENEMY, TEAM_ENEMY, ENEMY_SPAWN_X, ENEMY_SPAWN_Y, ENEMY_WIDTH, ENEMY_HEIGHT, ENEMY_TEXTURE);
}

void Enemy_InitAll(const int count) {
    _Enemy_Init();
}

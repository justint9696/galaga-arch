#ifndef _WORLD_H_
#define _WORLD_H_

#include "data/linked_list.h"

#include "entity/entity.h"
#include "entity/player.h"
#include "entity/logic/enemy.h"

#include "gfx/window.h"

typedef struct {
    Player *player;
    LinkedList *entities;
} world_s;

void World_Init(uint64_t tick);
void World_Update(uint64_t tick, uint64_t deltaTime);
void World_Destroy();

#endif

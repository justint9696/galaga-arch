#ifndef _WORLD_H_
#define _WORLD_H_

#include "data/linked_list.h"

#include "entity/entity.h"
#include "entity/player.h"

#include "gfx/window.h"

#define DATA_SIZE       WINDOW_WIDTH * WINDOW_HEIGHT

typedef struct {
    Player player;
    LinkedList entities;
    uint64_t *data;
} World;

void World_Init(World *self, uint64_t tick);
void World_Update(World *self, uint64_t tick, uint64_t deltaTime);
void World_Destroy(World *self);

#endif

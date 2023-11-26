#ifndef _WORLD_H_
#define _WORLD_H_

#include "data/linked_list.h"
#include "entity/player.h"
#include "entity/logic/formation.h"
#include "gfx/window.h"
#include "game/buttons.h"
#include "common/type.h"

#define DATA_SIZE       WINDOW_WIDTH * WINDOW_HEIGHT

#ifdef ENVIRONMENT64
    typedef uint64_t data_t;
#else
    typedef uint32_t data_t;
#endif

typedef struct {
    Player player;
    LinkedList entities;
    uint32_t count;
    data_t *data;

    Formation formation;
} World;

void World_Init(World *self, uint64_t tick);
void World_Update(World *self, Buttons *buttons, uint64_t tick, uint64_t deltaTime);
void World_Destroy(World *self);

#endif

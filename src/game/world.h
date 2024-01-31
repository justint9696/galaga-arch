#ifndef _WORLD_H_
#define _WORLD_H_

#include "common/type.h"
#include "data/linked_list.h"
#include "entity/entity.h"
#include "gfx/window.h"

#include <stdbool.h>

#define DATA_SIZE SCREEN_WIDTH * SCREEN_HEIGHT

typedef struct World {
    // contains the addresses of each entity that occupies a space within the world
    data_t *data;

    Entity *player;
    Entity *formation;

    // used to limit the attack rate of enemies
    uint32_t tick;

    // contains all the entities in the world
    LinkedList entities;

    // contains all the enemies in the world
    LinkedList enemies;
} World;

void world_init(World *);
void world_destroy(World *);
void world_update(World *);

void world_add_entity(World *, Entity *);
void world_remove_entity(World *, Entity *);

#endif

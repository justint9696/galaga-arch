#ifndef _POOL_H_
#define _POOL_H_

#include "entity/entity.h"

#include <stddef.h>

typedef enum {
    S_FREE = 0,
    S_USED = 1,
} slot_t;

typedef struct {
    slot_t slot;
    Entity data;
} item_s;

typedef struct {
    size_t size;
    item_s *items;
} EntityList;

void entity_prepare_all();
void entity_release_all();

Entity *entity_request();
void entity_release(Entity *);

#endif 

#ifndef _POOL_H_
#define _POOL_H_

#include "entity/entity.h"

#include <stddef.h>

typedef struct {
    size_t size;
    Entity *items;
} Pool;

void entity_prepare_all();
void entity_release_all();

Entity *entity_request();
void entity_release(Entity *);

#endif 

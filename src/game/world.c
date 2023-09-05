#include "game/world.h"
#include "entity/entity.h"
#include "data/queue.h"
#include "gfx/hud.h"

#include <assert.h>
#include <stdlib.h>

void World_Init(World *self, uint64_t tick) {
    memset(self, 0, sizeof(World));
    Player_Init(&self->player, tick);
    LinkedList_Add(&self->entities, &self->player.entity);
}

void World_Update(World *self, uint64_t tick, uint64_t deltaTime) {
    Player_Update(&self->player, tick, deltaTime);
    
    Entity *entity;
    Node *tmp = self->entities.head;
    int count = 0;
    while (tmp) {
        count ++;
        entity = (Entity *)tmp->item;
        assert(entity);

        tmp = tmp->next;
        if (!Entity_IsAlive(entity)) {
            LinkedList_Remove(&self->entities, entity);
            continue;
        }

        Entity_Update(entity, deltaTime);
    }

    Hud_AddText("Entities: %i", count);
}

void World_Destroy(World *self) {
    memset(self, 0, sizeof(World));
}

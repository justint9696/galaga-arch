#include "game/world.h"
#include "entity/entity.h"
#include "gfx/hud.h"

#include <assert.h>
#include <stdlib.h>

static int get_offset(int x, int y) {
    return x + (y * WINDOW_WIDTH);
}

static void *get(uint64_t *data, int x, int y) {
    return (void *)data[get_offset(x, y)]; 
}

static void update(World *world, int x, int y, const void *item) {
    memcpy(&world->data[get_offset(x, y)], &item, sizeof(void *));
}

static void clear(World *world, int x, int y) {
    memset(&world->data[get_offset(x, y)], 0, sizeof(void *));
}

static void _World_Clear(World *self, Entity *entity) {
    v2 pos = entity->pos, dim = entity->dim;
    v2 size = {
        .x = pos.x + dim.w,
        .y = pos.y + dim.h,
    };
    for (int y = pos.y; y < size.y; y++) {
        for (int x = pos.x; x < size.x; x++)
            clear(self, x, y);
    }
}

static void _World_Collision(World *self, Entity *entity) {
    Entity *ent;
    v2 pos = entity->pos, dim = entity->dim;
    v2 size = {
        .x = pos.x + dim.w,
        .y = pos.y + dim.h,
    };
    for (int y = pos.y; y < size.y; y++) {
        for (int x = pos.x; x < size.x; x++) {
            ent = (Entity *)get(self->data, x, y);        
            if (!ent || ent == entity)
                continue;

            if (ent->team == entity->team)
                continue;

            if (Entity_Collision(entity, ent)) {
                Entity_Collide(entity, ent);
                _World_Clear(self, entity);
                _World_Clear(self, ent);
                return;
            }
        }
    }
}

static void _World_Update(World *self, vec2 p_pos, const Entity *entity) {
    vec2 size = {
        .x = p_pos.x + entity->dim.w,
        .y = p_pos.y + entity->dim.y
    };

    for (int y = p_pos.y; y < size.y; y++) {
        for (int x = p_pos.x; x < size.x; x++)
            update(self, x, y, 0);
    }

    size = (vec2) {
        .x = entity->pos.x + entity->dim.w,
        .y = entity->pos.y + entity->dim.y
    };

    for (int y = entity->pos.y; y < size.y; y++) {
        for (int x = entity->pos.x; x < size.x; x++) {
            update(self, x, y, entity);
        }
    }
}

void World_Init(World *self, uint64_t tick) {
    memset(self, 0, sizeof(World));
    self->data = calloc(1, DATA_SIZE * sizeof(void *));
    Player_Init(&self->player, tick);
    LinkedList_Add(&self->entities, &self->player.entity);
}

void World_Update(World *self, uint64_t tick, uint64_t deltaTime) {
    Player_Update(&self->player, &self->entities, tick, deltaTime);
    
    int count = 0;
    vec2 p_pos;
    Entity *entity;
    Node *tmp = self->entities.head;
    while (tmp) {
        entity = (Entity *)tmp->item;
        assert(entity);

        tmp = tmp->next;
        if (!Entity_IsAlive(entity)) {
            LinkedList_Remove(&self->entities, entity);

            if (entity->child) {
                _World_Clear(self, entity);
                free(entity);
            }

            continue;
        }

        _World_Collision(self, entity);

        memcpy(&p_pos, &entity->pos, sizeof(vec2));
        Entity_Update(entity, deltaTime);

        _World_Update(self, p_pos, entity);

        count++;
    }

    Hud_AddText("Entities: %i", count);
}

void World_Destroy(World *self) {
    free(self->data);
    Player_Destroy(&self->player);
    LinkedList_Free(&self->entities);
    memset(self, 0, sizeof(World));
}

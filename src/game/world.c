#include "game/world.h"
#include "gfx/hud.h"

#include <assert.h>
#include <stdlib.h>

#define size_loop(a, b) \
    for (size_t y = a.y; y < b.y && y < WINDOW_HEIGHT; y++) \
        for (size_t x = a.x; x < b.x && x < WINDOW_WIDTH; x++)

static inline int _get_offset(int x, int y) {
    return x + (y * WINDOW_WIDTH);
}

static void *_get(const uint64_t *data, int x, int y) {
    return (void *)data[_get_offset(x, y)]; 
}

static void _update(World *world, int x, int y, const void *item) {
    memcpy(&world->data[_get_offset(x, y)], &item, sizeof(void *));
}

static void _clear(World *world, int x, int y) {
    memset(&world->data[_get_offset(x, y)], 0, sizeof(void *));
}

static void _World_Clear(World *self, Entity *entity) {
    vec2 pos = entity->pos, dim = entity->dim;
    vec2 size = {
        .x = pos.x + dim.w,
        .y = pos.y + dim.h,
    };
    size_loop(pos, size) {
        _clear(self, x, y);
    }
}

static void _World_Collision(World *self, Entity *entity) {
    Entity *ent;
    vec2 pos = entity->pos, dim = entity->dim;
    vec2 size = {
        .x = pos.x + dim.w,
        .y = pos.y + dim.h,
    };
    size_loop(pos, size) {
        ent = (Entity *)_get(self->data, x, y);        
        if (!ent || ent == entity || 
                (ent->type == entity->type && entity->type == TYPE_PROJECTILE) ||
                (ent->team == entity->team))
            continue;

        if (Entity_Collision(entity, ent)) {
            Entity_Collide(entity, ent);

            _World_Clear(self, entity);
            _World_Clear(self, ent);

            return;
        }
    }
}

static void _World_Update(World *self, vec2 p_pos, const Entity *entity) {
    vec2 size = {
        .x = p_pos.x + entity->dim.w,
        .y = p_pos.y + entity->dim.h
    };

    size_loop(p_pos, size) {
        _clear(self, x, y);
    }

    size = (vec2) {
        .x = entity->pos.x + entity->dim.w,
        .y = entity->pos.y + entity->dim.h
    };

    size_loop(entity->pos, size) {
        _update(self, x, y, entity);
    }
}

void World_Init(World *self, uint64_t tick) {
    memset(self, 0, sizeof(World));
    self->data = calloc(1, DATA_SIZE * sizeof(void *));
    assert(self->data);

    Formation_Init(&self->formation);
    LinkedList_Add(&self->entities, &self->formation.entity);

    Player_Init(&self->player, tick);
    LinkedList_Add(&self->entities, &self->player.entity);
}

void World_Update(World *self, Buttons *buttons, uint64_t tick, uint64_t deltaTime) {
    Entity *entity = Player_Update(&self->player, buttons, tick, deltaTime);
    Formation_Update(&self->formation);
    // Hud_AddText("Center: (%.2f, %.2f)", self->formation.entity.pos.x, self->formation.entity.pos.y);

    if (entity)
        LinkedList_Add(&self->entities, entity);
    
    vec2 p_pos;
    Node *tmp = self->entities.head;
    while (tmp) {
        entity = (Entity *)tmp->item;
        assert(entity);

        tmp = tmp->next;
        if (!Entity_IsAlive(entity)) {
            LinkedList_Remove(&self->entities, entity);
            _World_Clear(self, entity);

            if (entity->type == TYPE_PROJECTILE)
                free(entity);

            continue;
        }

        memcpy(&p_pos, &entity->pos, sizeof(vec2));
        Entity_Update(entity, deltaTime);

        if (entity->type == TYPE_FORMATION)
            continue;

        _World_Collision(self, entity);
        _World_Update(self, p_pos, entity);
    }
}

static inline void _destroy_projectiles(LinkedList *lst) {
    Entity *entity;
    Node *tmp = lst->head; 

    while (tmp) {
        entity = (Entity *)tmp->item;
        assert(entity);

        tmp = tmp->next;
        switch (entity->type) {
            case TYPE_PROJECTILE:
                free(entity);
                break;
            default:
                break;
        }
    }
}

void World_Destroy(World *self) {
    free(self->data);
    _destroy_projectiles(&self->entities);
    LinkedList_Free(&self->entities);
}
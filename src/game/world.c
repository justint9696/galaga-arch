#include "game/world.h"

#include "entity/formation.h"
#include "entity/player.h"
#include "entity/pool.h"
#include "entity/star.h"

#include <assert.h>
#include <string.h>

#define data_loop(_a, _b) \
    for (size_t y = _a.y; y < _b.y && y < SCREEN_HEIGHT; y++) \
        for (size_t x = _a.x; x < _b.x && x < SCREEN_WIDTH; x++)

#define get_offset(x, y) (x + (y * SCREEN_WIDTH))

static void clear(World *world, int x, int y) {
    memset(&world->data[get_offset(x, y)], 0, sizeof(void *));
}

static void update(World *world, int x, int y, void *value) {
    memcpy(&world->data[get_offset(x, y)], &value, sizeof(void *));
}

static void *get_item(World *world, int x, int y) {
    return (void *)world->data[get_offset(x, y)]; 
}

static void world_clear_previous(World *self, Entity *entity, vec2 prev_pos) {
    vec2 size = {
        .x = prev_pos.x + entity->dim.width,
        .y = prev_pos.y + entity->dim.height,
    };

    data_loop(prev_pos, size)
        clear(self, x, y);
}

static void world_update_data(World *self, Entity *entity, vec2 prev_pos) {
    // clear the entity's previous position
    world_clear_previous(self, entity, prev_pos);

    // update the current position
    vec2 size = (vec2) {
        .x = entity->pos.x + entity->dim.width,
        .y = entity->pos.y + entity->dim.height,
    };

    data_loop(entity->pos, size)
        update(self, x, y, entity);
}

static void world_clear(World *self, Entity *entity) {
    vec2 size = {
        .x = entity->pos.x + entity->dim.width,
        .y = entity->pos.y + entity->dim.height,
    };

    data_loop(entity->pos, size)
        clear(self, x, y);
}

static void world_check_collision(World *self, Entity *entity) {
    Entity *e;
    vec2 size = {
        .x = entity->pos.x + entity->dim.width,
        .y = entity->pos.y + entity->dim.height,
    };

    data_loop(entity->pos, size) {
        e = (Entity *)get_item(self, x, y);
        if (!e || e == entity ||
                (e->type == E_PROJECTILE && entity->type == E_PROJECTILE) || 
                (e->team == entity->team))
            continue;

        if (entity_collision(entity, e)) {
            // do not clear primary entity position here
            // the previous position also needs to be cleared
            entity_damage(entity);

            entity_damage(e);
            if (!entity_is_alive(e)) 
                world_clear(self, e);

            return;
        }
    }
}

void world_init(World *self) {
    memset(self, 0, sizeof(World));
    self->data = calloc(DATA_SIZE, sizeof(data_t));

    // prepare entity pool
    entity_prepare_all();

    Entity *e;
    for (size_t i = 0; i < STAR_MAX; i++) {
        e = entity_init(E_STAR, star_init, NULL, star_update, self); 
        world_add_entity(self, e);
    }

    self->player = entity_init(E_PLAYER, player_init, NULL, player_update, self);
    world_add_entity(self, self->player);

    self->formation = entity_init(E_FORMATION, formation_init, NULL, formation_update, self);
    world_add_entity(self, self->formation);
}

void world_destroy(World *self) {
    free(self->data);
    list_clear(&self->entities);
    list_clear(&self->enemies);
    entity_release_all();
}

void world_update(World *self) {
    vec2 pos;
    Entity *e;
    Node *tmp = self->entities.head;
    while (tmp != NULL) {
        e = (Entity *)tmp->item;
        assert(e);

        tmp = tmp->next;
        if (!entity_is_alive(e)) {
            world_remove_entity(self, e);
            entity_destroy(e, self);
            continue;
        }

        // store entity position before update
        pos = e->pos;
        entity_update(e, self);

        // does entity have collision flag
        if (e->flags & FLAG_COLLISION) {
            world_check_collision(self, e);
            if (entity_is_alive(e))
                world_update_data(self, e, pos);
            else
                world_clear_previous(self, e, pos);
        }
    }
}

void world_add_entity(World *self, Entity *e) {
    list_add(&self->entities, e);
    switch (e->type) {
        case E_ENEMY:
            list_add(&self->enemies, e);
            break;
        default:
            break;
    }
}

void world_remove_entity(World *self, Entity *e) {
    list_remove(&self->entities, e);
    switch (e->type) {
        case E_ENEMY:
            list_remove(&self->enemies, e);
            break;
        default:
            break;
    }
}

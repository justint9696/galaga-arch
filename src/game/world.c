#include "game/world.h"

#include "entity/logic/ai.h"
#include "entity/entity.h"
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

static vec2 get_size(vec2 pos, vec2 dim) {
    return (vec2) {
        .x = pos.x + dim.w,
        .y = pos.y + dim.y,
    };
}

static void world_clear_previous(World *self, Entity *entity) {
    vec2 size = get_size(entity->prev_pos, entity->dim);
    data_loop(entity->prev_pos, size)
        clear(self, x, y);
}

static void world_update_data(World *self, Entity *entity) {
    // clear the entity's previous position
    world_clear_previous(self, entity);

    // update the current position
    vec2 size = get_size(entity->pos, entity->dim);
    data_loop(entity->pos, size)
        update(self, x, y, entity);
}

static void world_clear(World *self, Entity *entity) {
    vec2 size = get_size(entity->pos, entity->dim);
    data_loop(entity->pos, size)
        clear(self, x, y);
}

static void world_check_collision(World *self, Entity *entity) {
    Entity *e;
    vec2 size = get_size(entity->pos, entity->dim);
    data_loop(entity->pos, size) {
        e = (Entity *)get_item(self, x, y);
        if (!e || e == entity ||
                (e->type == E_PROJECTILE && entity->type == E_PROJECTILE) || 
                (e->team == entity->team))
            continue;

        if (entity_collision(entity, e)) {
            entity_damage(entity);
            if (!entity_is_alive(e)) 
                world_clear_previous(self, e);

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
    assert(self->data);

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

        entity_update(e, self);

        // should entity respond to collision
        if (entity_has_flag(e, FLAG_COLLISION)) {
            world_check_collision(self, e);
            world_update_data(self, e);
        }

        // is entity controlled by an ai
        if (entity_has_flag(e, FLAG_AI_CONTROLLED)) {
            entity_do_ai(e, self);
        }
    }
}

void world_add_entity(World *self, Entity *e) {
    list_add(&self->entities, e);
    switch (e->type) {
        case E_ABDUCTOR:
        case E_INVADER:
            list_add(&self->enemies, e);
            break;
        default:
            break;
    }
}

void world_remove_entity(World *self, Entity *e) {
    list_remove(&self->entities, e);
    switch (e->type) {
        case E_ABDUCTOR:
        case E_INVADER:
            list_remove(&self->enemies, e);
            break;
        default:
            break;
    }
}

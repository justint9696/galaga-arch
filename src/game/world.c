#include "game/world.h"
#include "data/linked_list.h"
#include "entity/entity.h"

#include <stdlib.h>
#include <assert.h>

static world_s _world;
static LinkedList *_enemies, *_allies;

static LinkedList *_get_team_list(team_t team) {
    switch (team) {
        case TEAM_ALLY:
            return _allies;
        case TEAM_ENEMY:
            return _enemies;
    }
}

static void _add_team_list(Entity *entity) {
    LinkedList *list = _get_team_list(entity->team);
    LinkedList_Add(list, entity);
}

static void _remove_team_list(Entity *entity) {
    LinkedList *list = _get_team_list(entity->team);
    LinkedList_Remove(list, entity);
}

static void _collision_handler(Entity *e0) {
    LinkedList *list = _get_team_list(Entity_GetOtherTeam(e0->team));
    Node *tmp = list->head;
    Entity *e1;

    while (tmp) {
        e1 = (Entity *)tmp->item;
        assert(e1);
        tmp = tmp->next;

        if (Entity_Collision(e0, e1)) {
            Entity_Collide(e0, e1);
        }

    }
}

static void _world_update(uint64_t deltaTime) {
    Entity *entity;
    Node *tmp = _world.entities->head;
    while (tmp) {
        entity = (Entity *)tmp->item;
        tmp = tmp->next;

        _collision_handler(entity);
        _add_team_list(entity);
        Entity_Update(entity, deltaTime);
    }

    // LinkedList_Free(_allies);
    // LinkedList_Free(_enemies);
}

void World_Init(uint64_t tick) {
    memset(&_world, 0, sizeof(world_s));

    _allies = LinkedList_Init();
    _enemies = LinkedList_Init();

    _world.entities = Entity_InitAll();
    _world.player = Player_Init(tick);
}

void World_Update(uint64_t tick, uint64_t deltaTime) {
    Player_Update(tick, deltaTime);
    _world_update(deltaTime);
}

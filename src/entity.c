#include "include/app.h"
#include "include/entity.h"
#include "include/linked_list.h"
#include "include/render.h"
#include "include/util.h"
#include "include/window.h"

#include <assert.h>
#include <math.h>

static int _num_ent = 0;
static LinkedList _entities;

static inline Entity *_Entity_Alloc() {
    Entity *self = (Entity *)malloc(sizeof(Entity));
    memset(self, 0, sizeof(Entity));
    self->id = _num_ent++;

    LinkedList_Add(&_entities, (void *)self);

    return self;
}

static inline void _Entity_Free(Entity *self) {
    assert(self);

    LinkedList_Remove(&_entities, &self);
    free(self);

    --_num_ent;
}

static void _Entity_Render_Rect(Entity *self) {
    DrawRect(self->pos.x, self->pos.y, self->width, self->height, self->color);
}

static void _Entity_Render_Texure(Entity *self) {
    DrawTexture(self->texture, self->pos.x, self->pos.y, self->width, self->height);
}

static inline void _Entity_Update(Entity *self, uint64_t deltaTime) {
    // update entity position
    printf("test %i\n", self->id);
    self->pos.x += round(1.f * deltaTime * self->vel.x),
    self->pos.x = clamp(0, self->pos.x, WINDOW_WIDTH - self->width);

    printf("test %i\n", self->id);
    self->pos.y += round(1.f * deltaTime * self->vel.y);
    self->pos.y = clamp(0, self->pos.y, WINDOW_HEIGHT - self->height);

    #ifdef DEBUG
        //printf("\nentity debug information: \n\t- id: %i \n\t- origin (%.2f, %.2f) \n\t- velocity: (%.2f, %.2f)\n", self->id, self->pos.x, self->pos.y, self->vel.x, self->vel.y);
    #endif
    
    printf("test %i\n", self->id);
    ((void(*)(Entity *))self->render)(self);
}

void Entity_UpdateAll(uint64_t deltaTime) {
    Entity *entity;
    LinkedList *tmp = &_entities;

    while (tmp) {
        //printf("test %i hello\n", _num_ent);
        entity = (Entity *)tmp->item;
        printf("test %i assign\n", entity->id);
        _Entity_Update(entity, deltaTime);

        //printf("test %i %i\n", _num_ent, entity->id);
        tmp = tmp->next;
        //printf("test %i goodbye\n", _num_ent);
    }
}

Entity *Entity_Init(type_t type, team_t team, float x, float y, int width, int height, const char *texture) {
    Entity *self = _Entity_Alloc();

    self->type = type;
    self->team = team;

    self->pos.x = x;
    self->pos.y = y;

    self->width = width;
    self->height = height;

    self->texture = texture == NULL ? NULL : LoadTexture(texture);

    self->render = texture == NULL ? &_Entity_Render_Rect : &_Entity_Render_Texure;

    #ifdef DEBUG
        printf("entity %i initialized.\n", self->id);
    #endif

    return self;
}

void Entity_SetPosition(Entity *self, vec2 pos) {
    memcpy(&self->pos, &pos, sizeof(vec2));

    #ifdef DEBUG
        printf("entity %i's position updated.\n", self->id);
    #endif
}

void Entity_SetVelocity(Entity *self, vec2 vel) {
    memcpy(&self->vel, &vel, sizeof(vec2));

    #ifdef DEBUG
        printf("entity %i's velocity updated.\n", self->id);
    #endif
}

void Entity_Fire(team_t team, vec2 pos, vec2 vel) {
    Entity *self = Entity_Init(TYPE_PROJECTILE, team, pos.x, pos.y, BULLET_WIDTH, BULLET_HEIGHT, BULLET_TEXTURE);

    Entity_SetVelocity(self, vel);
}
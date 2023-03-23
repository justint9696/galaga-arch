#include "include/app.h"
#include "include/entity.h"
#include "include/linked_list.h"
#include "include/render.h"
#include "include/util.h"

#include <stdio.h>

static int _num_ent = 0;
static LinkedList _entities;

static Entity *Entity_Alloc() {
    Entity *self = (Entity *)malloc(sizeof(Entity));
    memset(self, 0, sizeof(Entity));

    LinkedList_Add(&_entities, (void *)self);

    return self;
}

static void Entity_Free(Entity *self) {
    LinkedList_Remove(&_entities, &self);
    free(self);
}

static void Entity_Render(Entity *self) {
    DrawTexture(self->texture, self->pos.x, self->pos.y, self->width, self->height);
}

void Entity_Update(Entity *self) {
    // update entity position
    self->pos.x += self->vel.x;
    self->pos.y += self->vel.x;

    #ifdef DEBUG
        printf("\nentity debug information: \n\t- id: %i \n\t- origin (%.2f, %.2f) \n\t- velocity: (%.2f, %.2f)\n", self->id, self->pos.x, self->pos.y, self->vel.x, self->vel.y);
    #endif

    Entity_Render(self);
}

void Entity_UpdateAll() {
    Entity *entity;
    LinkedList *tmp = &_entities;

    while (tmp) {
        entity = (Entity *)tmp->item;
        Entity_Update(entity);

        tmp = tmp->next;
    }
}

Entity *Entity_Init(float x, float y, int width, int height, const char *texture) {
    Entity *self = Entity_Alloc();

    self->id = _num_ent++;

    self->pos.x = x;
    self->pos.y = y;

    self->vel.x = 0.f;
    self->vel.y = 0.f;

    self->width = width;
    self->height = height;

    self->texture = LoadTexture(texture);

    #ifdef DEBUG
        printf("entity %i initialized.\n", self->id);
    #endif

    return self;
}

void Entity_SetPosition(Entity *self, float x, float y) {
    self->pos.x = x;
    self->pos.y = y;

    #ifdef DEBUG
        printf("entity %i's position updated.\n", self->id);
    #endif
}

void Entity_SetVelocity(Entity *self, float vx, float vy) {
    self->vel.x = vx;
    self->vel.y = vy;

    #ifdef DEBUG
        printf("entity %i's velocity updated.\n", self->id);
    #endif
}
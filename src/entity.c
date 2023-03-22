#include "include/app.h"
#include "include/entity.h"
#include "include/linked_list.h"

static int numEnt = 0;
static LinkedList entities;

static Entity *Entity_Alloc() {
    Entity *self = (Entity *)malloc(sizeof(Entity));
    memset(self, 0, sizeof(Entity));

    LinkedList_Add(&entities, &self);

    return self;
}

static void Entity_Free(Entity *self) {
    LinkedList_Remove(&entities, &self);
    free(self);
}

void Entity_Update(Entity *self) {
    self->pos.x += self->vel.x;
    self->pos.y += self->vel.x;
}

Entity *Entity_Init(float x, float y, int width, int height, const char *texture) {
    Entity *self = Entity_Alloc();

    self->pos.x = x;
    self->pos.y = y;

    self->vel.x = 0.f;
    self->vel.y = 0.f;

    self->width = width;
    self->height = height;

    self->texture = LoadTexture(texture);

    return self;
}

void Entity_SetPosition(Entity *self, float x, float y) {
    self->pos.x = x;
    self->pos.y = y;
}

void Entity_SetVelocity(Entity *self, float vx, float vy) {
    self->vel.x = vx;
    self->vel.y = vy;
}
#include "inc/app.h"
#include "inc/entity.h"
#include "inc/game.h"
#include "inc/linked_list.h"
#include "inc/render.h"
#include "inc/util.h"
#include "inc/window.h"

#include <assert.h>
#include <math.h>

static int _num_ent = 0;
static LinkedList *_entities;

static inline Entity *_Entity_Alloc() {
    Entity *self = (Entity *)malloc(sizeof(Entity));
    memset(self, 0, sizeof(Entity));
    self->id = _num_ent++;

    LinkedList_Add(_entities, (void *)self);

    printf("entity spawned. number of entities: %i\n", _num_ent);

    return self;
}

static inline void _Entity_Free(Entity *self) {
    assert(self);

    const int id = self->id;

    LinkedList_Remove(&_entities, (void *)self);
    free(self);

    printf("entity %i released. number of entities: %i\n", id, --_num_ent);
}

static inline void _Entity_Render_Rect(const Entity *self) {
    DrawRect(self->pos.x, self->pos.y, self->width, self->height, self->color);
}

static inline void _Entity_Render_Texure(const Entity *self) {
    DrawTexture(self->texture, self->pos.x, self->pos.y, self->width, self->height);
}

static inline void _Entity_Update(Entity *self, uint64_t deltaTime) {
    if ((self->pos.x > WINDOW_WIDTH || self->pos.x < 0.f) || 
            (self->pos.y > WINDOW_HEIGHT || self->pos.y < 0.f)) {
        _Entity_Free(self);
        return;
    }

    // update entity position
    self->pos.x += round(1.f * deltaTime * self->vel.x);
    self->pos.y += round(1.f * deltaTime * self->vel.y);

    #ifdef DEBUG
        //printf("\nentity debug information: \n\t- id: %i \n\t- origin (%.2f, %.2f) \n\t- velocity: (%.2f, %.2f)\n", self->id, self->pos.x, self->pos.y, self->vel.x, self->vel.y);
    #endif
    
    ((void(*)(Entity *))self->render)(self);
}

static inline vec2 _Entity_Midpoint(const Entity *self) {
    vec2 pos;
    pos.x = self->pos.x + (self->width / 2.f);
    pos.y = self->pos.y + (self->type == TYPE_PLAYER ? self->height : -self->height);
    return pos;
}

float Distance(const vec2 from, const vec2 to) {
    return fabs(from.x - to.x) + fabs(from.y - to.y);
}

void Entity_InitList() {
	_entities = (LinkedList *)malloc(sizeof(LinkedList));
	memset(_entities, 0, sizeof(LinkedList));
	
    assert(_entities);
	printf("Entity list initialized.\n");
}

void Entity_UpdateAll(uint64_t deltaTime) {
    Entity *entity;
    LinkedList *tmp = _entities;

    while (tmp) {
        entity = (Entity *)tmp->item;
        _Entity_Update(entity, deltaTime);

        tmp = tmp->next;
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

void Entity_Fire(Entity *self, uint64_t tick) {
    const uint64_t time = Time_Passed(self->tick);
    if (time < BULLET_DELAY)
        return;

    vec2 pos, vel;

    pos = _Entity_Midpoint(self);

    vel.x = 0.f;
    vel.y = self->team == TEAM_ALLY ? BULLET_VELOCITY : -BULLET_VELOCITY;

    Entity *entity = Entity_Init(TYPE_PROJECTILE, self->team, pos.x, pos.y, BULLET_WIDTH, BULLET_HEIGHT, BULLET_TEXTURE);

    Entity_SetVelocity(entity, vel);

    self->tick = tick;
}

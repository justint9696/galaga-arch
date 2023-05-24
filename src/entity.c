#include "inc/app.h"
#include "inc/entity.h"
#include "inc/game.h"
#include "inc/hud.h"
#include "inc/linked_list.h"
#include "inc/render.h"
#include "inc/time.h"
#include "inc/util.h"
#include "inc/window.h"

#include <assert.h>
#include <math.h>

static int _num_ent = 0;
static LinkedList *_entities;

bool Entity_IsAlive(const Entity *self) {
    return (self->state != STATE_DEAD && self->health > 0.f);
}

static team_t _Entity_GetOtherTeam(const team_t team) {
    return (team == TEAM_ALLY ? TEAM_ENEMY : TEAM_ALLY);
}

static inline Entity *_Entity_Alloc() {
    Entity *self = (Entity *)malloc(sizeof(Entity));
    memset(self, 0, sizeof(Entity));
    self->id = _num_ent++;

    LinkedList_Add(_entities, (void *)self);

    #ifdef DEBUG
        printf("entity spawned. number of entities: %i\n", _num_ent);
    #endif

    return self;
}

static inline void _Entity_Free(Entity *self) {
    assert(self);

    const int id = self->id;

    LinkedList_Remove(&_entities, (void *)self);
    free(self);

    _num_ent--;

    #ifdef DEBUG
        printf("entity %i released. number of entities: %i\n", id, --_num_ent);
    #endif
}

static void _Entity_Damage(Entity *self) {
    if (--self->health <= 0.f)
        self->state = STATE_DEAD;    
}

static inline void _Entity_Render_Rect(const Entity *self) {
    DrawRect(round(self->pos.x), round(self->pos.y), self->width, self->height, self->color);
}

static inline void _Entity_Render_Texure(const Entity *self) {
    DrawTexture(self->texture, round(self->pos.x), round(self->pos.y), self->width, self->height);
}

static LinkedList *_Entity_FilterByAll(const team_t team, const type_t type) {
    Entity *entity;
    LinkedList *head = (LinkedList *)malloc(sizeof(LinkedList)), *tmp = _entities;

    memset(head, 0, sizeof(LinkedList));

    while (tmp) {
        entity = (Entity *)tmp->item;
        assert(entity);

        if ((team == -1 && entity->type == type) ||
                (type == -1 && entity->team == team) ||
                (entity->team == team && entity->type == type))
            LinkedList_Add(head, tmp->item);

        tmp = tmp->next;
    }

    return head;
}

static inline bool _Entity_IsColliding(const Entity *e1, const Entity *e2) {
    return ((e1->pos.x + e1->width) >= e2->pos.x 
            && (e1->pos.y + e1->height) >= e2->pos.y)
            && e1->pos.x <= (e2->pos.x + e2->width)
            && e1->pos.y <= (e2->pos.y + e2->height);        
}

static void _Entity_Collide(Entity *e1, Entity *e2) {
    switch (e1->type) {
    case TYPE_PROJECTILE:
        _Entity_Free(e1);
        break;
    default:
        _Entity_Damage(e1);
        break;
    }

    switch (e2->type) {
    case TYPE_PROJECTILE:
        _Entity_Free(e2);
        break;
    default:
        _Entity_Damage(e2);
        break;
    }
}

static inline void _Entity_CollisionHandler(Entity *self) {
    Entity *entity;
    LinkedList *tmp = _Entity_FilterByAll(_Entity_GetOtherTeam(self->team), -1);

    while (tmp && tmp->item) {
        entity = (Entity *)tmp->item;
        assert(entity);

        if (Entity_IsAlive(entity) && _Entity_IsColliding(self, entity)) {
            #ifdef DEBUG
                printf("entity %i is colliding with entity %i\n", entity->id, self->id);
            #endif

            // _Entity_Free(self);
            //_Entity_Free(entity);

            // since enemies.c points to the same memory addresses, freeing entities breaks game
            // rather than freeing both entities, just free projectile and damage player/enemy ents
             _Entity_Collide(self, entity);

            // TODO: free entities upon loading a new level
             

            break;
        }
            
        tmp = tmp->next;
    }

    free(tmp);
}

static inline void _Entity_Update(Entity *self, uint64_t deltaTime) {
    if ((self->pos.x > (WINDOW_WIDTH + WINDOW_BUFFER) || self->pos.x < -WINDOW_BUFFER) || 
            (self->pos.y > (WINDOW_HEIGHT + WINDOW_BUFFER) || self->pos.y < -WINDOW_BUFFER)) {
        _Entity_Free(self);
        return;
    }

    // update entity position
    self->pos.x += (1.f * deltaTime * self->vel.x);
    self->pos.y += (1.f * deltaTime * self->vel.y);

    switch (self->type) {
    case TYPE_PLAYER:
        self->pos.x = clamp(0, self->pos.x, WINDOW_WIDTH - self->width);
        self->pos.y = clamp(0, self->pos.y, WINDOW_HEIGHT - self->height);
        break;
    default:
        break;
    }

    #ifdef DEBUG
        //printf("\nentity debug information: \n\t- id: %i \n\t- origin (%.2f, %.2f) \n\t- velocity: (%.2f, %.2f)\n", self->id, self->pos.x, self->pos.y, self->vel.x, self->vel.y);
    #endif
    
    _Entity_CollisionHandler(self);
    ((void(*)(Entity *))self->render)(self);
}

static inline vec2 _Entity_Midpoint(const Entity *self) {
    vec2 pos;
    pos.x = self->pos.x + (self->width / 2.f);
    pos.y = self->pos.y + (self->type == TYPE_PLAYER ? self->height : 0.f);
    return pos;
}

void Entity_InitAll() {
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
        assert(entity);

        if (Entity_IsAlive(entity)) {
            _Entity_Update(entity, deltaTime);
        }

        tmp = tmp->next;
    }
    Hud_AddText("Entities: %i", _num_ent);
}

Entity *Entity_Init(type_t type, team_t team, float health, float x, float y, int width, int height, const char *texture) {
    Entity *self = _Entity_Alloc();

    self->health = health;

    self->type = type;
    self->team = team;
    self->state = STATE_ALIVE;

    self->pos.x = x;
    self->pos.y = y;

    self->width = width;
    self->height = height;

    self->texture = texture == NULL ? NULL : LoadTexture(texture);

    switch (type) {
    case TYPE_PLAYER:
        self->color = COLOR_PLAYER;
        break;
    case TYPE_ENEMY:
        self->color = COLOR_ENEMY;
        break;
    case TYPE_PROJECTILE:
        self->color = COLOR_PROJECTILE;
        break;
    default:
        printf("ERROR: Unknown entity type: %i.\n", type);
        exit(1);
        break;
    }

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
        printf("entity %i's velocity updated to (%.2f, %.2f).\n", self->id, vel.x, vel.y);
    #endif
}

void Entity_Fire(Entity *self, uint64_t tick) {
    assert((self->type == TYPE_PLAYER) || (self->type == TYPE_ENEMY));
    
    const uint64_t time = Time_Passed(self->tick);
    if (time < BULLET_DELAY)
        return;

    vec2 pos, vel;

    pos = _Entity_Midpoint(self);

    vel.x = 0.f;
    vel.y = self->team == TEAM_ALLY ? BULLET_VELOCITY : -BULLET_VELOCITY;

    Entity *entity = Entity_Init(TYPE_PROJECTILE, self->team, 1.f, pos.x, pos.y, BULLET_WIDTH, BULLET_HEIGHT, BULLET_TEXTURE);

    Entity_SetVelocity(entity, vel);

    self->tick = tick;
}

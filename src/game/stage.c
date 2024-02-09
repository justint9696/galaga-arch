#include "common/time.h"
#include "common/util.h"

#include "entity/logic/enemy.h"

#include "entity/abductor.h"
#include "entity/formation.h"
#include "entity/invader.h"

#include "game/stage.h"
#include "game/world.h"

#include <assert.h>
#include <string.h>

static vec2 get_spawnpoint(wave_t wave) {
    switch (wave) {
        case WAVE_ONE:
        // case WAVE_FOUR:
            return (vec2) { .x = SCREEN_WIDTH / 2.f, .y = SCREEN_HEIGHT };
        case WAVE_TWO:
            return (vec2) { .x = -ENEMY_WIDTH, .y = ENEMY_SPAWN_Y - 250.f };
        case WAVE_THREE:
            return (vec2) { .x = SCREEN_WIDTH, .y =  ENEMY_SPAWN_Y - 250.f };
        default:
            return (vec2) { .x = -ENEMY_WIDTH, .y =  ENEMY_SPAWN_Y - 250.f };
    }
}

void stage_init(Stage *self) {
    memset(self, 0, sizeof(Stage));
    self->state = S_SPAWN;
}

static void destroy_queue(Queue *q) {
    Entity *e;
    size_t size = q->size;
    for (size_t i = 0; i < size; i++) {
        e = (Entity *)queue_front(q);
        assert(e);
        dequeue(q);
    }
}

void stage_destroy(Stage *self) {
    destroy_queue(&self->queue);
}

static bool wave_complete(Stage *self, World *world) {
    // all waves have been spawned
    if (self->wave >= WAVE_MAX)
        return false;

    // no waves spawned
    if (!self->wave)
        return true;

    // no enemies in the stage
    if (list_is_empty(&world->enemies))
        return true;

    // the last entity's path is done or the entity is undefined
    Entity *e = (Entity *)world->enemies.tail->item;
    return (!e || (e && queue_is_empty(&self->queue) && queue_is_empty(&e->path)));
}

static bool stage_complete(Stage *self, World *world) {
    return self->wave >= WAVE_MAX && list_is_empty(&world->enemies);
}

static void monitor_spawn_queue(Stage *self, World *world) {
    if (queue_is_empty(&self->queue))
        return;

    Entity *e = (Entity *)queue_front(&self->queue);
    assert(e);

    // ensure front entity is alive and has traveled far enough from spawn
    vec2 spawn = get_spawnpoint(self->wave - 1);
    if (entity_is_alive(e) && distance(e->pos, spawn) < 150.f)
        return;

    // dequeue entity & add new front to world entities
    dequeue(&self->queue);

    if (!queue_is_empty(&self->queue)) {
        e = (Entity *)queue_front(&self->queue);
        world_add_entity(world, e);
    }
}

static void monitor_attack_queue(Stage *self, World *world) {
    if (queue_is_empty(&self->queue)) {
        self->tick = NOW();
        self->state = S_IDLE; 
        return;
    }

    Entity *e = (Entity *)queue_front(&self->queue);
    assert(e);

    entity_set_state(e, STATE_SWOOP);

    // ensure front entity is alive and has traveled far enough from formation
    vec2 pos = formation_entity_position(world->formation, e);
    if (entity_is_alive(e) && distance(e->pos, pos) < 250.f)
        return;

    LOG("enemy dequeued: %i!\n", e->type);
    dequeue(&self->queue);
}

static void stage_next(Stage *self, World *world) {
    self->current++;
    self->id = 0;
    self->wave = 0;
    self->state = S_SPAWN;
    LOG("Stage updated: %i\n", self->id);
}

static bool spawn_complete(Stage *self, World *world) {
    if (self->wave < WAVE_MAX)
        return false;

    Entity *e = (Entity *)world->enemies.tail->item;
    return (e && queue_is_empty(&self->queue) && e->state == STATE_IDLE);
}

static bool enemy_waves_spawned(World *world) {
    return (world->enemies.size >= (WAVE_COUNT * WAVE_MAX));
}

static Entity *random_enemy(World *world) {
    Entity *e = NULL;
    size_t size = world->enemies.size;
    uint32_t index = RAND(size);
    for (size_t i = 0; i < size; i++) {
        e = (Entity *)list_get_index(&world->enemies, (index + i) % size);
        if (e->state == STATE_IDLE && enemy_in_formation(e, world))
            break;
    }

    return e;
}

static void spawn_enemies(Stage *self, World *world) {
    // check if all enemies are spawned and in formation
    if (spawn_complete(self, world)) {
        self->state = S_IDLE;
        return;
    }

    // check if current wave of enemies are in formation
    if (!wave_complete(self, world)) {
        monitor_spawn_queue(self, world);
        return;
    }

    // otherwise, clear queue, spawn next wave of enemies, & increment wave number
    queue_clear(&self->queue);

    Entity *e;
    vec2 spawn = get_spawnpoint(self->wave);
    for (size_t i = 0; i < WAVE_COUNT; i++) {
        e = entity_init(E_ABDUCTOR, abductor_init, abductor_destroy, abductor_update, world); 
        e->pos = spawn;
        e->id = self->id++;
        enqueue(&self->queue, e);

        // e = entity_init(E_INVADER, invader_init, invader_destroy, invader_update, world); 
        // e->pos = spawn;
        // e->id = self->id++;
        // enqueue(&self->queue, e);
    }

    // add front entity to world entities
    e = (Entity *)queue_front(&self->queue);
    world_add_entity(world, e);

    self->wave++;
    LOG("Stage wave updated: %i\n", self->wave);
}

static void attack_enemies(Stage *self, World *world) {
    if (time_since(self->tick) < SWOOP_COOLDOWN)
        return;

    Entity *e;
    size_t max_iter = 16;
    for (size_t i = 0; i < max_iter; i++) {
        if (self->queue.size >= 2 || self->queue.size >= world->enemies.size) 
            break;

        e = random_enemy(world);
        if (e && !queue_contains(&self->queue, e)) {
            enqueue(&self->queue, e);
        }
    }

    LOG("queue->size: %li\n", self->queue.size);
    self->state = S_ATTACK;
}

static void monitor_stage(Stage *self, World *world) {
    if (stage_complete(self, world))
        stage_next(self, world);
}

void stage_update(Stage *self, World *world) {
    switch (self->state) {
        case S_SPAWN:
            spawn_enemies(self, world);
            break;
        case S_IDLE:
            monitor_stage(self, world);
            attack_enemies(self, world);
            break;
        case S_ATTACK:
            monitor_stage(self, world);
            monitor_attack_queue(self, world);
            break;
    }
}

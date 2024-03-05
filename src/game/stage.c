#include "common/time.h"
#include "common/util.h"

#include "entity/logic/enemy.h"

#include "game/stage.h"
#include "game/world.h"

#include <assert.h>

static vec2 wave_get_spawnpoint(wave_t wave) {
    switch (wave) {
        case WAVE_ONE:
            // case WAVE_FOUR:
            return VEC2(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT);
        case WAVE_TWO:
            return VEC2(-ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f);
        case WAVE_THREE:
            return VEC2(SCREEN_WIDTH, ENEMY_SPAWN_Y - 250.f);
        default:
            return VEC2(-ENEMY_WIDTH, ENEMY_SPAWN_Y - 250.f);
    }
}

static bool spawn_complete(Stage *self, World *world) {
    if (self->wave < WAVE_MAX)
        return false;

    Entity *e = (Entity *)world->enemies.tail->item;
    return (e && queue_is_empty(&self->queue) && e->state == STATE_IDLE);
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
    return (!e || (e && queue_is_empty(&self->queue) && enemy_in_formation(e, world)));
}

static bool stage_complete(Stage *self, World *world) {
    return self->wave >= WAVE_MAX && list_is_empty(&world->enemies);
}

static void change_state_delayed(Stage *self, stage_t state, uint32_t delay) {
    self->tick = NOW();
    self->delay = delay;
    self->state = S_WAIT;
    self->next_state = state;
}

static void spawn_wave(Stage *self, World *world) {
    Entity *e;
    vec2 spawn = wave_get_spawnpoint(self->wave);
    for (size_t i = 0; i < WAVE_COUNT; i++) {
        e = entity_create(E_ABDUCTOR, world);
        e->pos = spawn;
        e->id = self->id++;
        enqueue(&self->queue, e);

        // e = entity_create(E_INVADER, world);
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

static void monitor_queue(Stage *self, World *world) {
    if (queue_is_empty(&self->queue))
        return;

    Entity *e = (Entity *)queue_front(&self->queue);
    assert(e);

    // ensure front entity is alive and has traveled far enough from spawn
    vec2 spawn = wave_get_spawnpoint(self->wave - 1);
    if (entity_is_alive(e) && distance(e->pos, spawn) < 150.f)
        return;

    // dequeue entity & add new front to world entities
    dequeue(&self->queue);

    if (!queue_is_empty(&self->queue)) {
        e = (Entity *)queue_front(&self->queue);
        world_add_entity(world, e);
    }
}

static void spawn_update(Stage *self, World *world) {
    // check if spawned enemies are in formation
    if (spawn_complete(self, world)) {
        change_state_delayed(self, S_ATTACK, SWOOP_DELAY);
        return;
    } 
    // if not, check if current wave of enemies are in formation
    else if (!wave_complete(self, world)) {
        monitor_queue(self, world);
        return;
    }

    // otherwise, clear queue and spawn next wave of enemies
    queue_clear(&self->queue);
    spawn_wave(self, world);
}

static void wait_update(Stage *self, World *world) {
    if (time_since(self->tick) > self->delay) {
        self->state = self->next_state;
    }
}

static void stage_next(Stage *self, World *world) {
    self->current++;
    self->id = 0;
    self->wave = 0;
    LOG("Stage updated: %i\n", self->id);
}

static void queue_random_enemy(Stage *self, World *world) {
    size_t size = world->enemies.size;
    if (self->queue.size >= size)
        return;

    size_t index = RAND(size);
    Entity *e = (Entity *)list_get(&world->enemies, index);
    while (queue_contains(&self->queue, e) || e->state != STATE_IDLE || !enemy_in_formation(e, world)) {
        index = (index + 1) % size;
        e = (Entity *)list_get(&world->enemies, index);
    }

    assert(e);

    e->state = STATE_SWOOP;
    enqueue(&self->queue, e);
}

static void idle_update(Stage *self, World *world) {
    if (queue_is_empty(&self->queue)) {
        change_state_delayed(self, S_ATTACK, SWOOP_COOLDOWN);
        return;
    }

    Entity *e = (Entity *)queue_front(&self->queue);
    assert(e);

    if (!entity_has_flag(e, FLAG_ACTIVE) || (e->state == STATE_IDLE && entity_is_alive(e) && enemy_in_formation(e, world)))
        dequeue(&self->queue);
}

static void attack_update(Stage *self, World *world) {
    if (self->queue.size >= world->enemies.size) {
        self->state = S_IDLE;
        return;
    }

    queue_random_enemy(self, world);

    if (self->queue.size < SWOOP_COUNT)
        change_state_delayed(self, S_ATTACK, SWOOP_DELAY);
    else
        self->state = S_IDLE;
}

static void monitor_stage(Stage *self, World *world) {
    if (!stage_complete(self, world))
        return;

    // TODO: change_state_delayed for stage transition
    self->state = S_SPAWN;
    stage_next(self, world);
}

void stage_init(Stage *self, World *world) {
    memset(self, 0, sizeof(Stage));
    self->state = S_SPAWN;
}

void stage_update(Stage *self, World *world) {
    monitor_stage(self, world);

    switch (self->state) {
        case S_IDLE:
            idle_update(self, world);
            break;
        case S_WAIT:
            wait_update(self, world);
            break;
        case S_SPAWN:
            spawn_update(self, world);
            break;
        case S_ATTACK:
            attack_update(self, world);
            break;
        default: break;
    }
}

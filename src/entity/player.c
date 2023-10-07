#include "game/buttons.h"
#include "entity/entity.h"
#include "entity/player.h"

#include <assert.h>

bool Player_IsAlive(const Player *self) {
    return Entity_IsAlive(&self->entity);
}

bool Player_IsMoving(const Player *self) {
    vec2 vel = Player_Velocity(self);
    return (vel.x > 0 || vel.y > 0);
}

vec2 Player_Position(const Player *self) {
    return self->entity.pos;
}

vec2 Player_Velocity(const Player *self) {
    return self->entity.vel;
}

void Player_Init(Player *self, uint64_t tick) {
    memset(self, 0, sizeof(Player));

    Entity_Init(&self->entity, TYPE_PLAYER, TEAM_ALLY, PLAYER_SPAWN_HEALTH, PLAYER_SPAWN_X, PLAYER_SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_TEXTURE);
    self->entity.tick = tick;

    printf("Player initialized.\n");
}

Entity *Player_Update(Player *self, uint64_t tick, uint64_t deltaTime) {
    if (!Player_IsAlive(self))
        return NULL;

    Entity *child = NULL;
    if (self->buttons & BUTTON_SPACE) 
        child = Entity_Fire(&self->entity, tick);
    
    if (self->p_buttons == self->buttons) 
        return child;

    memcpy(&self->p_buttons, &self->buttons, sizeof(uint32_t));

    // if (buttons & BUTTON_UP)
    //     vy = PLAYERvel;
    // else if (buttons & BUTTON_DOWN)
    //     vy = -PLAYERvel;

    vec2 vel = { .x = 0.f, .y = 0.f };
    if (self->buttons & BUTTON_LEFT)
        vel.x = -PLAYER_VELOCITY;
    else if (self->buttons & BUTTON_RIGHT)
        vel.x = PLAYER_VELOCITY;

    if (!memcmp(&self->entity.vel, &vel, sizeof(vec2)))
        return child;
        
    Entity_SetVelocity(&self->entity, vel);

    return child;
}

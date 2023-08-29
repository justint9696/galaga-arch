#include "game/buttons.h"
#include "entity/entity.h"
#include "entity/player.h"

static vec2 _vel;
static Player _player;

bool Player_IsAlive() {
    return Entity_IsAlive(_player.entity);
}

bool Player_IsMoving() {
    vec2 vel = Player_Velocity();
    return (vel.x > 0 || vel.y > 0);
}

vec2 Player_Position() {
    return _player.entity->pos;
}

vec2 Player_Velocity() {
    return _player.entity->vel;
}

Player *Player_Init(uint64_t tick) {
    memset(&_player, 0, sizeof(Player));
    _player.entity = Entity_Init(TYPE_PLAYER, TEAM_ALLY, PLAYER_SPAWN_HEALTH, PLAYER_SPAWN_X, PLAYER_SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_TEXTURE);
    _player.entity->tick = tick;

    printf("Player initialized.\n");

   return &_player;
}

void Player_Update(uint64_t tick, uint64_t deltaTime) {
    int buttons = Buttons_Get();
    if (!Player_IsAlive())
        return;

    if (buttons & BUTTON_SPACE) 
        Entity_Fire(_player.entity, tick);
    
    if (buttons == _player.buttons) 
        return;

    _player.buttons = buttons;
    memset(&_vel, 0, sizeof(vec2));

    // if (buttons & BUTTON_UP)
    //     vy = PLAYER_VEL;
    // else if (buttons & BUTTON_DOWN)
    //     vy = -PLAYER_VEL;

    if (buttons & BUTTON_LEFT)
        _vel.x = -PLAYER_VELOCITY;
    else if (buttons & BUTTON_RIGHT)
        _vel.x = PLAYER_VELOCITY;
        
    Entity_SetVelocity(_player.entity, _vel);
    Entity_Update(_player.entity, deltaTime);
}

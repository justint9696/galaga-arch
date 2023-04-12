#include "inc/entity.h"
#include "inc/player.h"

static Player _self;
static vec2 _vel;

bool Player_IsAlive() {
    return (_self.ent->state != STATE_DEAD);
}

vec2 Player_Position() {
    return _self.ent->pos;
}

vec2 Player_Velocity() {
    return _self.ent->vel;
}

void Player_Init(uint64_t tick) {
    memset(&_self, 0, sizeof(Player));
    _self.ent = Entity_Init(TYPE_PLAYER, TEAM_ALLY, PLAYER_SPAWN_HEALTH, PLAYER_SPAWN_X, PLAYER_SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_TEXTURE);
    _self.ent->tick = tick;

    printf("Player initialized.\n");
}

void Player_Update(int buttons, uint64_t tick) {
    if (_self.ent->state == STATE_DEAD)
        return;

    if (buttons & BUTTON_SPACE) 
        Entity_Fire(_self.ent, tick);
    
    if (buttons == _self.buttons) 
        return;

    _self.buttons = buttons;
    _vel.x = 0.f;
    _vel.y = 0.f;

    // if (buttons & BUTTON_UP)
    //     vy = PLAYER_VEL;
    // else if (buttons & BUTTON_DOWN)
    //     vy = -PLAYER_VEL;

    if (buttons & BUTTON_LEFT)
        _vel.x = -PLAYER_VELOCITY;
    else if (buttons & BUTTON_RIGHT)
        _vel.x = PLAYER_VELOCITY;
        
    Entity_SetVelocity(_self.ent, _vel);
}

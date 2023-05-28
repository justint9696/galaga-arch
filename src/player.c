#include "inc/buttons.h"
#include "inc/entity.h"
#include "inc/player.h"

static vec2 _vel;
static Player _self;

bool Player_IsAlive() {
    return Entity_IsAlive(_self.entity);
}

vec2 Player_Position() {
    return _self.entity->pos;
}

vec2 Player_Velocity() {
    return _self.entity->vel;
}

void Player_Init(uint64_t tick) {
    memset(&_self, 0, sizeof(Player));
    _self.entity = Entity_Init(TYPE_PLAYER, TEAM_ALLY, PLAYER_SPAWN_HEALTH, PLAYER_SPAWN_X, PLAYER_SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_TEXTURE);
    _self.entity->tick = tick;

    printf("Player initialized.\n");
}

void Player_Update(uint64_t tick) {
    int buttons = Buttons_Get();
    if (!Player_IsAlive())
        return;

    if (buttons & BUTTON_SPACE) 
        Entity_Fire(_self.entity, tick);
    
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
        
    Entity_SetVelocity(_self.entity, _vel);
}

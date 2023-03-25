#include "include/entity.h"
#include "include/player.h"

static Player _self;
static vec2 _vel;

void Player_Init() {
    memset(&_self, 0, sizeof(Player));
    _self.buttons = 0;
    _self.ent = Entity_Init(TYPE_PLAYER, TEAM_ALLY, PLAYER_SPAWN_X, PLAYER_SPAWN_Y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_TEXTURE);

    printf("Player initialized.\n");
}

void Player_Update(int buttons) {
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

    if (buttons & BUTTON_SPACE) {
        _vel.y = BULLET_VELOCITY;
        Entity_Fire(_self.ent->team, _self.ent->pos, _vel);
    }
}
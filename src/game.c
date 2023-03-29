#include "inc/game.h"
#include "inc/entity.h"
#include "inc/enemy.h"
#include "inc/player.h"
#include "inc/render.h"
#include "inc/stars.h"
#include "inc/time.h"

#include <SDL2/SDL_render.h>

static uint64_t _time, _delta_time;

static inline void _Game_Tick() {
    const uint64_t tick = Get_Tick();

    _delta_time = tick - _time;
    _time += _delta_time;
}

void Game_Init() {
    // store current tick
    _time = Get_Tick();

    // prepare entities
    Entity_InitList();
    Player_Init();
    Enemy_InitAll(1);

    // prepare gfx
    Stars_Init();
}

void Game_Main(int buttons) {
    _Game_Tick();
    Renderer_Prepare();
    Stars_Update(_time, _delta_time);
    Player_Update(buttons, _time);
    Entity_UpdateAll(_delta_time);
    Renderer_Update();
}

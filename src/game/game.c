#include "gfx/hud.h"
#include "gfx/renderer.h"
#include "gfx/stars.h"

#include "common/util.h"

#include "entity/ai/enemy.h"
#include "entity/player.h"

#include "game.h"
#include "fps.h"
#include "stage.h"
#include "time.h"

#include <SDL2/SDL.h>

static uint64_t _time, _start_time, _delta_time;

static inline void _Game_Tick() {
    const uint64_t tick = Time_GetTick();

    _delta_time = tick - _time;
    _time += _delta_time;

    Hud_AddText("Level time: %lis", ((Time_Passed(_start_time) / 1000)));
}

bool Game_IsRunning() {
    return (Player_IsAlive());
}

void Game_Init() {
    // prepare time
    _time = Time_GetTick();
    _start_time = _time;
    Time_Init();
    fps_init();

    // prepare entities
    Entity_InitAll();
    Player_Init(_time);

    // prepare gfx
    Hud_Init();
    Stars_Init();

    // prepare stage
    Stage_Init();
    Stage_Set();
}

void Game_Main() {
    if (Stage_Complete()) 
        Stage_Set();

    // frame start
    frame_start();

    // prepare renderer
    Renderer_Prepare();

    // update enemies
    Enemy_UpdateAll(_time);

    // update gfx
    Stars_Update(_delta_time);

    // update entities
    Player_Update(_time);
    Entity_UpdateAll(_delta_time);

    // draw hud
    Hud_Draw();

    // render screen
    Renderer_Update();

    // game tick
    _Game_Tick();
    frame_end();
    
    // frame delay
    fps_limit();

    // draw fps
    Hud_DrawFPS();
}

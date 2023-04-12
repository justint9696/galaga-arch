#include "inc/game.h"
#include "inc/entity.h"
#include "inc/enemy.h"
#include "inc/level.h"
#include "inc/hud.h"
#include "inc/player.h"
#include "inc/render.h"
#include "inc/stars.h"
#include "inc/time.h"
#include "inc/util.h"

#include <SDL2/SDL.h>

static float _fps;
static uint64_t _frame_start, _frame_end, _frame_time;
static uint64_t _time, _ticks, _delta_time;

static inline void _Calculate_FPS() {
   /*if (_frame_time) {
       _fps = (1000.f / (float)_frame_time);
       printf("FPS: %.2f\n", _fps);
   }*/
}

static inline void _Game_Tick() {
    const uint64_t tick = Get_Tick();

    _delta_time = tick - _time;
    _time += _delta_time;
    
    _frame_end = tick;
    _frame_time = _frame_end - _frame_start;

    _ticks++;
}

bool Game_IsRunning() {
    return (Player_IsAlive());
}

void Game_Init() {
    // store current tick
    _time = Get_Tick();

    // prepare level

    // prepare entities
    Entity_InitAll();
    Player_Init(_time);
    Enemy_InitAll(_time);

    // prepare gfx
    Hud_Init();
    Stars_Init();
}

void Game_Main(int buttons) {
    // store current tick
    _frame_start = Get_Tick();

    // prepare renderer
    Renderer_Prepare();

    // update enemies
    Enemy_UpdateAll(_time);

    // update gfx
    Stars_Update(_delta_time);

    // update entities
    Player_Update(buttons, _time);
    Entity_UpdateAll(_delta_time);

    // draw hud
    Hud_Draw();

    // render screen
    Renderer_Update();

    // game tick
    _Game_Tick();

    #ifdef DEBUG
        // _Calculate_FPS();
    #endif
}

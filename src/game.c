#include "include/game.h"
#include "include/entity.h"
#include "include/player.h"
#include "include/render.h"

#include <SDL2/SDL_render.h>
#include <sys/time.h>


static uint64_t _time = 0, _delta_time = 0;

static inline uint64_t _Get_Tick() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t tick = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    return tick;
}

void _Game_Tick() {
    uint64_t tick = _Get_Tick();

    _delta_time = tick - _time;
    _time = tick;
}

void Game_Init() {
    Player_Init();
}

void Game_Main(int buttons) {
    _Game_Tick();
    Renderer_Prepare();
    Player_Update(buttons);
    Entity_UpdateAll(_delta_time);
    Renderer_Update();
}
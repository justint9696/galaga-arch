#include "inc/game.h"
#include "inc/entity.h"
#include "inc/player.h"
#include "inc/render.h"

#include <SDL2/SDL_render.h>
#include <sys/time.h>

static uint64_t _time, _delta_time;

static inline uint64_t _Get_Tick() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    const uint64_t tick = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    return tick;
}

static void _Game_Tick() {
    const uint64_t tick = _Get_Tick();

    _delta_time = tick - _time;
    _time += _delta_time;
}

inline uint64_t Time_Passed(uint64_t tick) {
    const uint64_t time = _Get_Tick() - tick;
    return time;
}

void Game_Init() {
    _time = _Get_Tick();
    Entity_InitList();
    Player_Init();
}

void Game_Main(int buttons) {
    _Game_Tick();
    Renderer_Prepare();
    Player_Update(buttons, _time);
    Entity_UpdateAll(_delta_time);
    Renderer_Update();
}

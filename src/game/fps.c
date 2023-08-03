#include "fps.h"
#include "time.h"

#include <SDL2/SDL.h>

static fps_s _fps;

static inline int _frame_time() {
    return (_fps.end - _fps.start);
}

void frame_start() {
    _fps.start = Time_GetTick();
}

void frame_end() {
    _fps.end = Time_GetTick();
    _fps.time = _frame_time();
    _fps.frames++;
}

float fps_get() {
    float fps = _fps.frames / (Time_Ticks() / 1000.f);
    return fps;
}

void fps_init() {
    memset(&_fps, 0, sizeof(fps_s));
    _fps.start = Time_GetTick();
}

void fps_limit() {
    int delay = 1000.f / (FPS_TARGET - _fps.time);
    SDL_Delay(delay > 0 ? delay : 0);
}

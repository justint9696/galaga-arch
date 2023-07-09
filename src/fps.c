#include "inc/fps.h"
#include "inc/time.h"

#include <SDL2/SDL.h>

static fps_s _fps;

static inline uint64_t _get_frame_time() {
    return (_fps.end - _fps.start);
}

void frame_start() {
    _fps.start = Get_Tick();
}

void frame_end() {
    _fps.end = Get_Tick();
    _fps.frames++;
}

float fps_get() {
    return _fps.frames / (_get_frame_time()  / 1000.f);
}

void fps_init() {
    memset(&_fps, 0, sizeof(fps_s));
}

void fps_limit() {
    int delay = 1000.f / (FPS_TARGET - _get_frame_time());
    SDL_Delay(delay > 0 ? delay : 0);
}

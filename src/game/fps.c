#include "game/fps.h"
#include "game/time.h"

#include <SDL2/SDL.h>
#include <assert.h>

static fps_s *_fps;

static inline uint32_t _frame_time() {
    return (_fps->frame.end - _fps->frame.start);
}

void Frame_Start() {
    _fps->frame.start = Time_GetTick();
}

void Frame_End() {
    _fps->frame.end = Time_GetTick();
    _fps->frame.time = _frame_time();
    _fps->frames++;
    _fps->delta = (_fps->frame.end - _fps->ticks);
    _fps->ticks += _fps->delta;
}

float FPS_Get() {
    return (_fps->frames / (Time_Ticks() / 1000.f));
}

void FPS_Init(fps_s *fps) {
    assert(!_fps);

    memcpy(&_fps, &fps, sizeof(fps_s *));
    memset(fps, 0, sizeof(fps_s));

    fps->start = Time_GetTick();
    fps->ticks = _fps->start;
}

void FPS_Limit() {
    uint32_t delay = (1000.f / (FPS_TARGET - _fps->frame.time));
    SDL_Delay(delay ? delay : 0);
}

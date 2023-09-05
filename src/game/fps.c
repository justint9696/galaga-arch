#include "game/fps.h"
#include "game/time.h"

#include <SDL2/SDL.h>
#include <assert.h>

static inline uint32_t _frame_time(fps_s *fps) {
    return (fps->frame.end - fps->frame.start);
}

void Frame_Start(fps_s *fps) {
    fps->frame.start = Time_GetTick();
}

void Frame_End(fps_s *fps) {
    fps->frame.end = Time_GetTick();
    fps->frame.time = _frame_time(fps);
    fps->frames++;
    fps->delta = (fps->frame.end - fps->ticks);
    fps->ticks += fps->delta;
}

float FPS_Get(fps_s *fps) {
    return (fps->frames / (Time_Ticks() / 1000.f));
}

void FPS_Init(fps_s *fps) {
    assert(!fps);

    memcpy(&fps, &fps, sizeof(fps_s *));
    memset(fps, 0, sizeof(fps_s));

    fps->start = Time_GetTick();
    fps->ticks = fps->start;
}

void FPS_Limit(fps_s *fps) {
    uint32_t delay = (1000.f / (FPS_TARGET - fps->frame.time));
    SDL_Delay(delay ? delay : 0);
}

#ifndef _FPS_H_
#define _FPS_H_

#include <stdint.h>

#define FPS_TARGET      60
#define FRAME_DELAY     (1000.f / FPS_TARGET)

typedef struct {
    uint64_t start, delta, frames, ticks;
    struct {
        uint64_t start, end, time;
    } frame;
} fps_s;

void Frame_Start(fps_s *fps);
void Frame_End(fps_s *fps);

void FPS_Init(fps_s *fps);
void FPS_Limit(fps_s *fps);
float FPS_Get(const fps_s *fps);

#endif

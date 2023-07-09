#ifndef _FPS_H_
#define _FPS_H_

#include <stdint.h>

#define FPS_TARGET      60
#define FRAME_DELAY     (1000.f / FPS_TARGET)

typedef struct {
    int frames, time;
    uint64_t start, end;
} fps_s;

void frame_start();
void frame_end();

float fps_get();
void fps_init();
void fps_limit();

#endif

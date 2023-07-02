#ifndef _FPS_H_
#define _FPS_H_

#define FPS_TARGET      60
#define FRAME_DELAY     (1000.f / FPS_TARGET)

typedef struct {
    int previous, frames;
} fps_s;

#endif

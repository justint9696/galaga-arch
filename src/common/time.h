#ifndef _TIME_H_
#define _TIME_H_

#include <sys/time.h>
#include <stdint.h>

#define FPS_TARGET 60
#define FPS_DELAY (1000.f / FPS_TARGET)

#define NOW() ({\
    struct timeval tv;\
    gettimeofday(&tv, NULL);\
    (tv.tv_sec * 1000LL + tv.tv_usec / 1000);})

typedef struct {
    uint32_t start, delta, last_second, fps;
    struct {
        uint32_t previous, current, total;
    } frame;
    struct {
        uint32_t next, total;
    } tick;
} Time;

void time_init();
void time_update();
void time_limit();

uint32_t time_delta();
float time_fps();

#endif

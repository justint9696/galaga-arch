#include "inc/time.h"

#include <sys/time.h>
#include <stddef.h>

static uint64_t _start;

inline uint64_t Get_Tick() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    const uint64_t tick = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    return tick;
}

void Time_Init() {
    _start = Get_Tick();
}

uint64_t Time_Passed(uint64_t tick) {
    return (Get_Tick() - tick);
}

uint64_t Time_Ticks() {
    return (Get_Tick() - _start);
}


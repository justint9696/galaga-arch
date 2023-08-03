#include "time.h"

#include <sys/time.h>
#include <stddef.h>

static uint64_t _start;

inline uint64_t Time_GetTick() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    const uint64_t tick = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    return tick;
}

void Time_Init() {
    _start = Time_GetTick();
}

uint64_t Time_Passed(uint64_t tick) {
    return (Time_GetTick() - tick);
}

uint64_t Time_Ticks() {
    return (Time_GetTick() - _start);
}


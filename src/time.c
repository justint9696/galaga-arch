#include "inc/time.h"

#include <sys/time.h>
#include <stddef.h>

inline uint64_t Get_Tick() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    const uint64_t tick = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    return tick;
}

uint64_t Time_Passed(uint64_t tick) {
    return (Get_Tick() - tick);
}


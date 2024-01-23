#include "common/util.h"
#include "common/time.h"

#include <SDL2/SDL_timer.h>
#include <string.h>

static Time t;

void time_init() {
    const uint32_t now = NOW();
    memset(&t, 0, sizeof(Time));

    SRAND();

    t.start = now;
    t.last_second = now;
    t.frame.previous = now;
    t.tick.total = 0;
}

void time_update() {
    const uint32_t now = NOW();
    t.delta = now - t.frame.previous;
    t.tick.total += t.delta;

    t.frame.previous = now;
    t.frame.current++;

    if (now - t.last_second > 1000) {
        t.last_second = now;

        t.fps = t.frame.current;
        t.frame.total += t.frame.current;

        t.frame.current = 0;
        LOG("FPS: %i\tAverage FPS: %.2f\n", t.fps, time_fps());
    }
}

void time_limit() {
    int32_t delay = t.tick.next - t.tick.total; 
    t.tick.next += FPS_DELAY; 

    SDL_Delay(delay > 0 ? delay : 0);
}

void time_set_paused(bool paused) {
    t.paused = paused;
}

uint32_t time_since(uint32_t tick) {
    const uint32_t now = NOW();
    return (now - tick);
}

uint32_t time_delta() {
    if (t.paused)
        return 0.f;
    return t.delta > 0 ? t.delta : 1.f;
}

float time_fps() {
    return t.frame.total / (t.tick.total / 1000.f);
}

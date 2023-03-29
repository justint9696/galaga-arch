#include "inc/render.h"
#include "inc/stars.h"
#include "inc/time.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static uint16_t _tick = 0, _count = 0;
static Star _stars[STAR_MAX_COUNT];

static float _rand_velocity() {
    return (5.f * (1 + (rand() % 3)) / 100); 
}

static int _rand_x_position() {
    return (rand() % WINDOW_WIDTH);
}

static int _rand_y_position() {
    //float num = (rand() )
    return (rand() % WINDOW_HEIGHT);
}

static Star *_Star_Init(int index) {
    Star *star = &_stars[index];
    star->pos.x = _rand_x_position();
    star->vel.y = _rand_velocity();
    return star;
}

static void _Star_Reset(Star *self) {
    self->pos.y = 0;
}

static void _Star_Update(Star *self, uint64_t deltaTime) {
    self->pos.y += (1.f * deltaTime * self->vel.y);
}

static bool _Star_Check(Star *self) {
    return ((self->pos.y + STAR_HEIGHT) < WINDOW_HEIGHT); 
}

static void _Star_Render(Star *self) {
    DrawRect(self->pos.x, self->pos.y, STAR_WIDTH, STAR_HEIGHT, COLOR_STAR);
}

void Stars_Init() {
    // prepare random number gen
    srand(time(NULL));

    // initialize stars
    // _stars = (Star *)malloc(sizeof(Star) * STAR_MAX_COUNT);
    memset(_stars, 0, sizeof(Star) * STAR_MAX_COUNT);

    assert(_stars);

    // assign random star positions
    Star *star;
    for (int i = 0; i < STAR_MAX_COUNT; i++) {
        star = _Star_Init(i);
        star->pos.y = _rand_y_position();
    }

    printf("Stars initialized.\n");
}

void Stars_Update(uint64_t tick, uint64_t deltaTime) {    
    Star *star;
    for (int i = 0; i < STAR_MAX_COUNT; i++) {
        star = &_stars[i];
        if (!star)
            continue;

        // printf("star at index %i is at (%.2f, %.2f) has velocity (%.2f, %.2f).\n", i, star->pos.x, star->pos.y, star->vel.x, star->vel.y);

        _Star_Update(star, deltaTime);
        if (_Star_Check(star))
            _Star_Render(star);
        else {
            _Star_Reset(star);
            _Star_Init(i);
        }
    }    
}

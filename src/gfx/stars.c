#include "../game/time.h"

#include "renderer.h"
#include "stars.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static Star _stars[STAR_MAX_COUNT];

static float _rand_velocity() {
    return (5.f * (1 + (rand() % 5)) / 100); 
}

static float _rand_x_position() {
    return 1.f * (rand() % WINDOW_WIDTH);
}

static float _rand_y_position() {
    return 1.f * (rand() % WINDOW_HEIGHT);
}

static star_state_t _rand_state() {
    switch (rand() % 2) {
    case 0:
        return STATE_FADEIN;
    case 1:
        return STATE_FADEOUT;
    default:
        return STATE_NORMAL;
    }
}

static uint64_t _rand_color() {
    uint64_t color = COLOR_STAR;
    color += (rand() % 0xFF);
    return color;
}

static Star *_Star_Init(int index) {
    Star *star = &_stars[index];
    memset(star, 0, sizeof(Star));

    star->pos.x = _rand_x_position();
    star->pos.y = WINDOW_HEIGHT - STAR_HEIGHT;

    star->vel.y = _rand_velocity();

    star->state = _rand_state();
    star->color = _rand_color();

    return star;
}

static void _Star_Update(Star *self, uint64_t deltaTime) {
    self->pos.y -= (1.f * deltaTime * self->vel.y);
}

static bool _Star_Check(const Star *self) {
    return (self->pos.y > 0.f); 
}

static void _Star_Render(const Star *self) {
    DrawRect(self->pos.x, self->pos.y, STAR_WIDTH, STAR_HEIGHT, self->color);
}

static void _Star_Flash(Star *self, uint64_t deltaTime) {
    const uint8_t alpha = self->color & 0xFF;
    
    switch (self->state) {
    case STATE_FADEIN:
        if (alpha < 0xFF)
            self->color += deltaTime;
        else
            self->state = STATE_FADEOUT;
        break;
    case STATE_FADEOUT:
        if (alpha > 0)
            self->color -= deltaTime;
        else
            self->state = STATE_FADEIN;
        break;
    default:
        break;
    }
}

void Stars_Init() {
    // prepare random number gen
    srand(time(NULL));

    // initialize stars
    assert(_stars);

    // assign random star positions
    Star *star;
    for (int i = 0; i < STAR_MAX_COUNT; i++) {
        star = _Star_Init(i);
        star->pos.y = _rand_y_position();
    }

    printf("Stars initialized.\n");
}

void Stars_Update(uint64_t deltaTime) {    
    Star *star;
    for (int i = 0; i < STAR_MAX_COUNT; i++) {
        star = &_stars[i];
        if (!star)
            break;

        // printf("star at index %i is at (%.2f, %.2f) has velocity (%.2f, %.2f) and color 0x%x.\n", i, star->pos.x, star->pos.y, star->vel.x, star->vel.y, star->color);

        _Star_Update(star, deltaTime);
        if (_Star_Check(star)) {
            _Star_Flash(star, deltaTime);
            _Star_Render(star);
        } else {
            _Star_Init(i);
        }
    }    
}

#ifndef _GAME_H_
#define _GAME_H_

#include "game/stage.h"
#include "game/world.h"
#include "gfx/ui.h"

#include <stdbool.h>

#define GAME_TITLE "GALAGA"

typedef enum {
    G_QUIT = 0,
    G_IDLE,
    G_PAUSED,
    G_PLAYING,
} game_state_t;

typedef struct Game {
    game_state_t state;
    Stage stage;
    World world;
    UI ui;
} Game;

void game_init(Game *);
void game_update(Game *);
void game_destroy(Game *);

void game_main(Game *);

void game_pause(Game *);
void game_resume(Game *);
void game_reset(Game *);
void game_quit(Game *);

#endif

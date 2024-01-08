#ifndef _GAME_H_
#define _GAME_H_

#include "game/stage.h"
#include "game/world.h"

#include <stdbool.h>

#define GAME_TITLE "GALAGA"

typedef enum {
    G_QUIT = 0,
    G_IDLE,
    G_PAUSED,
    G_PLAYING,
} game_state_t;

typedef struct {
    game_state_t state;

    World world;
    Stage stage;
} Game;

void game_init(Game *);
void game_update(Game *);
void game_destroy(Game *);
void game_main(Game *);

#endif

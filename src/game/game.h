#ifndef _GAME_H_
#define _GAME_H_

#include "game/fps.h"
#include "game/stage.h"
#include "game/world.h"

#include <stdint.h>
#include <stdbool.h>

#define GAME_TITLE          "GALAGA"

typedef struct {
    fps_s fps;
    Stage stage;
    World world;
} Game;

bool Game_IsRunning(Game *self);
void Game_Init(Game *self);
void Game_Destroy(Game *self);
void Game_Main(Game *self);

#endif

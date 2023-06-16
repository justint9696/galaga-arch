#ifndef _GAME_H_
#define _GAME_H_

#include <stdint.h>
#include <stdbool.h>

#define FPS_TARGET      60
#define FRAME_DELAY     (1000.f / FPS_TARGET)

bool Game_IsRunning();

void Game_Init();

void Game_Main();

#endif

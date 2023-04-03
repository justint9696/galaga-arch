#ifndef _GAME_H_
#define _GAME_H_

#include <stdint.h>

#define FPS_TARGET      120
#define FRAME_DELAY     (1000.f / FPS_TARGET)

void Game_Init();

void Game_Main(int buttons);

#endif

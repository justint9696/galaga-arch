#ifndef _GAME_H_
#define _GAME_H_

#include <stdint.h>

uint64_t Time_Passed(uint64_t tick);

void Game_Init();

void Game_Main(int buttons);

#endif

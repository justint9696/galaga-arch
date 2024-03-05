#ifndef _GAME_HUD_H_
#define _GAME_HUD_H_

#include "gfx/window.h"

#define HUD_Y       SCREEN_HEIGHT - 80
#define HUD_DIST    18

typedef struct Game Game;

void game_hud_update(Game *);
void game_hud_reset(Game *);

#endif

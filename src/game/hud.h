#ifndef _GAME_HUD_H_
#define _GAME_HUD_H_

#include "gfx/window.h"

#define HUD_Y       SCREEN_HEIGHT - 80
#define HUD_DIST    18

struct Game;

void game_hud_init(struct Game *);
void game_hud_update(struct Game *);
void game_hud_reset(struct Game *);

#endif

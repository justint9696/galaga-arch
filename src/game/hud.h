#ifndef _GAME_HUD_H_
#define _GAME_HUD_H_

struct Game;

void game_hud_init(struct Game *);
void game_hud_update(struct Game *);
void game_hud_reset(struct Game *);

#endif

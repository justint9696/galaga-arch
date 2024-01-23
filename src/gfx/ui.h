#ifndef _GFX_UI_H_
#define _GFX_UI_H_

#include "gfx/hud.h"

#include <stdbool.h>
#include <stdint.h>

#define UI_MAX 12

struct Game;
typedef void(*game_f)(struct Game *);

typedef struct {
    uint32_t index;
    char title[STR_LEN_MAX];
    game_f function;
} UI_Item;

typedef struct {
    size_t size;
    int32_t pos;
    UI_Item items[UI_MAX]; 
} UI;

void ui_init(UI *);
void ui_add(UI *, const char *title, game_f);
int32_t ui_scroll(UI *, int32_t);
game_f ui_select(UI *);
bool ui_item_selected(const UI *, const UI_Item *);

#endif